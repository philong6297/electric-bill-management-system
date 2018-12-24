#ifndef MACROS_H
#define MACROS_H
#ifndef PCH_H
#	error "This file is an extension of pch.h, cannot be used alone"
#else
#	pragma once
#	define MAKE_NON_COPYABLE(T)  \
		T(const T& other) = delete; \
		T& operator=(const T& other) = delete;
#	define MAKE_NON_MOVEABLE(T) \
		T(T&& other) = delete;     \
		T& operator=(T&& other) = delete;
#	define MAKE_NON_COPYABLE_NON_MOVEABLE(T) \
		MAKE_NON_COPYABLE(T)                    \
		MAKE_NON_MOVEABLE(T)
#endif  // !PCH_H

#endif  // !MACROS_H
