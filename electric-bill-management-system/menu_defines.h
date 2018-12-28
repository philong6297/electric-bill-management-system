#ifndef MENU_DEFINES_H
#define MENU_DEFINES_H
#pragma once
namespace menu {
	enum MenuType : UnsignedInteger { MENU_BASE, MENU_ITEM, SUB_MENU };
	enum MenuState : UnsignedInteger { INACTIVE, FOCUS, SELECTED };
}  // namespace menu
#endif  // !MENU_DEFINES_H
