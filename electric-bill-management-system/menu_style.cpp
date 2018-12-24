/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "menu_style.h"
#include "menu.h"
using menu::MenuStyle;

MenuStyle menu::reset_style() {
	static constexpr auto result = MenuStyle{rang::fg::reset};
	return result;
}

MenuStyle menu::get_style(const MenuBase& menu_base) {
	static constexpr auto focus = MenuStyle{rang::fg::yellow};
	static constexpr auto not_focus = MenuStyle{rang::fg::reset};
	auto result = not_focus;
	if (menu_base.IsFocus()) {
		result = focus;
	}
	return result;
}
