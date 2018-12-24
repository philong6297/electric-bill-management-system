/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "console.h"
#include <rang.hpp>

// !Substitute typedef HANDLE to void* because of misc-misplaced-const

// helpers
namespace {
	std::optional<KEY_EVENT_RECORD> get_keyboard_event_record(void* h) {
		DWORD cc{};
		INPUT_RECORD input_record{};
		std::optional<KEY_EVENT_RECORD> result{};
		if (h != nullptr) {
			while (true) {
				ReadConsoleInput(h, &input_record, 1, &cc);
				if (input_record.EventType == KEY_EVENT) {
					if (const auto& key_event =
									reinterpret_cast<KEY_EVENT_RECORD&>(  // NOLINT
											input_record.Event);              // NOLINT
							key_event.bKeyDown != 0) {
						result = key_event;
						break;
					}
				}
			}
		}
		return result;
	}
}  // namespace

void console::SetupConsole() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	setWinTermMode(rang::winTerm::Ansi);
	setControlMode(rang::control::Force);
}

bool console::ClearConsoleScreen(void* handle) {
	using fmt::arg;
	using fmt::print;

	auto result = false;

	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(handle, &csbi) != 0) {
		const auto total_char_in_buffer = csbi.dwSize.X * csbi.dwSize.Y;
		DWORD total_char_written{};
		const auto top_left = COORD{0, 0};
		if (FillConsoleOutputCharacter(handle,
																	 static_cast<TCHAR>(' '),
																	 total_char_in_buffer,
																	 top_left,
																	 &total_char_written) != 0 &&
				GetConsoleScreenBufferInfo(handle, &csbi) != 0 &&
				FillConsoleOutputAttribute(handle,
																	 csbi.wAttributes,
																	 total_char_in_buffer,
																	 top_left,
																	 &total_char_written) != 0 &&
				SetConsoleCursorPosition(handle, top_left) != 0) {
			result = true;
		}
	}
	return result;
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

console::ControlKeyboard console::WaitForKeyboardEvent(void* handle) {
	auto result = ControlKeyboard::INVALID_INPUT;
	if (const auto keyboard_event = get_keyboard_event_record(handle);
			keyboard_event.has_value()) {
		const auto key = keyboard_event.value();
		switch (key.wVirtualKeyCode) {
			case VK_UP:
				result = ControlKeyboard::UP;
				break;
			case VK_DOWN:
				result = ControlKeyboard::DOWN;
				break;
			case VK_RETURN:
				result = ControlKeyboard::ENTER;
				break;
			case VK_ESCAPE:
				result = ControlKeyboard::ESC;
				break;
			default:
				// do nothing
				break;
		}
	}
	return result;
}

std::ostream& operator<<(
		std::ostream& os,
		[[maybe_unused]] const std::optional<COORD>& coordinate) {
#ifdef _DEBUG
	if (coordinate.has_value()) {
		os << '(' << coordinate.value().X << ',' << coordinate.value().Y << ')';
	}
	else {
		os << "(no coordinate)";
	}
#endif  // _DEBUG
	return os;
}
