#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#pragma once
#include "menu_base.h"
namespace menu {
	class SubMenu;
	class MenuItem final : public MenuBase {
		Q_OBJECT

	 public:
		explicit MenuItem(Utf8StringView title,
											UnsignedInteger id,
											SubMenu* parent = nullptr);
		~MenuItem() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(MenuItem)

		UnsignedInteger GetId() const;
		void SetId(UnsignedInteger id);
		MenuType GetType() const override;

	 private:
		UnsignedInteger id_;
	};
}  // namespace menu

#endif  // !MENU_ITEM_H
