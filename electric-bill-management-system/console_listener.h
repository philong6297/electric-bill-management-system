#ifndef CONSOLE_LISTENER_H
#define CONSOLE_LISTENER_H

#pragma once
#include <QtCore/QWinEventNotifier>
class ConsoleListener final : public QObject {
	Q_OBJECT

 public:
	explicit ConsoleListener(QObject* parent);
	~ConsoleListener() override;
	MAKE_NON_COPYABLE_NON_MOVEABLE(ConsoleListener)
	enum class ControlKeyboard : UnsignedInteger {
		UP,
		DOWN,
		ENTER,
		ESC,
		INVALID_INPUT
	};
 signals:
	void KeyEventReceived(ControlKeyboard key);

 private:
	std::unique_ptr<QWinEventNotifier> notifier_;

	void ListenKeyEvent(Qt::HANDLE handle);
};

#endif  // !CONSOLE_LISTENER_H
