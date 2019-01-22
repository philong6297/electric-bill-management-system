/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "console_listener.h"
#include <Windows.h>
#include <iostream>
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

ConsoleListener::ConsoleListener(QObject* parent, const ListenType type)
		: QObject{parent},
			notifier_{new QWinEventNotifier{GetStdHandle(STD_INPUT_HANDLE), this}} {
	switch (type) {
		case SINGLE_KEY:
			connect(notifier_,
							&QWinEventNotifier::activated,
							this,
							&ConsoleListener::ListenKeyEvent);
			break;
		case STRING:
			connect(notifier_,
							&QWinEventNotifier::activated,
							this,
							&ConsoleListener::ListenStringEvent);
			break;
		default:
			// do nothing
			break;
	}
}

ConsoleListener::~ConsoleListener() = default;

void ConsoleListener::ListenKeyEvent(const Qt::HANDLE handle) {
	auto result = INVALID_INPUT;
	if (const auto keyboard_event = get_keyboard_event_record(handle);
			keyboard_event.has_value()) {
		const auto key = keyboard_event.value();
		switch (key.wVirtualKeyCode) {
			case VK_UP:
				result = UP;
				break;
			case VK_DOWN:
				result = DOWN;
				break;
			case VK_RETURN:
				result = ENTER;
				break;
			case VK_ESCAPE:
				result = ESC;
				break;
			default:
				// do nothing
				break;
		}
	}
	emit SingleKeyReceived(result);
}

void ConsoleListener::ListenStringEvent([[maybe_unused]] Qt::HANDLE handle) {
	Utf8String line{};
	std::getline(std::cin, line);
	emit StringReceived(line);
}
