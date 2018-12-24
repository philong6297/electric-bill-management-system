#ifndef CONSOLE_H
#define CONSOLE_H
#pragma once
#include <Windows.h>
#include <fmt/ostream.h>

namespace console {
	void SetupConsole();
	bool ClearConsoleScreen(HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE));
	std::optional<COORD> GetConsoleCursorPosition(
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE));
	BOOL SetConsoleCursorAt(SHORT x,
													SHORT y,
													HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE));

	enum class ControlKeyboard : UnsignedInteger {
		UP,
		DOWN,
		ENTER,
		ESC,
		INVALID_INPUT
	};
	ControlKeyboard WaitForKeyboardEvent(
			HANDLE handle = GetStdHandle(STD_INPUT_HANDLE));
}  // namespace console
std::ostream& operator<<(std::ostream& os,
												 const std::optional<COORD>& coordinate);
#endif  // !CONSOLE_H
