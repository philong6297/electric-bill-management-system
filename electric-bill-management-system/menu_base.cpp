/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "menu_base.h"
#include "menu.h"
using menu::get_style;
using menu::MenuBase;
using menu::reset_style;
using menu::SubMenu;
namespace {
	auto child_level_of(const SubMenu* parent) -> UnsignedInteger {
		UnsignedInteger result = 0;
		if (parent != nullptr) {
			result = parent->GetLevel() + 1;
		}
		return result;
	}

	auto last_child_index_of(const SubMenu* parent)
			-> std::optional<UnsignedInteger> {
		std::optional<UnsignedInteger> result{};
		if (parent != nullptr && !parent->children().isEmpty()) {
			result = parent->children().size();
		}
		return result;
	}
}  // namespace

// MenuBase -------------------------------------------------------------------
MenuBase::MenuBase(const Utf8StringView title, SubMenu* parent)
		: QObject(parent),
			title_{title.data()},
			level_{child_level_of(parent)},
			id_{last_child_index_of(parent)},
			is_focus_{false} {}

UnsignedInteger MenuBase::GetLevel() const {
	return level_;
}

Utf8String MenuBase::GetTitle() const {
	return title_;
}

void MenuBase::SetTitle(const Utf8StringView title) {
	title_ = title;
}

std::optional<SignedInteger> MenuBase::GetId() const {
	return id_;
}

bool MenuBase::HasParent() const {
	return id_.has_value();
}

void MenuBase::SetParent(SubMenu* new_parent) {
	setParent(new_parent);
	on_parent_changed(new_parent);
}

bool MenuBase::IsFocus() const {
	return is_focus_;
}

void MenuBase::SetFocus(const bool is_focus) {
	is_focus_ = is_focus;
}

MenuBase* MenuBase::NextSibling() const {
	MenuBase* result = nullptr;
	if (HasParent()) {
		result = SiblingAt(id_.value() + 1);
	}
	return result;
}

MenuBase* MenuBase::PreviousSibling() const {
	MenuBase* result = nullptr;
	if (HasParent()) {
		result = SiblingAt(id_.value() - 1);
	}
	return result;
}

MenuBase* MenuBase::SiblingAt(const SignedInteger index) const {
	MenuBase* result = nullptr;
	// have a parent
	if (HasParent() && index > 0 && index <= parent()->children().size()) {
		result = qobject_cast<MenuBase*>(parent()->children().at(index - 1));
	}
	return result;
}

Utf8String MenuBase::GetCurrentContent() const {
	using fmt::arg;
	using fmt::format;
	using fmt::to_string;

	const auto index =
			Utf8String{id_.has_value() ? to_string(id_.value()) + "." : ""};
	const auto align = format(u8"{content:\t>{level}}",
														arg(u8"content", u8""),
														arg(u8"level", GetLevel()));
	const auto title = GetTitle();
	const auto style = get_style(*this);

	return format(u8"{style}{align}{index}{title}{reset_style}\n",
								arg(u8"index", index),
								arg(u8"align", align),
								arg(u8"title", title),
								arg(u8"style", style),
								arg(u8"reset_style", reset_style()));
}

void MenuBase::on_parent_changed(const SubMenu* new_parent) {
	level_ = child_level_of(new_parent);
	id_ = last_child_index_of(new_parent);
}

void MenuBase::Show() const {
	fmt::print(u8"{}", *this);
}
