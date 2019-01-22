#ifndef DATABASE_INL
#define DATABASE_INL

#ifndef PCH_H
#	error "This file is an extension of pch.h, cannot be used alone"
#else
#	pragma once
#	include <iostream>
#	include <vector>
namespace database {
	inline namespace type {
		using Client = struct {
			UnsignedInteger client_id;
			Utf8String full_name;
			Utf8String id_card;
			Utf8String address;
			Utf8String phone_number;
			Utf8String birth_date;
			Utf8String register_date;
		};
	}  // namespace type

	inline char selection_process(const Utf8StringView content,
																const std::vector<char>& valid_value) {
		char value;

		do {
			std::cin.clear();
			fmt::print(content);
			std::cin >> value;
		} while (std::none_of(
				valid_value.begin(), valid_value.end(), [value](const char valid) {
					return valid == value;
				}));

		return value;
	}

	inline void execute_query(QSqlQuery* query,
														const Callback& on_query_success,
														const Callback& on_query_fail) {
		if (query->exec()) {
			on_query_success();
		}
		else {
			on_query_fail();
		}
	}

	inline void confirm_process(const Utf8StringView confirm_title,
															const Callback& on_yes,
															const Callback& on_no) {
		if (const auto confirm =
						selection_process(confirm_title, {'Y', 'y', 'n', 'N'});
				std::toupper(confirm) == 'Y') {
			on_yes();
		}
		else {
			on_no();
		}
	}

}  // namespace database
#endif  // !PCH_H
#endif  // !DATABASE_INL
