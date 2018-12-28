/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "menu_base.h"
#include "menu.h"
using menu::MenuBase;
using menu::SubMenu;
namespace {
	auto get_level_by_parent_level(const SubMenu* parent) {
		const auto result = parent != nullptr ? parent->GetLevel() + 1U : 0U;
		return result;
	}

	auto get_last_child_order_of(const SubMenu* parent) {
		std::optional<SignedInteger> result{};
		if (parent != nullptr) {
			result = parent->children().size() - 1;
		}
		return result;
	}
}  // namespace
MenuBase::MenuBase(const Utf8StringView title, SubMenu* parent)
		: QObject{parent},
			title_{title},
			level_{get_level_by_parent_level(parent)},
			child_order_{get_last_child_order_of(parent)} {}

MenuBase::~MenuBase() = default;

void MenuBase::SetTile(const Utf8StringView title) {
	title_ = title;
}

Utf8String MenuBase::GetTitle() const {
	return title_;
}

UnsignedInteger MenuBase::GetLevel() const {
	return level_;
}

void MenuBase::SetParent(SubMenu* parent) {
	setParent(parent);
}

std::optional<SignedInteger> MenuBase::GetChildOrder() const {
	return child_order_;
}

bool MenuBase::HasParent() const {
	return GetParent() != nullptr && child_order_.has_value();
}

SubMenu* MenuBase::GetParent() const {
	return qobject_cast<SubMenu*>(parent());
}

MenuBase* MenuBase::GetNextSibling() const {
	MenuBase* result = nullptr;
	if (HasParent()) {
		result = GetSiblingByOrder(child_order_.value() + 1U);
	}
	return result;
}

MenuBase* MenuBase::GetPreviousSibling() const {
	MenuBase* result = nullptr;
	if (HasParent()) {
		result = GetSiblingByOrder(child_order_.value() - 1U);
	}
	return result;
}

MenuBase* MenuBase::GetSiblingByOrder(const SignedInteger child_order) const {
	MenuBase* result = nullptr;
	if (HasParent()) {
		const auto& children_list = GetParent()->children();
		if (child_order >= 0 && child_order < children_list.size()) {
			result = qobject_cast<MenuBase*>(children_list.at(child_order));
		}
	}
	return result;
}

menu::MenuType MenuBase::GetType() const {
	return MENU_BASE;
}
