#ifndef CONSOLE_LISTENER_H
#define CONSOLE_LISTENER_H

#pragma once
#include <QtCore/QWinEventNotifier>
// la class listen cac input tu nguoi dung, vi du nhu ng dung an phim nao,
// class nay se notify toi main view
class ConsoleListener final : public QObject {
	Q_OBJECT

 public:
	enum ListenType : UnsignedInteger { SINGLE_KEY, STRING };
	enum ControlKeyboard { UP, DOWN, ENTER, ESC, INVALID_INPUT };

	explicit ConsoleListener(QObject* parent, ListenType type);
	~ConsoleListener() override;
	MAKE_NON_COPYABLE_NON_MOVEABLE(ConsoleListener)

 signals:
	// send tin hieu toi main view key nao dang duoc su dung
	void SingleKeyReceived(ControlKeyboard key);
	void StringReceived(Utf8StringView input);

 private:
	gsl::owner<QWinEventNotifier*> notifier_;
 private slots:
	void ListenKeyEvent(Qt::HANDLE handle);
	void ListenStringEvent(Qt::HANDLE handle);
};

#endif  // !CONSOLE_LISTENER_H
