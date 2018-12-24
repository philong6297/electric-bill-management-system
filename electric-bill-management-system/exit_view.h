#ifndef EXIT_VIEW_H
#define EXIT_VIEW_H
#pragma once

#include "view_base.h"
namespace view {
	class ExitView final : public ViewBase {
		Q_OBJECT

	 public:
		using ViewBase::ViewBase;
		~ExitView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(ExitView)

		void Run() override;
		enum RequestMessage : SignedInteger { QUIT };

	 private:
		void Render() const override;
	};

}  // namespace view

#endif  // !EXIT_VIEW_H
