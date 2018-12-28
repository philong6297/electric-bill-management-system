#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#pragma once
#include "menu_base.h"
namespace menu {
	class MenuItem final : public MenuBase {
		Q_OBJECT

	 public:
		using MenuBase::MenuBase;
		~MenuItem() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(MenuItem)

		MenuType GetType() const override;
	};
}  // namespace menu

#endif  // !MENU_ITEM_H
