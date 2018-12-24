/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "program.h"
#include "console.h"
#include "view.h"
using view::ExitView;
using view::MainView;
using view::RequestPackage;
using view::Type;
using view::ViewBase;

Program::Program() : QObject{nullptr} {}

Program::~Program() = default;

void Program::Run() {
	Initialize();
	while (should_run_) {
		switch (command_) {
			case OPEN_MAIN_VIEW:
				OnOpenMainView();
				break;
			case OPEN_EXIT_VIEW:
				OnOpenExitView();
				break;
			case QUIT:
				OnQuit();
				break;
			default:
				// do nothing
				break;
		}
	}
}

void Program::OnReceiveRequest(const RequestPackage& request) {
	switch (request.type) {
		case Type::MAIN_VIEW:
			if (request.message.has_value()) {
				switch (std::any_cast<UnsignedInteger>(request.message)) {
					case MainView::EXIT:
						command_ = OPEN_EXIT_VIEW;
						break;
					case MainView::ADD_USER:
						break;
					case MainView::EDIT_USER:
						break;
					case MainView::DELETE_USER:
						break;
					case MainView::SEARCH_INFO:
						break;
					case MainView::SUMMARY_PAID_BILL:
						break;
					case MainView::SUMMARY_NON_PAID_BILL:
						break;
					case MainView::PRINT_BILL:
						break;
					default:
						// do nothing
						break;
				}
			}
			break;

		case Type::EXIT_VIEW:
			command_ = QUIT;
			break;
		case Type::VIEW_BASE:
			break;
		default:
			// do nothing
			break;
	}
}

void Program::OnOpenMainView() {
	// main_ = make_unique<MainView>(this);

	// main_->Run();
	MainView(this).Run();
}

void Program::OnOpenExitView() {
	// exit_ = make_unique<ExitView>(this);

	// exit_->Run();
	ExitView(this).Run();
}

void Program::OnQuit() {
	should_run_ = false;
}

void Program::Initialize() {
	should_run_ = true;
	command_ = OPEN_MAIN_VIEW;
	console::SetupConsole();
}
