/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "main_view.h"
#include "console.h"
#include "console_listener.h"
#include "menu.h"
using menu::MenuBase;
using menu::MenuItem;
using menu::MenuRenderer;
using menu::MenuState;
using menu::SubMenu;
using std::make_unique;
using std::unique_ptr;
using view::MainView;
namespace {
	auto* make_info_menu() {
		auto result =
				make_unique<SubMenu>(u8"Quản Lý Thông Tin Người Dùng", nullptr);
		result->SetParent(nullptr);
		[[maybe_unused]] const auto* add_item =
				result->AddMenuItem(u8"Thêm Người Dùng Mới");
		[[maybe_unused]] const auto* edit_item =
				result->AddMenuItem(u8"Sửa Đổi Thông Tin Người Dùng");
		[[maybe_unused]] const auto* del_item =
				result->AddMenuItem(u8"Xóa Người Dùng");
		return result.release();
	}

	auto* make_search_menu() {
		auto result = make_unique<MenuItem>(u8"Tìm Kiếm Thông Tin");
		return result.release();
	}

	auto* make_summary_menu() {
		auto result = make_unique<SubMenu>(u8"Thống Kê");
		[[maybe_unused]] const auto* paid_item =
				result->AddMenuItem(u8"Đã Thanh Toán");
		[[maybe_unused]] const auto* non_paid_item =
				result->AddMenuItem(u8"Chưa Thanh Toán");
		return result.release();
	}  // namespace

	auto* make_bill_print_menu() {
		auto result = make_unique<MenuItem>(u8"In Hóa Đơn");
		return result.release();
	}

	auto* make_exit_program() {
		auto result = make_unique<MenuItem>(u8"Thoát");
		return result.release();
	}
}  // namespace

MainView::MainView()
		: initialize_handle_{std::async(std::launch::async,
																		&MainView::Initialize,
																		this)},
			listener_{std::make_unique<ConsoleListener>(this)} {
	connect(listener_.get(),
					&ConsoleListener::KeyEventReceived,
					this,
					&MainView::OnKeyboardEventReceived);
}

MainView::~MainView() {}

void MainView::Run() {
	initialize_handle_.wait();
	RenderAll();
}

void MainView::RenderAll() const {
	using fmt::arg;
	using fmt::format;
	using fmt::print;
	console::ClearConsoleScreen();
	static const auto align =
			format("{content:\t^{align}}", arg("content", ""), arg("align", tabs));
	static const auto title =
			format(u8"{align}HỆ THỐNG QUẢN LÝ TIỀN ĐIỆN\n\n", arg(u8"align", align));
	static const auto tutorial = format(
			u8"\n\n{align}SỬ DỤNG PHÍM LÊN/XUỐNG ĐỂ DI CHUYỂN\n"
			u8"{align}SỬ DỤNG PHÍM ENTER ĐỂ LỰA CHỌN MENU\n"
			u8"{align}SỬ DỤNG ESC ĐỂ THOÁT MENU HIỆN TẠI\n",
			arg(u8"align", align));
	print(title);
	for (const auto& menu : menu_ui_) {
		print(Render(menu.get()));
	}
	print(tutorial);
}

Utf8String MainView::Render(const MenuBase* menu) const {
	Utf8String result{};

	if (menu != nullptr) {
		if (menu == current_focus_) {
			result = MenuRenderer::Render(
					menu, current_focus_->GetLevel() + tabs, MenuState::FOCUS);
		}
		else if (menu == current_focus_->GetParent()) {
			result = MenuRenderer::Render(
					menu, menu->GetLevel() + tabs, MenuState::SELECTED);
			for (const auto* child : menu->children()) {
				const auto* menu_base_cast = qobject_cast<const MenuBase*>(child);
				result += Render(menu_base_cast);
			}
		}
		else {
			result = MenuRenderer::Render(
					menu, menu->GetLevel() + tabs, MenuState::INACTIVE);
		}
	}

	return result;
}

