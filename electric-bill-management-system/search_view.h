#ifndef SEARCH_VIEW_H
#define SEARCH_VIEW_H
#pragma once
#include "view_base.h"
namespace view {
	// kich ban tim kiem theo tu khoa
	class SearchView final : public ViewBase {
		Q_OBJECT

	 public:
		explicit SearchView(QSqlDatabase *database);
		~SearchView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(SearchView)
		enum MessageId { OPEN_MAIN_VIEW, OPEN_EXIT_VIEW };
		void Run() override;
	 signals:
		void SendMessage(MessageId message) const;

	 private:
		std::unique_ptr<QSqlQuery> query_;

		void PerformEndScene();
		// kich ban search theo tu khoa
		void PerformSearchScene();
		// khi query thanh cong search theo tu khoa
		void OnQuerySearchKeywordSuccess() const;
		// in ra man hinh cac ket qua
		void PrintResultTitle() const;
	};
}  // namespace view

#endif  // !SEARCH_VIEW_H
