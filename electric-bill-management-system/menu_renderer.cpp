/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "menu_renderer.h"
#include "menu.h"
using menu::MenuBase;
using menu::MenuRenderer;
using menu::MenuState;
Utf8String MenuRenderer::Render(const MenuBase* menu,
																const UnsignedInteger align,
																const MenuState state) {
	using fmt::arg;
	using fmt::format;
	const auto align_string = format(
			u8"{content:\t^{align}}", arg(u8"content", u8""), arg(u8"align", align));
	const auto style = MenuStyle::GetStyle(state);
	const auto order = menu->HasParent()
												 ? format(u8"{}. ", menu->GetChildOrder().value())
												 : format(u8"");
	const auto reset = MenuStyle::GetResetStyle();
	const auto title = menu->GetTitle();
	auto result = format(u8"{align}{style}{order}{title}{reset}\n",
											 arg(u8"align", align_string),
											 arg(u8"style", style),
											 arg(u8"order", order),
											 arg("title", title),
											 arg(u8"reset", reset));

	return result;
}
