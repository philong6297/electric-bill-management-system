#ifndef SUB_MENU_H
#define SUB_MENU_H

#pragma once
#include "menu_base.h"

namespace menu {
	class MenuItem;
	class SubMenu final : public MenuBase {
		Q_OBJECT
	 public:
		explicit SubMenu(Utf8StringView title, SubMenu* parent = nullptr);
		~SubMenu() override = default;
		MAKE_NON_COPYABLE_NON_MOVEABLE(SubMenu)

		// MenuBase overrides
		Utf8String GetCurrentContent() const override;

		bool IsExpand() const;
		void SetExpand(bool is_expand);
		MenuItem* AddMenuItem(Utf8StringView title, UnsignedInteger command);
		SubMenu* AddSubMenu(Utf8StringView title);

	 private:
		bool is_expand_;
	};

}  // namespace menu

#endif  // !SUB_MENU_H
