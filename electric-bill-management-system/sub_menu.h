#ifndef SUB_MENU_H
#define SUB_MENU_H
#pragma once
#include "menu_base.h"
namespace menu {
	class MenuItem;
	class SubMenu final : public MenuBase {
		Q_OBJECT

	 public:
		using MenuBase::MenuBase;
		~SubMenu() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(SubMenu)

		MenuItem* AddMenuItem(Utf8StringView title, UnsignedInteger id);
		SubMenu* AddSubMenu(Utf8StringView title);
		MenuType GetType() const override;
	};
}  // namespace menu

#endif  // !SUB_MENU_H
