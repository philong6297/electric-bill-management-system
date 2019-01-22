#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H
#pragma once
#include <future>
#include <vector>
#include "console_listener.h"
#include "view_base.h"
namespace menu {
	enum MenuState : UnsignedInteger;
	class MenuBase;
}  // namespace menu
namespace view {

	class MainView final : public ViewBase {
		Q_OBJECT

	 public:
		explicit MainView();
		~MainView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(MainView)
		void Run() override;

		// cac loai message gui toi Program
		enum MenuItemId : UnsignedInteger {
			ADD_USER,
			EDIT_USER,
			DELETE_USER,
			SEARCH_INFO,
			SUMMARY_PAID_BILL,
			SUMMARY_NON_PAID_BILL,
			PRINT_BILL,
			EXIT
		};

	 signals:
		// send message toi program
		void MenuItemSelected(UnsignedInteger menu_item_id);

	 private:
		std::future<void> initialize_handle_;

		// console listener
		gsl::owner<ConsoleListener*> listener_;

		std::vector<std::unique_ptr<menu::MenuBase>> menu_ui_;
		gsl::owner<const menu::MenuBase*> current_focus_ = nullptr;
		std::optional<size_t> current_menu_index_ = std::nullopt;
		void Render();
		Utf8String GetFormattedString(const menu::MenuBase* menu) const;
		void Initialize();

		// *xu ly cac input
		void OnKeyboardEventUp();
		void OnKeyboardEventDown();
		void OnKeyboardEventEnter();
		void OnKeyboardEventEscape();
	 private slots:
		// slot nhan signal tu console_listener
		void OnKeyboardEventReceived(ConsoleListener::ControlKeyboard key);
	};
}  // namespace view
#endif  // !MAIN_VIEW_H
