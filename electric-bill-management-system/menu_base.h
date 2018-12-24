#ifndef MENU_BASE_H
#define MENU_BASE_H
#pragma once

#include <fmt/ostream.h>
#include "menu_style.h"

namespace menu {
	class SubMenu;
	class MenuBase;

	class MenuBase : public QObject {
		Q_OBJECT

	 public:
		explicit MenuBase() = delete;
		explicit MenuBase(Utf8StringView title, SubMenu* parent = nullptr);
		~MenuBase() override = default;
		MAKE_NON_COPYABLE_NON_MOVEABLE(MenuBase)

		void Show() const;

		UnsignedInteger GetLevel() const;

		Utf8String GetTitle() const;

		void SetTitle(Utf8StringView title);

		std::optional<SignedInteger> GetId() const;

		bool HasParent() const;

		void SetParent(SubMenu* new_parent);

		bool IsFocus() const;

		void SetFocus(bool is_focus);

		MenuBase* NextSibling() const;

		MenuBase* PreviousSibling() const;

		MenuBase* SiblingAt(SignedInteger index) const;

		friend std::ostream& operator<<(std::ostream& os, const MenuBase& menu) {
			os << menu.GetCurrentContent() << reset_style();
			return os;
		}

		virtual Utf8String GetCurrentContent() const;

	 private:
		using QObject::setParent;

		Utf8String title_;

		UnsignedInteger level_;

		std::optional<SignedInteger> id_;

		bool is_focus_;

		void on_parent_changed(const SubMenu* new_parent);
	};

}  // namespace menu
#endif  // !MENU_BASE_H
