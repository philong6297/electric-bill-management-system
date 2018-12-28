/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/
#include "menu_item.h"
using menu::MenuItem;

MenuItem::~MenuItem() = default;

menu::MenuType MenuItem::GetType() const {
	return MENU_ITEM;
}
