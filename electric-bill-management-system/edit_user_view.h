#ifndef EDIT_USER_VIEW_H
#define EDIT_USER_VIEW_H

#pragma once
#include "view_base.h"

namespace view {
	// chinh sua thong tin ng dung
	class EditUserView final : public ViewBase {
		Q_OBJECT

	 public:
		explicit EditUserView(QSqlDatabase* database);
		~EditUserView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(EditUserView)
		// cac loai message toi program
		enum MessageId { OPEN_MAIN_VIEW, OPEN_EXIT_VIEW };
		void Run() override;
	 signals:
		// gui signal toi program
		void SendMessage(MessageId message) const;

	 private:
		// kich ban ket thuc : 1/2/3
		void PerformEndScene();
		// kich ban tim kiem theo id
		void PerformSearchClientScene();
		// kich ban chinh sua theo id
		void PerformEditClientScene();

		void OnQueryFindClientByIdSuccess(UnsignedInteger client_id) const;
		void UpdateClientInfoToDatabase(const database::Client& client) const;

		std::unique_ptr<QSqlQuery> query_;
	};
}  // namespace view

#endif  // !EDIT_USER_VIEW_H
