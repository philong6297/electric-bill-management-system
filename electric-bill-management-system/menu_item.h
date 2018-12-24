#ifndef MENU_ITEM_H
#define MENU_ITEM_H
#pragma once
#include "menu_base.h"

namespace menu {
	class MenuItem final : public MenuBase {
		Q_OBJECT
	 public:
		explicit MenuItem(Utf8StringView title,
											UnsignedInteger command,
											SubMenu* parent = nullptr);
		~MenuItem() override = default;
		MAKE_NON_COPYABLE_NON_MOVEABLE(MenuItem)

		// MenuBase overrides
		Utf8String GetCurrentContent() const override;

		UnsignedInteger GetCommand() const;

	 private:
		UnsignedInteger command_;
	};
}  // namespace menu
#endif  // !MENU_ITEM_H
