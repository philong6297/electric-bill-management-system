/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "edit_user_view.h"
#include <iostream>
#include "console.h"
using database::Client;
using database::confirm_process;
using database::execute_query;
using database::selection_process;
using fmt::arg;
using fmt::format;
using fmt::print;
using std::cin;
using std::getline;
using view::EditUserView;
namespace {}  // namespace
EditUserView::EditUserView(QSqlDatabase* database)
		: query_{std::make_unique<QSqlQuery>(*database)} {}

EditUserView::~EditUserView() = default;

void EditUserView::Run() {
	console::SetupConsole();
	PerformSearchClientScene();
	PerformEndScene();
}

void EditUserView::PerformEndScene() {
	/*
	 * chọn 1/2/3 để thực hiện các xử
	 * lý tiếp theo
	 */
	const auto select_key = selection_process(
			"NHẬP 1/2/3 ĐỂ TRỞ LẠI MENU CHÍNH/ TIẾP TỤC XÓA NGƯỜI DÙNG/ "
			"THOÁT CHƯƠNG TRÌNH: ",
			{'1', '2', '3'});
	switch (select_key) {
		case '1':
			// gọi Program mở main menu
			emit SendMessage(OPEN_MAIN_VIEW);
			break;
		case '2':
			// thực hiện tiếp thêm người dùng
			Run();
			break;
		case '3':
			// gọi Program mở Exit view
			emit SendMessage(OPEN_EXIT_VIEW);
			break;
		default:
			// do nothing
			break;
	}
}

void EditUserView::PerformSearchClientScene() {
	cin.clear();
	console::ClearConsoleScreen();
	print("SỬA THÔNG TIN NGƯỜI DÙNG\n\n");
	/*
	 * Nhập mã hộ muốn sửa thông tin
	 */
	print("NHẬP MÃ HỘ: ");
	UnsignedInteger client_id{};
	cin >> client_id;
	print("NHẬP THÀNH CÔNG, THỰC HIỆN TÌM KIẾM...\n");

	// chuẩn bị query để tìm kiếm trên csdl
	query_->finish();
	query_->prepare(
			format("select client_id, full_name, id_card, address, phone_number, "
						 "birth_date, register_date "
						 "from Client "
						 "where client_id = {}",
						 client_id)
					.data());

	// chạy query
	execute_query(
			query_.get(),
			std::bind(&EditUserView::OnQueryFindClientByIdSuccess, this, client_id),
			[this] {
				print("CÓ LỖI TRONG QUÁ TRÌNH TÌM NGƯỜI DÙNG TRÊN CSDL\n");
				print("THÔNG TIN LỖI: ");
				qInfo() << query_->lastError();
			});
}

void EditUserView::OnQueryFindClientByIdSuccess(
		const UnsignedInteger client_id) const {
	// tìm thấy kết quả
	if (query_->isSelect() && query_->first()) {
		/*
		 * In ra thông tin trước khi sửa
		 */
		print("KIỂM TRA THÔNG TIN TRƯỚC KHI SỬA:\n\n");
		print(
				"\tHọ và Tên: {name}\n"
				"\tSố CMND: {id}\n"
				"\tĐịa Chỉ: {address}\n"
				"\tSố Điện Thoại: {phone}\n"
				"\tNgày Sinh: {birth}\n"
				"\tNgày Đăng Ký: {register}\n\n",
				arg("name", query_->value(1).toString().toStdString()),
				arg("id", query_->value(2).toString().toStdString()),
				arg("address", query_->value(3).toString().toStdString()),
				arg("phone", query_->value(4).toString().toStdString()),
				arg("birth", query_->value(5).toString().toStdString()),
				arg("register", query_->value(6).toString().toStdString()));

		// tiến hành xác nhận sửa thông tin
		confirm_process("NHẬP Y/N ĐỂ XÁC NHẬN BẮT ĐẦU SỬA THÔNG TIN: ",
										std::bind(&EditUserView::PerformEditClientScene,
															const_cast<EditUserView*>(this)),
										[] { print("HỦY SỬA THÔNG TIN\n"); });
	}
	// query thành công nhưng không có kết quả
	else {
		print("KHÔNG TÌM THẤY HỘ CÓ MÃ {} THỎA MÃN TRÊN CSDL\n\n", client_id);
	}
}

void EditUserView::PerformEditClientScene() {
	/*
	 * Sửa thông tin người dùng
	 */
	print("SỬA LẠI THÔNG TIN NGƯỜI DÙNG\n\n");
	Client client;
	client.client_id = query_->value(0).toULongLong();
	print("\tHọ và Tên: ");
	client.full_name = console::ReadUtf8String();
	print("\tSố CMND: ");
	client.id_card = console::ReadUtf8String();
	print("\tĐịa Chỉ: ");
	client.address = console::ReadUtf8String();
	print("\tSố Điện Thoại: ");
	client.phone_number = console::ReadUtf8String();
	print("\tNgày Sinh (dd/mm/yyyy): ");
	client.birth_date = console::ReadUtf8String();
	print("\tNgày Đăng Ký (dd/mm/yyyy): ");
	client.register_date = console::ReadUtf8String();
	print("\n\n");

	/*
	 *Xác nhận để nhập thông tin vào csdl hay không
	 */
	confirm_process(
			"NHẬP THÀNH CÔNG, NHẬP Y/N ĐỂ XÁC NHẬN SỬA THÔNG TIN: ",
			std::bind(&EditUserView::UpdateClientInfoToDatabase, this, client),
			[] { print("HỦY THÊM THÔNG TIN THÀNH CÔNG\n"); });
}

void EditUserView::UpdateClientInfoToDatabase(const Client& client) const {
	// chuẩn bị query và gán các giá trị vào query
	query_->finish();
	query_->prepare(format("update Client "
												 "set full_name = '{name}',"
												 "id_card = '{id_card}',"
												 "address = '{address}',"
												 "phone_number = '{phone}',"
												 "birth_date = '{birth}',"
												 "register_date = '{register}' "
												 "where client_id = {client_id}",
												 arg("name", client.full_name),
												 arg("id_card", client.id_card),
												 arg("address", client.address),
												 arg("phone", client.phone_number),
												 arg("birth", client.birth_date),
												 arg("register", client.register_date),
												 arg("client_id", client.client_id))
											.data());

	execute_query(
			query_.get(),
			[] { print("THÊM THÔNG TIN THÀNH CÔNG\n\n"); },
			[this] {
				print("LỖI TRONG QUÁ TRÌNH ĐƯA THÔNG TIN NGƯỜI DÙNG VÀO CSDL\n");
				print("THÔNG TIN LỖI : ");
				print("{}", query_->lastQuery().toStdString());
				qInfo() << query_->lastError();
			});
}