void MainView::Initialize() {
	console::SetupConsole(false);
	menu_ui_.clear();
	menu_ui_.push_back(unique_ptr<SubMenu>(make_info_menu()));
	menu_ui_.push_back(unique_ptr<MenuItem>(make_search_menu()));
	menu_ui_.push_back(unique_ptr<SubMenu>(make_summary_menu()));
	menu_ui_.push_back(unique_ptr<MenuItem>(make_bill_print_menu()));
	menu_ui_.push_back(unique_ptr<MenuItem>(make_exit_program()));

	current_focus_ = menu_ui_.front().get();
	current_menu_index_ = 0U;
}

void MainView::OnUpKey() {
	// if current focus have parent
	if (current_focus_->HasParent()) {
		// if current focus have previous sibling, move focus to it
		if (auto* prev_sibling = current_focus_->GetPreviousSibling();
				prev_sibling != nullptr) {
			current_focus_ = prev_sibling;
			RenderAll();
		}
		// else, try to move to last sibling
		else if (auto* last_sibling = qobject_cast<MenuBase*>(
								 current_focus_->GetParent()->children().back());
						 last_sibling != nullptr) {
			current_focus_ = last_sibling;
			RenderAll();
		}
	}
	// if current focus is root
	else {
		// move focus to prev menu
		if (current_menu_index_.value() > 0U) {
			--current_menu_index_.value();
		}
		// move focus to last menu
		else {
			current_menu_index_.value() = menu_ui_.size() - 1U;
		}
		current_focus_ = menu_ui_.at(current_menu_index_.value()).get();
		RenderAll();
	}
}

void MainView::OnDownKey() {
	// if current focus have parent
	if (current_focus_->HasParent()) {
		// if current focus have next sibling, move focus to it
		if (auto* next_sibling = current_focus_->GetNextSibling();
				next_sibling != nullptr) {
			current_focus_ = next_sibling;
			RenderAll();
		}
		// else, try to move to first sibling
		else if (auto* first_sibling = qobject_cast<MenuBase*>(
								 current_focus_->GetParent()->children().front());
						 first_sibling != nullptr) {
			current_focus_ = first_sibling;
			RenderAll();
		}
	}
	// if current focus is root
	else {
		// move focus to next menu
		if (current_menu_index_.value() < menu_ui_.size() - 1U) {
			++current_menu_index_.value();
		}
		// move focus to first menu
		else {
			current_menu_index_.value() = 0U;
		}
		current_focus_ = menu_ui_.at(current_menu_index_.value()).get();
		RenderAll();
	}
}

void MainView::OnEnterKey() {
	switch (current_focus_->GetType()) {
		case menu::MENU_BASE:
			break;
		case menu::MENU_ITEM:
			break;
		case menu::SUB_MENU:
			if (auto* child =
							qobject_cast<MenuBase*>(current_focus_->children().front());
					child != nullptr) {
				current_focus_ = child;
				RenderAll();
			}
			break;
		default:
			// do nothing
			break;
	}
}

void MainView::OnEscapeKey() {
	// if current focus has parent, move focus to parent, collapse parent
	if (current_focus_->HasParent()) {
		current_focus_ = current_focus_->GetParent();
		RenderAll();
	}
}

void MainView::OnKeyboardEventReceived(
		const ConsoleListener::ControlKeyboard key) {
	switch (key) {
		case ConsoleListener::ControlKeyboard::UP:
			OnUpKey();
			break;
		case ConsoleListener::ControlKeyboard::DOWN:
			OnDownKey();
			break;
		case ConsoleListener::ControlKeyboard::ENTER:
			OnEnterKey();
			break;
		case ConsoleListener::ControlKeyboard::ESC:
			OnEscapeKey();
			break;
		case ConsoleListener::ControlKeyboard::INVALID_INPUT:
			break;
		default:
			// do nothing
			break;
	}
}
