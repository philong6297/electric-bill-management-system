#ifndef ADD_USER_VIEW_H
#define ADD_USER_VIEW_H
#pragma once

#include "view_base.h"
namespace view {
	class AddUserView final : public ViewBase {
		Q_OBJECT

	 public:
		using ViewBase::ViewBase;
		~AddUserView() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(AddUserView)

		void Run() override;

	 private:
		void Render() const override;
	};
}  // namespace view

#endif  // !ADD_USER_VIEW_H
