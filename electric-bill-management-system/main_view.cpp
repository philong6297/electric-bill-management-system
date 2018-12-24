/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "main_view.h"
#include "console.h"
#include "menu.h"
#include "program.h"
#include "view.h"
using menu::MenuBase;
using menu::MenuItem;
using menu::SubMenu;
using std::make_unique;
using std::unique_ptr;
using view::MainView;
namespace {
	auto* make_info_menu() {
		auto result =
				make_unique<SubMenu>(u8"Quản Lý Thông Tin Người Dùng", nullptr);
		result->SetParent(nullptr);
		result->AddMenuItem(u8"Thêm Người Dùng Mới",
												MainView::RequestMessage::ADD_USER);
		result->AddMenuItem(u8"Sửa Đổi Thông Tin Người Dùng",
												MainView::RequestMessage::EDIT_USER);
		result->AddMenuItem(u8"Xóa Người Dùng",
												MainView::RequestMessage::DELETE_USER);
		return result.release();
	}

	auto* make_search_menu() {
		auto result = make_unique<MenuItem>(
				u8"Tìm Kiếm Thông Tin", MainView::RequestMessage::SEARCH_INFO, nullptr);
		return result.release();
	}

	auto* make_summary_menu() {
		auto result = make_unique<SubMenu>(u8"Thống Kê", nullptr);
		result->AddMenuItem(u8"Đã Thanh Toán",
												MainView::RequestMessage::SUMMARY_PAID_BILL);
		result->AddMenuItem(u8"Chưa Thanh Toán",
												MainView::RequestMessage::SUMMARY_NON_PAID_BILL);
		return result.release();
	}  // namespace

	auto* make_bill_print_menu() {
		auto result = make_unique<MenuItem>(
				u8"In Hóa Đơn", MainView::RequestMessage::PRINT_BILL, nullptr);
		return result.release();
	}

	auto* make_exit_program() {
		auto result = make_unique<MenuItem>(
				u8"Thoát", MainView::RequestMessage::EXIT, nullptr);
		return result.release();
	}
}  // namespace

MainView::MainView(Program* parent)
		: ViewBase{parent},
			initialize_handle_{
					std::async(std::launch::async, &MainView::Initialize, this)} {}

MainView::~MainView() = default;

void MainView::Render() const {
	using fmt::arg;
	using fmt::format;
	using fmt::print;
	console::ClearConsoleScreen();
	console::SetupConsole();
	const auto title = format(u8"{}", u8"CHƯƠNG TRÌNH QUẢN LÝ TIỀN ĐIỆN");
	print(u8"{title}\n\n", arg(u8"title", title));
	for (const auto& menu : menu_ui_) {
		print(u8"{menu}", arg(u8"menu", menu->GetCurrentContent()));
	}
	print(
			u8"\n\nSỬ DỤNG PHÍM LÊN XUỐNG ĐỂ DI CHUYỂN\n"
			u8"ENTER ĐỂ CHỌN MENU\n"
			u8"ESC ĐỂ THOÁT MENU ĐANG CHỌN");
}

void MainView::Run() {
	// run should wait for initialize
	initialize_handle_.wait();
	while (should_run_) {
		if (should_render_) {
			Render();
		}
		const auto event = console::WaitForKeyboardEvent();
		OnKeyboardEvent(event);
	}
}

void MainView::OnKeyboardEvent(const console::ControlKeyboard event) {
	switch (event) {
		case console::ControlKeyboard::UP:
			OnUp();
			break;
		case console::ControlKeyboard::DOWN:
			OnDown();
			break;
		case console::ControlKeyboard::ENTER:
			OnEnter();
			break;
		case console::ControlKeyboard::ESC:
			OnEscape();
			break;
		case console::ControlKeyboard::INVALID_INPUT:
			break;
		default:
			// do nothing
			break;
	}
}

