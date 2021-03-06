/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "add_user_view.h"
#include <iostream>
#include <vector>
#include "console.h"
using view::AddUserView;
namespace {
	template <class T>
	T selection_process(const Utf8StringView content,
											const std::vector<T>& valid_value) {
		T value{};
		std::cin.clear();
		do {
			fmt::print(content);
			std::cin >> value;
		} while (std::none_of(valid_value.begin(),
													valid_value.end(),
													[value](const T valid) { return valid == value; }));

		return value;
	}
}  // namespace
AddUserView::AddUserView(QSqlDatabase* database)
		: query_{std::make_unique<QSqlQuery>(*database)} {}

AddUserView::~AddUserView() = default;

void AddUserView::Run() {
	using fmt::arg;
	using fmt::format;
	using fmt::print;
	using std::cin;
	using std::getline;
	console::SetupConsole();

	auto run_again = true;
	while (run_again) {
		cin.clear();
		console::ClearConsoleScreen();
		/*
		 * Nhập và lưu các thông tin cần thiết trước khi thêm ng dùng
		 */
		print(u8"THÊM NGƯỜI DÙNG\n\n");
		print(u8"NHẬP THÔNG TIN NGƯỜI DÙNG\n\n");
		Utf8String name, id, address, phone, birth_date, register_date;
		print(u8"\tHọ và Tên: ");
		name = console::ReadUtf8String();
		print(u8"\tSố CMND: ");
		id = console::ReadUtf8String();
		print(u8"\tĐịa Chỉ: ");
		address = console::ReadUtf8String();
		print(u8"\tSố Điện Thoại: ");
		phone = console::ReadUtf8String();
		print(u8"\tNgày Sinh (dd/mm/yyyy): ");
		birth_date = console::ReadUtf8String();
		print(u8"\tNgày Đăng Ký (dd/mm/yyyy): ");
		register_date = console::ReadUtf8String();
		print("\n\n");

		/*
		 *Xác nhận để nhập thông tin vào csdl hay không
		 */
		const auto confirm_key = selection_process<char>(
				u8"NHẬP THÀNH CÔNG, NHẬP Y/N ĐỂ XÁC NHẬN SỬA THÔNG TIN: ",
				{'Y', 'y', 'N', 'n'});
		// chọn thêm thông tin vào CSDL
		if (std::toupper(confirm_key) == 'Y') {
			// chuẩn bị query và gán các giá trị vào query
			query_->prepare(
					"insert into "
					"CLIENT"
					"(full_name, id_card, address, phone_number, birth_date, "
					"register_date)"
					"values"
					"(:full_name, :id_card, :address, :phone_number, :birth_date, "
					":register_date)");
			query_->bindValue(":full_name", name.data());
			query_->bindValue(":id_card", id.data());
			query_->bindValue(":address", address.data());
			query_->bindValue(":phone_number", phone.data());
			query_->bindValue(":birth_date", birth_date.data());
			query_->bindValue(":register_date", register_date.data());
			// nếu query thành công
			if (query_->exec()) {
				print(u8"THÊM THÔNG TIN THÀNH CÔNG\n\n");
			}
			// nếu query fail thì in ra lỗi
			else {
				print(u8"LỖI TRONG QUÁ TRÌNH ĐƯA THÔNG TIN NGƯỜI DÙNG VÀO CSDL\n");
				print(u8"THÔNG TIN LỖI : ");
				qInfo() << query_->lastError();
			}
		}
		// chọn hủy thêm thông tin
		else {
			print(u8"HỦY THÊM THÔNG TIN THÀNH CÔNG\n");
		}

		/*
		 * chọn 1/2/3 để thực hiện các xử lý tiếp theo
		 */
		const auto select_key = selection_process<int>(
				u8"NHẬP 1/2/3 ĐỂ TRỞ LẠI MENU CHÍNH/ TIẾP TỤC THÊM NGƯỜI DÙNG/ "
				u8"THOÁT CHƯƠNG TRÌNH: ",
				{1, 2, 3});
		switch (select_key) {
			case 1:
				// gọi Program mở main menu
				emit SendMessage(OPEN_MAIN_VIEW);
				run_again = false;
				break;
			case 2:
				// thực hiện tiếp thêm người dùng
				run_again = true;
				break;
			case 3:
				// gọi Program mở Exit view
				emit SendMessage(OPEN_EXIT_VIEW);
				run_again = false;
				break;
			default:
				// do nothing
				break;
		}
	}
}
