#ifndef PRINT_BILL_VIEW_H
#define PRINT_BILL_VIEW_H
#pragma once
#include "view_base.h"
namespace view {
	// kich ban in hoa don
	class PrintBillView final : public ViewBase {
		Q_OBJECT

	 public:
		explicit PrintBillView(QSqlDatabase* database);
		~PrintBillView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(PrintBillView)
		// cac loai message gui toi program
		enum MessageId { OPEN_MAIN_VIEW, OPEN_EXIT_VIEW };
		void Run() override;
	 signals:
		void SendMessage(MessageId message) const;

	 private:
		// kich ban ket thuc 1/2/3
		void PerformEndScene();
		// kich ban tim kiem hoa don theo ma hoa don
		void PerformSearchBillScene();
		// in ma hoa don theo ma
		void PerformPrintBillScene() const;

		void OnQueryFindBillByIdSuccess(UnsignedInteger bill_id) const;

		std::unique_ptr<QSqlQuery> query_;
	};
}  // namespace view

#endif  // !PRINT_BILL_VIEW_H