void MainView::OnUp() {
	assert(current_menu_index_.has_value() && current_focus_ != nullptr);
	should_run_ = true;
	// if have previous sibling, move focus to it
	if (const auto prev_sibling = current_focus_->PreviousSibling();
			prev_sibling != nullptr) {
		OnChangeNewFocus(prev_sibling);
	}
	// if current focus is root and is not the first menu in menu ui, move
	// focus to prev menu
	else if (current_focus_->GetLevel() == 0U &&
					 current_menu_index_.value() > 0U) {
		--current_menu_index_.value();
		OnChangeNewFocus(
				menu_ui_.at(static_cast<size_t>(current_menu_index_.value())).get());
	}
	else {
		should_render_ = false;
	}
}

void MainView::OnDown() {
	assert(current_menu_index_.has_value() && current_focus_ != nullptr);

	should_run_ = true;
	// if have next sibling, move focus to it
	if (const auto next_sibling = current_focus_->NextSibling();
			next_sibling != nullptr) {
		OnChangeNewFocus(next_sibling);
	}
	// if current focus is a root and is not the last menu in menu ui, move
	// focus to next menu
	else if (!current_focus_->HasParent() &&
					 static_cast<size_t>(current_menu_index_.value()) <
							 this->menu_ui_.size() - 1U) {
		++current_menu_index_.value();
		OnChangeNewFocus(
				menu_ui_.at(static_cast<size_t>(current_menu_index_.value())).get());
	}
	else {
		should_render_ = false;
	}
}

void MainView::OnEscape() {
	assert(current_menu_index_.has_value() && current_focus_ != nullptr);

	should_run_ = true;
	// if current focus has parent, move focus to parent, collapse parent
	if (current_focus_->HasParent()) {
		if (auto* parent = qobject_cast<SubMenu*>(current_focus_->parent());
				parent != nullptr) {
			parent->SetExpand(false);
			OnChangeNewFocus(parent);
		}
		should_render_ = true;
	}
	else {
		should_render_ = false;
	}
}

void MainView::OnEnter() {
	assert(current_menu_index_.has_value() && current_focus_ != nullptr);

	// if current focus is sub menu
	if (auto* cast_to_sub_menu = qobject_cast<SubMenu*>(current_focus_);
			cast_to_sub_menu != nullptr) {
		// if is expand, collapse it
		if (cast_to_sub_menu->IsExpand()) {
			cast_to_sub_menu->SetExpand(false);
		}
		// else expand, set focus to first child
		else {
			cast_to_sub_menu->SetExpand(true);
			OnChangeNewFocus(
					qobject_cast<MenuBase*>(cast_to_sub_menu->children().front()));
		}
		should_render_ = true;
		should_run_ = true;
	}
	// if current focus is menu item
	else if (auto* cast_to_menu_item = qobject_cast<MenuItem*>(current_focus_);
					 cast_to_menu_item != nullptr) {
		emit Request(RequestPackage{MAIN_VIEW, cast_to_menu_item->GetCommand()});
		should_render_ = false;
		should_run_ = false;
	}
	else {
		// do nothing
		should_render_ = false;
		should_run_ = true;
	}
}

void MainView::OnChangeNewFocus(MenuBase* new_focus) {
	if (current_focus_ != nullptr) {
		current_focus_->SetFocus(false);
	}
	new_focus->SetFocus(true);
	current_focus_ = new_focus;
	should_render_ = true;
}

void MainView::Initialize() {
	if (!menu_ui_.empty()) {
		menu_ui_.clear();
	}
	menu_ui_.push_back(unique_ptr<SubMenu>(make_info_menu()));
	menu_ui_.push_back(unique_ptr<MenuItem>(make_search_menu()));
	menu_ui_.push_back(unique_ptr<SubMenu>(make_summary_menu()));
	menu_ui_.push_back(unique_ptr<MenuItem>(make_bill_print_menu()));
	menu_ui_.push_back(unique_ptr<MenuItem>(make_exit_program()));

	current_focus_ = menu_ui_.front().get();
	current_focus_->SetFocus(true);
	current_menu_index_ = 0;
	should_render_ = true;
	should_run_ = true;
}
