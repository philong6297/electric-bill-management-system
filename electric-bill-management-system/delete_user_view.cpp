/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "delete_user_view.h"
#include "console.h"

using database::Client;
using database::confirm_process;
using database::execute_query;
using database::selection_process;
using fmt::arg;
using fmt::format;
using fmt::print;
using std::cin;
using view::DeleteUserView;

DeleteUserView::DeleteUserView(QSqlDatabase* database)
		: query_{std::make_unique<QSqlQuery>(*database)} {}

DeleteUserView::~DeleteUserView() = default;

void DeleteUserView::Run() {
	console::SetupConsole();
	PerformSearchClientScene();
	PerformEndScene();
}

void DeleteUserView::PerformEndScene() {
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

void DeleteUserView::PerformSearchClientScene() {
	cin.clear();
	console::ClearConsoleScreen();
	print("XÓA NGƯỜI DÙNG\n\n");
	/*
	 * Nhập mã hộ muốn xóa thông tin
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
			std::bind(&DeleteUserView::OnQueryFindClientByIdSuccess, this, client_id),
			[this] {
				print("CÓ LỖI TRONG QUÁ TRÌNH TÌM NGƯỜI DÙNG TRÊN CSDL\n");
				print("THÔNG TIN LỖI: ");
				qInfo() << query_->lastError();
			});
}

void DeleteUserView::PerformDeleteClientScene() {
	const auto client_id = query_->value(0).toInt();
	// chuẩn bị query
	query_->finish();
	query_->prepare(
			format("delete from Client where client_id = {}", client_id).data());
	// thực hiện query
	execute_query(
			query_.get(),
			[] { print("XÓA THÀNH CÔNG...\n"); },
			[this] {
				print("CÓ LỖI TRONG QUÁ TRÌNH XÓA NGƯỜI DÙNG TRÊN CSDL\n");
				print("THÔNG TIN LỖI: ");
				qInfo() << query_->lastError();
			});
}

void DeleteUserView::OnQueryFindClientByIdSuccess(
		const UnsignedInteger client_id) const {
	// tìm thấy kết quả
	if (query_->isSelect() && query_->first()) {
		/*
		 * In ra thông tin trước khi xóa
		 */
		print("KIỂM TRA THÔNG TIN TRƯỚC KHI XÓA:\n\n");
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

		// tiến hành xác nhận xóa thông tin
		confirm_process("NHẬP Y/N ĐỂ XÁC NHẬN BẮT ĐẦU XÓA NGƯỜI DÙNG: ",
										std::bind(&DeleteUserView::PerformDeleteClientScene,
															const_cast<DeleteUserView*>(this)),
										[] { print("HỦY XÓA THÔNG TIN\n"); });
	}
	// query thành công nhưng không có kết quả
	else {
		print("KHÔNG TÌM THẤY HỘ CÓ MÃ {} THỎA MÃN TRÊN CSDL\n\n", client_id);
	}
}
