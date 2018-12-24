#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

#ifndef PCH_H
#	error "This file is an extension of pch.h, cannot be used alone"
#else
#	pragma once
#	include <string>

inline namespace type {
	using Utf8StringView = std::string_view;
	using Utf8String = std::string;
	using UnsignedInteger = uint_fast32_t;
	using SignedInteger = int_fast32_t;
}  // namespace type
#endif  // !PCH_H

#endif  // !TYPE_DEFINE_H
