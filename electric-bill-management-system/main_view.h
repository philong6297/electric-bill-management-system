#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#pragma once
#include <future>
#include <vector>
#include "view_base.h"

namespace console {
	enum class ControlKeyboard : UnsignedInteger;
}  // namespace console

namespace menu {
	class MenuBase;
}  // namespace menu
class Program;

namespace view {
	class MainView final : public ViewBase {
		Q_OBJECT
	 public:
		explicit MainView(Program* parent);
		~MainView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(MainView)

		void Run() override;

		enum RequestMessage : UnsignedInteger {
			ADD_USER,
			EDIT_USER,
			DELETE_USER,
			SEARCH_INFO,
			SUMMARY_PAID_BILL,
			SUMMARY_NON_PAID_BILL,
			PRINT_BILL,
			EXIT
		};

	 private:
		std::vector<std::unique_ptr<menu::MenuBase>> menu_ui_{};
		menu::MenuBase* current_focus_ = nullptr;
		std::optional<UnsignedInteger> current_menu_index_{};
		bool should_run_ = false;
		bool should_render_ = false;
		std::future<void> initialize_handle_{};
		void Render() const override;
		void OnKeyboardEvent(console::ControlKeyboard event);
		void OnUp();
		void OnDown();
		void OnEscape();
		void OnEnter();
		void OnChangeNewFocus(menu::MenuBase* new_focus);
		void Initialize();
	};
}  // namespace view

#endif  // !MAIN_VIEW_H
