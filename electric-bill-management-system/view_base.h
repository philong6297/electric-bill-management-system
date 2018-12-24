#ifndef VIEW_BASE_H
#define VIEW_BASE_H
#pragma once

class Program;
namespace view {
	struct RequestPackage;

	class ViewBase : public QObject {
		Q_OBJECT
	 public:
		explicit ViewBase(Program* parent);
		~ViewBase() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(ViewBase)

		virtual void Run() = 0;
	 signals:
		void Request(const RequestPackage& request) const;

	 protected:
		Program* program_;
		virtual void Render() const = 0;
	};
}  // namespace view

#endif  // !VIEW_BASE_H
