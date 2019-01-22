/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "menu_item.h"
#include "menu.h"
using menu::MenuItem;
using menu::SubMenu;
MenuItem::MenuItem(const Utf8StringView title,
									 const UnsignedInteger id,
									 SubMenu* parent)
		: MenuBase{title, parent}, id_{id} {}

MenuItem::~MenuItem() = default;

UnsignedInteger MenuItem::GetId() const {
	return id_;
}

void MenuItem::SetId(const UnsignedInteger id) {
	id_ = id;
}

menu::MenuType MenuItem::GetType() const {
	return MENU_ITEM;
}
