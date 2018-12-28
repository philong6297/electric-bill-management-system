#ifndef MENU_BASE_H
#define MENU_BASE_H
#pragma once
#include "menu_defines.h"

namespace menu {
	class SubMenu;
	class MenuBase : public QObject {
		Q_OBJECT
	 public:
		explicit MenuBase(Utf8StringView title, SubMenu* parent = nullptr);
		~MenuBase() override;
		MAKE_NON_COPYABLE_NON_MOVEABLE(MenuBase)

		void SetTile(Utf8StringView title);
		Utf8String GetTitle() const;
		UnsignedInteger GetLevel() const;
		void SetParent(SubMenu* parent);
		std::optional<SignedInteger> GetChildOrder() const;
		bool HasParent() const;
		SubMenu* GetParent() const;
		MenuBase* GetNextSibling() const;
		MenuBase* GetPreviousSibling() const;
		MenuBase* GetSiblingByOrder(SignedInteger child_order) const;
		virtual MenuType GetType() const;

	 private:
		using QObject::parent;
		using QObject::setParent;
		Utf8String title_;
		UnsignedInteger level_;
		std::optional<SignedInteger> child_order_;
	};
}  // namespace menu

#endif  // !MENU_BASE_H
