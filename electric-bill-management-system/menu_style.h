#ifndef MENU_STYLE_H
#define MENU_STYLE_H

#pragma once
#include <fmt/ostream.h>
#include <rang.hpp>
namespace menu {
	class MenuBase;
	struct MenuStyle final {
		rang::fg text_color;
		friend std::ostream& operator<<(std::ostream& os, const MenuStyle& style) {
			os << style.text_color;
			return os;
		}
	};

	MenuStyle reset_style();

	MenuStyle get_style(const MenuBase& menu_base);
}  // namespace menu

#endif  // !MENU_STYLE_H
