/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "program.h"
#include <QtCore/QCoreApplication>
#include "view.h"
using std::make_unique;
using std::unique_ptr;
using view::ExitView;
using view::MainView;
Program::Program()
		: QObject{nullptr},
			database_{
					make_unique<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE"))} {
	// kết nối database
	database_->setDatabaseName("database/electric-bill.db");
}

Program::~Program() {
	database_->close();
};

void Program::Run() const {
	current_view_->Run();
}

void Program::Start() {
	Initialize();
	if (database_->open()) {
		Run();
	}
	else {
		fmt::print(u8"KHÔNG THỂ KẾT NỐI TỚI CSDL");
	}
}

void Program::Initialize() {
	CreateMainView();
}

void Program::CreateMainView() {
	current_view_ = make_unique<MainView>();
	// kết nối message gửi từ main view sang cho program
	connect(qobject_cast<const MainView*>(current_view_.get()),
					&MainView::MenuItemSelected,
					[this](const UnsignedInteger id) {
						switch (id) {
							case MainView::ADD_USER:
								CreateAddUserView();
								Run();
								break;
							case MainView::EDIT_USER:
								CreateEditUserView();
								Run();
								break;
							case MainView::DELETE_USER:
								CreateDeleteUserView();
								Run();
								break;
							case MainView::SEARCH_INFO:
								CreateSearchView();
								Run();
								break;
							case MainView::SUMMARY_PAID_BILL:
								CreateSummaryView(true);
								Run();
								break;
							case MainView::SUMMARY_NON_PAID_BILL:
								CreateSummaryView(false);
								Run();
								break;
							case MainView::PRINT_BILL:
								CreatePrintBillView();
								Run();
								break;
							case MainView::EXIT:
								CreateExitView();
								Run();
								break;
							default:
								// do nothing
								break;
						}
					});
}

void Program::CreateExitView() {
	current_view_ = make_unique<ExitView>();
	// kết nối
	connect(qobject_cast<const ExitView*>(current_view_.get()),
					&ExitView::Exit,
					[]() { qApp->quit(); });
}

void Program::CreateAddUserView() {
	using view::AddUserView;
	current_view_ = make_unique<AddUserView>(database_.get());
	connect(qobject_cast<const AddUserView*>(current_view_.get()),
					&AddUserView::SendMessage,
					[this](const AddUserView::MessageId message) {
						switch (message) {
							case AddUserView::OPEN_MAIN_VIEW:
								CreateMainView();
								Run();
								break;
							case AddUserView::OPEN_EXIT_VIEW:
								CreateExitView();
								Run();
								break;
							default:
								// do nothing
								break;
						}
					});
}

void Program::CreateEditUserView() {
	using view::EditUserView;
	current_view_ = make_unique<EditUserView>(database_.get());
	connect(qobject_cast<const EditUserView*>(current_view_.get()),
					&EditUserView::SendMessage,
					[this](const EditUserView::MessageId message) {
						switch (message) {
							case EditUserView::OPEN_MAIN_VIEW:
								CreateMainView();
								Run();
								break;
							case EditUserView::OPEN_EXIT_VIEW:
								CreateExitView();
								Run();
								break;
							default:
								// do nothing
								break;
						}
					});
}

void Program::CreateDeleteUserView() {
	using view::DeleteUserView;
	current_view_ = make_unique<DeleteUserView>(database_.get());
	connect(qobject_cast<const DeleteUserView*>(current_view_.get()),
					&DeleteUserView::SendMessage,
					[this](const DeleteUserView::MessageId message) {
						switch (message) {
							case DeleteUserView::OPEN_MAIN_VIEW:
								CreateMainView();
								Run();
								break;
							case DeleteUserView::OPEN_EXIT_VIEW:
								CreateExitView();
								Run();
								break;
							default:
								// do nothing
								break;
						}
					});
}

void Program::CreateSearchView() {
	using view::SearchView;
	current_view_ = make_unique<SearchView>(database_.get());
	connect(qobject_cast<const SearchView*>(current_view_.get()),
					&SearchView::SendMessage,
					[this](const SearchView::MessageId message) {
						switch (message) {
							case SearchView::OPEN_MAIN_VIEW:
								CreateMainView();
								Run();
								break;
							case SearchView::OPEN_EXIT_VIEW:
								CreateExitView();
								Run();
								break;
							default:
								// do nothing
								break;
						}
					});
}

void Program::CreatePrintBillView() {
	using view::PrintBillView;
	current_view_ = make_unique<PrintBillView>(database_.get());
	connect(qobject_cast<const PrintBillView*>(current_view_.get()),
					&PrintBillView::SendMessage,
					[this](const PrintBillView::MessageId message) {
						switch (message) {
							case PrintBillView::OPEN_MAIN_VIEW:
								CreateMainView();
								Run();
								break;
							case PrintBillView::OPEN_EXIT_VIEW:
								CreateExitView();
								Run();
								break;
							default:
								// do nothing
								break;
						}
					});
}

void Program::CreateSummaryView(const bool paid) {
	using view::SummaryView;
	current_view_ = make_unique<SummaryView>(
			paid ? SummaryView::PAID : SummaryView::NON_PAID, database_.get());
	connect(qobject_cast<const SummaryView*>(current_view_.get()),
					&SummaryView::SendMessage,
					[this](const SummaryView::MessageId message) {
						switch (message) {
							case SummaryView::OPEN_MAIN_VIEW:
								CreateMainView();
								Run();
								break;
							case SummaryView::OPEN_EXIT_VIEW:
								CreateExitView();
								Run();
								break;
							default:
								// do nothing
								break;
						}
					});
}
