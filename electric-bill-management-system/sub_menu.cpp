/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "sub_menu.h"
#include "menu.h"

using menu::MenuBase;
using menu::MenuItem;
using menu::MenuStyle;
using menu::SubMenu;
using std::make_unique;

SubMenu::SubMenu(const Utf8StringView title, SubMenu* parent)
		: MenuBase(title, parent), is_expand_{false} {}

Utf8String SubMenu::GetCurrentContent() const {
	auto result = MenuBase::GetCurrentContent();
	if (is_expand_) {
		for (const auto& child_object : children()) {
			if (const auto* try_sub_menu = qobject_cast<SubMenu*>(child_object);
					try_sub_menu != nullptr) {
				result += try_sub_menu->GetCurrentContent();
			}
			else if (const auto* try_menu_item =
									 qobject_cast<MenuItem*>(child_object);
							 try_menu_item != nullptr) {
				result += try_menu_item->GetCurrentContent();
			}
		}
	}
	return result;
}

bool SubMenu::IsExpand() const {
	return is_expand_;
}

void SubMenu::SetExpand(const bool is_expand) {
	is_expand_ = is_expand;
}

MenuItem* SubMenu::AddMenuItem(const Utf8StringView title,
															 const UnsignedInteger command) {
	return make_unique<MenuItem>(title, command, this).release();
}

SubMenu* SubMenu::AddSubMenu(const Utf8StringView title) {
	return make_unique<SubMenu>(title, this).release();
}
