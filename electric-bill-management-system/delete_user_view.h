#ifndef DELETE_USER_VIEW_H
#define DELETE_USER_VIEW_H
#pragma once
#include "view_base.h"
namespace view {
	// quan ly cac kich ban lien quan toi xoa ng dung
	class DeleteUserView final : public ViewBase {
		Q_OBJECT

	 public:
		explicit DeleteUserView(QSqlDatabase* database);
		~DeleteUserView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(DeleteUserView)
		// cac message
		enum MessageId { OPEN_MAIN_VIEW, OPEN_EXIT_VIEW };
		void Run() override;
	 signals:
		void SendMessage(MessageId message) const;

	 private:
		// kich ban ket thuc: 1/2/3
		void PerformEndScene();
		// kich ban tim kiem ng dung theo id
		void PerformSearchClientScene();
		// kich ban xoa nguoi dung khi da co id
		void PerformDeleteClientScene();

		void OnQueryFindClientByIdSuccess(UnsignedInteger client_id) const;

		std::unique_ptr<QSqlQuery> query_;
	};
}  // namespace view

#endif  // !DELETE_USER_VIEW_H
