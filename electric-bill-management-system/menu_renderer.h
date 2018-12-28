#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H
#pragma once
#include "menu_defines.h"
namespace menu {
	class MenuBase;
	struct MenuRenderer final {
		static Utf8String Render(const MenuBase* menu,
														 UnsignedInteger align,
														 MenuState state);
	};
}  // namespace menu

#endif  // !MENU_RENDERER_H
