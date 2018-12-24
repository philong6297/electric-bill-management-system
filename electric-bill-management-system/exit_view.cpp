/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/


#include "exit_view.h"
#include <fmt/format.h>
#include "console.h"
#include "view.h"
view::ExitView::~ExitView() = default;

void view::ExitView::Run() {
	Render();
}

void view::ExitView::Render() const {
	console::ClearConsoleScreen();
	fmt::print(u8"KẾT THÚC CHƯƠNG TRÌNH...\n");
	emit Request(RequestPackage{EXIT_VIEW, QUIT});
}
