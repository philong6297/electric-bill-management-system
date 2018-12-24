#ifndef VIEW_DEFINES_H
#define VIEW_DEFINES_H
#pragma once
#include <any>
namespace view {
	enum Type { VIEW_BASE, MAIN_VIEW, EXIT_VIEW };
	struct RequestPackage final {
		Type type;
		std::any message;
	};
}  // namespace view
#endif  // !VIEW_DEFINES_H
