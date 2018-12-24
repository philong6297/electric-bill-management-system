select Client.client_id, full_name, id_card, address, phone_number, birth_date, register_date, old_index, new_index, electric_units, money
from Client, Bill, ElectricIndex, Statistics
where Client.client_id = Bill.client_id 
and Client.client_id = ElectricIndex.client_id 
and Client.client_id = Statistics.client_id
and Statistics.status = 0
	