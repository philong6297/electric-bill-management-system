#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#pragma once
class Program;
class ViewBase : public QObject {
	Q_OBJECT

 public:
	explicit ViewBase();
	~ViewBase() override;
	MAKE_NON_COPYABLE_NON_MOVEABLE(ViewBase)

	virtual void Run() = 0;
};

#endif  // !VIEW_BASE_H
