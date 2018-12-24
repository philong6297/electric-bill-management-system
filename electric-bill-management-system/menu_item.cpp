/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "menu_item.h"
#include "menu.h"
using menu::MenuBase;
using menu::MenuItem;
using menu::SubMenu;

MenuItem::MenuItem(const Utf8StringView title,
									 const UnsignedInteger command,
									 SubMenu* parent)
		: MenuBase(title, parent), command_{command} {}

Utf8String MenuItem::GetCurrentContent() const {
	return MenuBase::GetCurrentContent();
}

UnsignedInteger MenuItem::GetCommand() const {
	return command_;
}
