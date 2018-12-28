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

	 private:
		std::future<void> initialize_handle_;
		std::unique_ptr<ConsoleListener> listener_;
		std::vector<std::unique_ptr<menu::MenuBase>> menu_ui_;
		gsl::owner<menu::MenuBase*> current_focus_ = nullptr;
		std::optional<size_t> current_menu_index_ = std::nullopt;
		static constexpr auto tabs = 2;
		void RenderAll() const;
		Utf8String Render(const menu::MenuBase* menu) const;
		void Initialize();

		void OnUpKey();
		void OnDownKey();
		void OnEnterKey();
		void OnEscapeKey();
	 private slots:
		void OnKeyboardEventReceived(ConsoleListener::ControlKeyboard key);
	};
}  // namespace view
#endif  // !MAIN_VIEW_H
