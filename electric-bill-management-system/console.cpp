/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "console.h"
#include <QtCore/QProcess>
#include <rang.hpp>
// !Substitute typedef HANDLE to void* because of misc-misplaced-const

void console::SetupConsole(const bool show_cursor) {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	setWinTermMode(rang::winTerm::Ansi);
	setControlMode(rang::control::Force);

	ShowConsoleCursor(show_cursor);
}

void console::ShowConsoleCursor(const bool show, void* handle) {
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(handle, &cursorInfo);
	cursorInfo.bVisible = static_cast<BOOL>(show);  // set the cursor visibility
	SetConsoleCursorInfo(handle, &cursorInfo);
}

bool console::ClearConsoleScreen() {
	const auto result = QProcess::execute(u8"cmd /c cls");
	return result != -2 && result != -1;
}

std::optional<COORD> console::GetConsoleCursorPosition(void* handle) {
	CONSOLE_SCREEN_BUFFER_INFOEX console_screen_buffer_info;
	std::optional<COORD> result{};
	if (GetConsoleScreenBufferInfoEx(handle, &console_screen_buffer_info) != 0) {
		result = console_screen_buffer_info.dwCursorPosition;
	}
	return result;
}

BOOL console::SetConsoleCursorAt(const SHORT x, const SHORT y, void* handle) {
	const auto result = SetConsoleCursorPosition(handle, {x, y});
	return result;
}

Utf8String console::ReadUtf8String(const bool flush_input_before_read) {
	static constexpr auto MAX_INPUT_LENGTH = 255;

	wchar_t wide_string[MAX_INPUT_LENGTH];

	char utf8_string[MAX_INPUT_LENGTH * 3 + 1];

	DWORD total_char_read{};
	auto* handle = GetStdHandle(STD_INPUT_HANDLE);
	if (flush_input_before_read) {
		FlushConsoleInputBuffer(handle);
	}
	ReadConsole(handle, wide_string, MAX_INPUT_LENGTH, &total_char_read, nullptr);

	auto const size = WideCharToMultiByte(CP_UTF8,
																				0,
																				wide_string,
																				total_char_read,
																				utf8_string,
																				sizeof(utf8_string),
																				nullptr,
																				nullptr);
	utf8_string[size] = 0;
	return QString(utf8_string).trimmed().toStdString();
}

SignedInteger console::CountCharacterInUtf8String(Utf8StringView str) {
	return std::accumulate(
			str.begin(), str.end(), 0, [](const SignedInteger res, const char ch) {
				return res + ((ch & 0xc0) != 0x80);
			});
}
