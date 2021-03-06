/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "exit_view.h"
#include "console.h"
using view::ExitView;

ExitView::ExitView() : ViewBase{} {}

ExitView::~ExitView() = default;

void ExitView::Run() {
	console::SetupConsole();
	Render();
	emit Exit();
}

void ExitView::Render() {
	console::ClearConsoleScreen();
	static const auto message = fmt::format(u8"THOÁT CHƯƠNG TRÌNH...\n");
	fmt::print(message);
}
