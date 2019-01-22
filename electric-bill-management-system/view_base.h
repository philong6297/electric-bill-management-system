#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#pragma once
namespace view {
	class ViewBase : public QObject {
		Q_OBJECT

	 public:
		explicit ViewBase();
		~ViewBase() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(ViewBase)

		virtual void Run() = 0;
	};
}  // namespace view

#endif  // !VIEW_BASE_H
