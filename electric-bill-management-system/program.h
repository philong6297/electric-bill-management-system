#ifndef PROGRAM_H
#define PROGRAM_H
#pragma once

namespace view {
	struct RequestPackage;
	class MainView;
	class ExitView;
}  // namespace view

class Program final : public QObject {
	Q_OBJECT

 public:
	explicit Program();
	~Program() override;
	MAKE_NON_COPYABLE_NON_MOVEABLE(Program)

	void Run();
 public slots:
	void OnReceiveRequest(const view::RequestPackage& request);

 private:
	enum Command : UnsignedInteger { OPEN_MAIN_VIEW, OPEN_EXIT_VIEW, QUIT };
	Command command_{OPEN_MAIN_VIEW};
	bool should_run_ = true;

	void OnOpenMainView();
	void OnOpenExitView();
	void OnQuit();

	void Initialize();
};

#endif  // !PROGRAM_H
