#ifndef CONSOLE_H
#define CONSOLE_H
#pragma once
#include <Windows.h>
#include <fmt/ostream.h>

// tat ca cac ham thuong dung lien quan den xu ly console
namespace console {
	// setup unicode cho cmd, show_cursor la de hien/an cai nha'y nha'y
	void SetupConsole(bool show_cursor = true);

	// hien/an cai nha'y nha'y
	void ShowConsoleCursor(bool show,
												 HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE));

	// clear man hinh
	bool ClearConsoleScreen();

	std::optional<COORD> GetConsoleCursorPosition(
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE));
	BOOL SetConsoleCursorAt(SHORT x,
													SHORT y,
													HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE));

	// doc input duoi dang utf-8, flush... la de xoa toan du lieu tu input truoc
	// khi read
	Utf8String ReadUtf8String(bool flush_input_before_read = true);

	SignedInteger CountCharacterInUtf8String(Utf8StringView str);
}  // namespace console
#endif  // !CONSOLE_H
