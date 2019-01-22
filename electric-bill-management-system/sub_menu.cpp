/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "sub_menu.h"
#include "menu.h"
using menu::SubMenu;

SubMenu::~SubMenu() = default;

menu::MenuItem* SubMenu::AddMenuItem(const Utf8StringView title,
																		 const UnsignedInteger id) {
	return std::make_unique<MenuItem>(title, id, this).release();
}

SubMenu* SubMenu::AddSubMenu(const Utf8StringView title) {
	return std::make_unique<SubMenu>(title, this).release();
}

menu::MenuType SubMenu::GetType() const {
	return SUB_MENU;
}
