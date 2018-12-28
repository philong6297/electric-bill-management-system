#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once
struct Message final {
	UnsignedInteger id{};
	std::optional<std::any> argument;
};

#endif  // !MESSAGE_H
