/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "pch.h"
#include "console_listener.h"
#include <Windows.h>
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

ConsoleListener::ConsoleListener(QObject* parent)
		: QObject{parent},
			notifier_{
					std::make_unique<QWinEventNotifier>(GetStdHandle(STD_INPUT_HANDLE),
																							this)} {
	connect(notifier_.get(),
					&QWinEventNotifier::activated,
					this,
					&ConsoleListener::ListenKeyEvent);
}

ConsoleListener::~ConsoleListener() = default;

void ConsoleListener::ListenKeyEvent([[maybe_unused]] const Qt::HANDLE handle) {
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
	emit KeyEventReceived(result);
}
