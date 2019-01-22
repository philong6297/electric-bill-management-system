#ifndef ADD_USER_VIEW_H
#define ADD_USER_VIEW_H
#pragma once
#include "view_base.h"
namespace view {
	// view de quan ly cac kich ban lien quan toi chuc nang them ng dung
	// view nay se nhan them mot database tu program de chay query
	class AddUserView final : public ViewBase {
		Q_OBJECT

	 public:
		explicit AddUserView(QSqlDatabase* database);
		~AddUserView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(AddUserView)

		// cac loai message se send toi program
		enum MessageId { OPEN_MAIN_VIEW, OPEN_EXIT_VIEW };

		void Run() override;
	 signals:
		// tin hieu gui toi program
		void SendMessage(MessageId message);

	 private:
		std::unique_ptr<QSqlQuery> query_;
	};

}  // namespace view
#endif  // !ADD_USER_VIEW_H
