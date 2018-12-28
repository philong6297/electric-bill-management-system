/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "menu_style.h"
#include "menu.h"
using menu::MenuState;
using menu::MenuStyle;
MenuStyle MenuStyle::GetStyle(const MenuState state) {
	using rang::fg;
	static constexpr auto focus = MenuStyle{fg::yellow};
	static constexpr auto inactive = MenuStyle{fg::reset};
	static constexpr auto select = MenuStyle{fg::blue};
	MenuStyle result{};
	switch (state) {
		case INACTIVE:
			result = inactive;
			break;
		case FOCUS:
			result = focus;
			break;
		case SELECTED:
			result = select;
			break;
		default:
			// do nothing
			break;
	}
	return result;
}

MenuStyle MenuStyle::GetResetStyle() {
	using rang::fg;
	static constexpr auto reset = MenuStyle{fg::reset};
	return reset;
}
