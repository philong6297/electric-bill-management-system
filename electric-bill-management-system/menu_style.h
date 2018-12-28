#ifndef MENU_STYLE_H
#define MENU_STYLE_H
#pragma once
#include <fmt/ostream.h>
#include <rang.hpp>
#include "menu_defines.h"
namespace menu {

	class MenuBase;
	struct MenuStyle final {
		rang::fg text_color;
		friend std::ostream& operator<<(std::ostream& os, const MenuStyle& style) {
			os << style.text_color;
			return os;
		}
		static MenuStyle GetStyle(MenuState state);
		static MenuStyle GetResetStyle();
	};
}  // namespace menu

#endif  // !MENU_STYLE_H
