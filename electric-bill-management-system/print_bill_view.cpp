/*
 * This is a personal academic project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "print_bill_view.h"
#include "console.h"

using database::confirm_process;
using database::execute_query;
using database::selection_process;
using fmt::arg;
using fmt::format;
using fmt::print;
using std::cin;

view::PrintBillView::PrintBillView(QSqlDatabase* database)
		: query_{std::make_unique<QSqlQuery>(*database)} {}

view::PrintBillView::~PrintBillView() = default;

void view::PrintBillView::Run() {
	console::SetupConsole();
	PerformSearchBillScene();
	PerformEndScene();
}

void view::PrintBillView::PerformEndScene() {
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

void view::PrintBillView::PerformSearchBillScene() {
	cin.clear();
	console::ClearConsoleScreen();
	print("IN HÓA ĐƠN\n\n");
	/*
	 * Nhập mã hóa đơn muốn in thông tin
	 */
	print("NHẬP MÃ HÓA ĐƠN: ");
	UnsignedInteger bill_id{};
	cin >> bill_id;
	print("NHẬP THÀNH CÔNG, THỰC HIỆN TÌM KIẾM...\n");

	// chuẩn bị query để tìm kiếm trên csdl
	query_->finish();
	query_->prepare(format("select Bill.bill_id, Bill.electric_units, Bill.money,"
												 "Client.client_id, Client.full_name, Client.id_card,"
												 "Client.address, Client.phone_number,"
												 "ElectricIndex.old_index, ElectricIndex.new_index,"
												 "Time.month_date "
												 "from Bill, Client, ElectricIndex, Time "
												 "where ("
												 "Bill.bill_id = {} "
												 "and Bill.client_id = Client.client_id "
												 "and ElectricIndex.client_id = Client.client_id "
												 "and ElectricIndex.month_id = Time.month_id"
												 ")",
												 bill_id)
											.data());

	// chạy query
	execute_query(
			query_.get(),
			std::bind(&PrintBillView::OnQueryFindBillByIdSuccess, this, bill_id),
			[this] {
				print("CÓ LỖI TRONG QUÁ TRÌNH TÌM HÓA ĐƠN TRÊN CSDL\n");
				print("THÔNG TIN LỖI: ");
				qInfo() << query_->lastError();
			});
}

void view::PrintBillView::PerformPrintBillScene() const {
	const auto bill_id = query_->value(0).toString().toStdString();
	const auto month_date = query_->value(10).toString().toStdString();
	const auto client_id = query_->value(3).toString().toStdString();
	const auto full_name = query_->value(4).toString().toStdString();
	const auto id_card = query_->value(5).toString().toStdString();
	const auto address = query_->value(6).toString().toStdString();
	const auto phone_number = query_->value(7).toString().toStdString();
	const auto old_index = query_->value(8).toString().toStdString();
	const auto new_index = query_->value(9).toString().toStdString();
	const auto electric_units = query_->value(1).toString().toStdString();
	const auto money = query_->value(2).toString().toStdString();
	print(
			"Mã Hóa Đơn: {bill_id}\n"
			"Tháng: {month_date}\n"
			"Mã Hộ: {client_id}\n"
			"Họ và Tên: {full_name}\n"
			"Số Điện Thoại: {phone_number}\n"
			"Số CMND: {id_card}\n"
			"Địa Chỉ: {address}\n"
			"Chỉ số cũ: {old_index}\n"
			"Chỉ số mới: {new_index}\n"
			"Tổng đơn vị điện tiêu thụ: {electric_units}\n"
			"Thành Tiền: {money} VND\n\n",
			arg("bill_id", bill_id),
			arg("month_date", month_date),
			arg("client_id", client_id),
			arg("full_name", full_name),
			arg("phone_number", phone_number),
			arg("id_card", id_card),
			arg("address", address),
			arg("old_index", old_index),
			arg("new_index", new_index),
			arg("electric_units", electric_units),
			arg("money", money));
}

void view::PrintBillView::OnQueryFindBillByIdSuccess(
		const UnsignedInteger bill_id) const {
	// tìm thấy kết quả
	if (query_->isSelect() && query_->first()) {
		/*
		 * In ra thông tin
		 */
		PerformPrintBillScene();
	}
	// query thành công nhưng không có kết quả
	else {
		print("KHÔNG TÌM THẤY HỘ CÓ MÃ {} THỎA MÃN TRÊN CSDL\n\n", bill_id);
	}
}
