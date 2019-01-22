#ifndef EXIT_VIEW_H
#define EXIT_VIEW_H
#pragma once
#include "view_base.h"

namespace view {
	// kich ban thoat chuong trinh
	class ExitView final : public ViewBase {
		Q_OBJECT

	 public:
		explicit ExitView();
		~ExitView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(ExitView)

		void Run() override;
	 signals:
		// co moi signal exit gui toi program de program exit
		void Exit();

	 private:
		void Render();
	};

}  // namespace view
#endif  // !EXIT_VIEW_H
