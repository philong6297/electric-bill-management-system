/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "summary_view.h"
#include "console.h"
using database::confirm_process;
using database::execute_query;
using database::selection_process;
using fmt::arg;
using fmt::format;
using fmt::print;
using std::cin;

view::SummaryView::SummaryView(const SummaryType type, QSqlDatabase* database)
		: query_{std::make_unique<QSqlQuery>(*database)}, type_{type} {}

view::SummaryView::~SummaryView() = default;

void view::SummaryView::Run() {
	console::SetupConsole();
	PerformSummaryScene();
	PerformEndScene();
}

void view::SummaryView::PerformEndScene() {
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

void view::SummaryView::PerformSummaryScene() {
	console::ClearConsoleScreen();
	if (type_ == PAID) {
		print("THỐNG KÊ HỘ ĐÃ THANH TOÁN\n\n");
	}
	else {
		print("THỐNG KÊ HỘ CHƯA THANH TOÁN");
	}
	query_->finish();
	query_->prepare(
			format("select Client.client_id, Client.full_name, Client.id_card,"
						 "Bill.bill_id,"
						 "Time.month_date "
						 "from Client, Bill, Time, Statistics "
						 "where ("
						 "Statistics.status = {type} "
						 "and Statistics.bill_id = Bill.bill_id "
						 "and Statistics.client_id = Client.client_id "
						 "and Statistics.month_id = Time.month_id "
						 ")",
						 arg("type", type_ == PAID ? 1 : 0))
					.data());
	execute_query(query_.get(),
								std::bind(&SummaryView::OnQuerySummarySuccess, this),
								[this] {
									print("CÓ LỖI TRONG QUÁ TRÌNH TÌM THỐNG KÊ TRÊN CSDL\n");
									print("THÔNG TIN LỖI: ");
									qInfo() << query_->lastQuery();
									qInfo() << query_->lastError();
								});
}

void view::SummaryView::OnQuerySummarySuccess() const {
	// tìm thấy kết quả
	if (query_->isSelect() && query_->first()) {
		/*
		 * In ra thông tin
		 */
		auto index = 1;
		do {
			const auto client_id =
					query_->value(0).toString().trimmed().toStdString();
			const auto bill_id = query_->value(3).toString().trimmed().toStdString();
			const auto full_name =
					query_->value(1).toString().trimmed().toStdString();
			const auto id_card = query_->value(2).toString().trimmed().toStdString();
			const auto month_date =
					query_->value(4).toString().trimmed().toStdString();
			print(
					format("{index}.\n"
								 "\tMã Hộ: {client_id}\n"
								 "\tMã Hóa Đơn: {bill_id}\n"
								 "\tHọ Và Tên: {full_name}\n"
								 "\tSố CMND: {id_card}\n"
								 "\tTháng: {month_date}\n",
								 arg("index", index),
								 arg("client_id", client_id),
								 arg("bill_id", bill_id),
								 arg("full_name", full_name),
								 arg("id_card", id_card),
								 arg("month_date", month_date)));
			++index;
		} while (query_->next());
	}
	// query thành công nhưng không có kết quả
	else {
		print("KHÔNG TÌM THẤY THỐNG KÊ THỎA MÃN TRÊN CSDL\n\n");
	}
}
