#ifndef PROGRAM_H
#define PROGRAM_H
#pragma once
#include "view.h"

namespace view {
	class ViewBase;
}
class Program final : public QObject {
	Q_OBJECT

 public:
	explicit Program();
	~Program() override;
	MAKE_NON_COPYABLE_NON_MOVEABLE(Program)
	void Start();

 private:
	// view hien tai, moi khi ma co view can chuyen view khac thi
	// current_view = new View...
	// current_view -> run de sang view khac
	// tat cac cac view de ke thua tu ViewBase
	// current_view_ = new ViewCon
	std::unique_ptr<view::ViewBase> current_view_;
	// database se khoi dong khi program khoi dong

	std::unique_ptr<QSqlDatabase> database_;

	// chay chuong trinh
	void Run() const;

	// khoi tao cac cai can thiet khi run
	void Initialize();

	void CreateMainView();
	void CreateExitView();
	void CreateAddUserView();
	void CreateEditUserView();
	void CreateDeleteUserView();
	void CreateSearchView();
	void CreatePrintBillView();

	void CreateSummaryView(bool paid);
};

#endif  // !PROGRAM_H
