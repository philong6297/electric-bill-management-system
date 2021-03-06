/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "search_view.h"
#include "console.h"
using database::Client;
using database::confirm_process;
using database::execute_query;
using database::selection_process;
using fmt::arg;
using fmt::format;
using fmt::print;
using std::cin;
using view::SearchView;
SearchView::SearchView(QSqlDatabase* database)
		: query_{std::make_unique<QSqlQuery>(*database)} {}

SearchView::~SearchView() {}

void SearchView::Run() {
	console::SetupConsole();
	PerformSearchScene();
	PerformEndScene();
}

void SearchView::PerformEndScene() {
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

void SearchView::PerformSearchScene() {
	cin.clear();
	console::ClearConsoleScreen();
	print("TÌM KIẾM THÔNG TIN\n\n");

	print("Nhập từ khóa: ");
	const auto keyword = console::ReadUtf8String();

	// chuẩn bị query
	query_->finish();
	query_->prepare(
			format("select client_id, full_name, id_card, address, phone_number, "
						 "birth_date, register_date "
						 "from Client "
						 "where (client_id like '%{keyword}%' or full_name like "
						 "'%{keyword}%' or "
						 "id_card like '%{keyword}%' or address like '%{keyword}%' or "
						 "phone_number like "
						 "'%{keyword}%' or birth_date like '%{keyword}%' or register_date "
						 "like '%{keyword}%')"
						 "order by client_id",
						 arg("keyword", keyword))
					.data());
	execute_query(query_.get(),
								std::bind(&SearchView::OnQuerySearchKeywordSuccess, this),
								[this] {
									print("CÓ LỖI TRONG QUÁ TRÌNH TÌM KIẾM TRÊN CSDL\n");
									print("THÔNG TIN LỖI: ");
									qInfo() << query_->lastError();
								});
}

void SearchView::OnQuerySearchKeywordSuccess() const {
	// tìm thấy kết quả
	if (query_->isSelect() && query_->first()) {
		print("Kết quả: \n");
		auto index = 1;
		do {
			print(
					"{index}.\n"
					"\tMã Hộ: {client_id}\n"
					"\tHọ và Tên: {name}\n"
					"\tSố CMND: {id}\n"
					"\tĐịa Chỉ: {address}\n"
					"\tSố Điện Thoại: {phone}\n"
					"\tNgày Sinh: {birth}\n"
					"\tNgày Đăng Ký: {register}\n\n",
					arg("index", index),
					arg("client_id", query_->value(0).toString().toStdString()),
					arg("name", query_->value(1).toString().toStdString()),
					arg("id", query_->value(2).toString().toStdString()),
					arg("address", query_->value(3).toString().toStdString()),
					arg("phone", query_->value(4).toString().toStdString()),
					arg("birth", query_->value(5).toString().toStdString()),
					arg("register", query_->value(6).toString().toStdString()));
			++index;
		} while (query_->next());
	}
	// query thành công nhưng không có kết quả
	else {
		print("KHÔNG TÌM THẤY KẾT QUẢ THỎA MÃN TRÊN CSDL\n\n");
	}
}

void SearchView::PrintResultTitle() const {}
