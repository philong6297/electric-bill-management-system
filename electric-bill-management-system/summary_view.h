#ifndef SUMMARY_VIEW_H
#define SUMMARY_VIEW_H
#pragma once
#include "view_base.h"

namespace view {
	// kich ban thong ke
	class SummaryView final : public ViewBase {
		Q_OBJECT

	 public:
		// cac loai view
		enum SummaryType { NON_PAID, PAID };
		explicit SummaryView(SummaryType type, QSqlDatabase* database);
		~SummaryView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(SummaryView)

		// cac message gui toi program
		enum MessageId { OPEN_MAIN_VIEW, OPEN_EXIT_VIEW };
		void Run() override;
	 signals:
		void SendMessage(MessageId message) const;

	 private:
		void PerformEndScene();
		// kich ban thong ke se in ra loai view nao PAID or NON_PAID
		void PerformSummaryScene();
		void OnQuerySummarySuccess() const;
		std::unique_ptr<QSqlQuery> query_;
		const SummaryType type_;
	};
}  // namespace view

#endif  // !SUMMARY_VIEW_H
