#include "InventoryManager.h"
using namespace std;
typedef InventoryManager::ItemType IT;
typedef InventoryManager::ValueType VT;

const vector<InventoryManager::ItemTypeInfo> InventoryManager::itemInfos{
{"Number", IT::NUMBER, VT::INT, false}, //Name, itemType, dataType, allowZero
{"Name", IT::NAME, VT::STRING, true},
{"Quantity", IT::QUANTITY, VT::INT, true},
{"Use Kilograms", IT::USE_KG, VT::BOOL, true},
{"Size", IT::SIZE, VT::DOUBLE, true},
{"Price", IT::PRICE, VT::DOUBLE, true},
{"Brand", IT::BRAND, VT::STRING, true},
{"Supplier", IT::SUPPLIER, VT::STRING, true},
{"Contact", IT::CONTACT, VT::STRING, true}
};

const bool InventoryManager::NewProfile(vector<string> title) {
	title.push_back("New Profile");
	if (!ChangeCompanyName(title, true)) {
		return false;
	}
	CLEAR;
	FileManager::SelectFile(true, true);
	return UpdateItems();
}
const bool InventoryManager::ChangeCompanyName(vector<string> title, const bool firstTime) {
	string temp = companyName;
	title.push_back(firstTime ? "Set Company Name" : "Change Company Name");
	if (!InputManager::GetString(temp, !firstTime, UI::Title(title), "Company name", "")) {
		return false;
	}
	companyName = temp;
	if (!firstTime) {
		UpdateItems();
	}
	return true;
}
const bool InventoryManager::UpdateItems() {
	UpdateInventoryLength();
	string s;
	s += companyName + FileManager::delimiter;
	for (Item& i : inventory) {
		if (i.itemNumber >= 0) {
			i.netPrice = i.price * i.quantity;
			i.priceWeight = i.price * i.quantity / i.size;
			i.netSize = i.size * i.quantity;
		}
		for (ItemTypeInfo info : itemInfos) {
			auto values = PtrFromItemType(info.itemType, i);
			switch (info.valueType) {
			case ValueType::INT:
				s += to_string(*get<0>(values));
				break;
			case ValueType::DOUBLE:
				s += to_string(*get<1>(values));
				break;
			case ValueType::STRING:
				s += *get<2>(values);
				break;
			case ValueType::BOOL:
				s += to_string(*get<3>(values));
				break;
			}
			s += FileManager::delimiter;
		}
	}
	return FileManager::UpdateFile(FileManager::GetFilePath(), s);
}
const bool InventoryManager::LoadItems() {
	CLEAR;
	FileManager::SelectFile(false, true);
	ifstream file = FileManager::ReadFile(FileManager::GetFilePath());
	if (file.fail()) {
		return false;
	}
	if (!file.eof()) {
		getline(file, companyName, FileManager::delimiter);
	}
	for (Item& item : inventory) {
		if (!file.eof()) {
			if (item.itemNumber >= 0) {
				for (ItemTypeInfo info : itemInfos) {
					string s;
					getline(file, s, FileManager::delimiter);
					auto values = PtrFromItemType(info.itemType, item);
					switch (info.valueType) {
					case ValueType::INT:
						*get<0>(values) = stoi(s);
						break;
					case ValueType::DOUBLE:
						*get<1>(values) = stod(s);
						break;
					case ValueType::STRING:
						*get<2>(values) = s;
						break;
					case ValueType::BOOL:
						*get<3>(values) = (bool)stoi(s);
						break;
					}
				}
				item.netPrice = item.price * item.quantity;
				item.priceWeight = item.price / item.size;
				item.netSize = item.size * item.quantity;
			}
		}
	}
	file.close();
	UpdateInventoryLength();
	return true;
}
const int InventoryManager::GetItemIndex(const int number) {
	if (number <= 0) {
		return -1;
	}
	for (int i = 0; i < maxItems; i++) {
		if (inventory[i].itemNumber == number) {
			return i;
		}
	}
	return -1;
}
const bool InventoryManager::IsValidItem(Item& item) {
	return (item.itemNumber > 0);
}
const bool InventoryManager::IsUsedItemNumber(const int number) {
	return GetItemIndex(number) == -1 ? false : true;
}
void InventoryManager::UpdateInventoryLength() {
	for (int i = 0; i < maxItems; i++) {
		if (!IsValidItem(inventory[i])) {
			inventoryLength = i;
			return;
		}
	}
}
tuple<int*, double*, string*, bool*> InventoryManager::PtrFromItemType(ItemType itemType, Item& item) {
	//e1, e2, e3, e4 etc are global empty variables utilised as a default pointer reference
	switch (itemType) {
	case ItemType::NUMBER:
		return make_tuple(&item.itemNumber,&e2, &e3, &e4);
	case ItemType::SIZE:
		return make_tuple(&e1, &item.size, &e3, &e4);
	case ItemType::USE_KG:
		return make_tuple(&e1, &e2, &e3, &item.useKg);
	case ItemType::QUANTITY:
		return make_tuple(&item.quantity, &e2, &e3, &e4);
	case ItemType::PRICE:
		return make_tuple(&e1, &item.price, &e3, &e4);
	case ItemType::NAME:
		return make_tuple(&e1, &e2, &item.name, &e4);
	case ItemType::CONTACT:
		return make_tuple(&e1, &e2, &item.contact, &e4);
	case ItemType::BRAND:
		return make_tuple(&e1, &e2, &item.brand, &e4);
	case ItemType::SUPPLIER:
		return make_tuple(&e1, &e2, &item.supplier, &e4);
	
	}
}
tuple<double, string> InventoryManager::ValueFromSortItemType(SortItemType itemType, Item& item) {
	switch (itemType) {
	case SortItemType::NUMBER:
		return make_tuple(item.itemNumber, "");
	case SortItemType::NAME:
		return make_tuple(0, item.name);
	case SortItemType::BRAND:
		return make_tuple(0, item.brand);
	case SortItemType::QUANTITY:
		return make_tuple(item.quantity, "");
	case SortItemType::SIZE:
		return make_tuple(item.size, "");
	case SortItemType::NET_SIZE:
		return make_tuple(item.netSize, "");
	case SortItemType::PRICE_UNIT:
		return make_tuple(item.price, "");
	case SortItemType::PRICE_WEIGHT:
		return make_tuple(item.priceWeight, "");
	case SortItemType::NET_PRICE:
		return make_tuple(item.netPrice, "");
	case SortItemType::SUPPLIER:
		return make_tuple(0, item.supplier);
	case SortItemType::CONTACT:
		return make_tuple(0, item.contact);
	}
}
void InventoryManager::SortItems(const SortItemType& itemType, const SortType& sortType, const string& keyword) {
	//Assumes that the inventory length is more than 1
	if (itemType != SortItemType::GENERAL && sortType != SortType::GENERAL) {
		UI::TableColumn column = UI::defaultColumns[static_cast<int>(itemType) - 1];
		bool isNumeric = column.isNumber;
		if (sortType == SortType::KEYWORD) {
			//Keyword
			vector<string> data;
			//Group all relevant strings 
			for (int i = 0; i < inventoryLength; i++) {
				if (isNumeric) {
					data.push_back(to_string(get<0>(ValueFromSortItemType(itemType, inventory[i]))));
				}
				else {
					data.push_back(get<1>(ValueFromSortItemType(itemType, inventory[i])));
					InputManager::ToLowerCase(data.back());
				}
			}
			//Compare strings with keyword and sort them in descending relevance
			while (true) {
				bool hasExchanged = false;
				for (int i = 0; i < data.size() - 1; i++) {
					string s1 = data[i], s2 = data[i + 1];
					//Higher values of string.find indicate lower relevance
					if (s1.find(keyword) > s2.find(keyword)) {
						data[i] = s2;
						data[i + 1] = s1;
						Item temp = inventory[i];
						inventory[i] = inventory[i + 1];
						inventory[i + 1] = temp;
						hasExchanged = true;
					}
				}
				if (!hasExchanged) {
					break;
				}
			}
		}
		else {
			auto compareFunc = [](SortType type, auto value, auto nextValue)
			{
					if (type == SortType::ASCENDING || type == SortType::LOW_INVENTORY) {
						return (value > nextValue);
					}
					else if (type == SortType::DESCENDING) {
						return (value < nextValue);
					}
					return false;
			};
			vector<double> nData;
			vector<string> sData;
			for (int i = 0; i < inventoryLength; i++) {
				auto values = ValueFromSortItemType(itemType, inventory[i]);
				if (isNumeric) {
					double v = get<0>(values);
					if (column.units) {
						if (inventory[i].useKg) {
							if (itemType == SortItemType::SIZE || itemType == SortItemType::NET_SIZE) {
								v *= 1000;
							}
							else if (itemType == SortItemType::PRICE_WEIGHT) {
								v /= 1000;
							}
						}
					}
					nData.push_back(v);
				}
				else {
					sData.push_back(get<1>(values));
					InputManager::ToLowerCase(sData.back());
				}
			}
			while (true) {
				bool hasExchanged = false, comparison;
				for (int i = 0; i < inventoryLength - 1; i++) {
					if (isNumeric) {
						comparison = compareFunc(sortType, nData[i], nData[i + 1]);
					}
					else {
						comparison = compareFunc(sortType, sData[i], sData[i + 1]);
					}
					if (comparison) {
						if (isNumeric) {
							double temp = nData[i];
							nData[i] = nData[i + 1];
							nData[i + 1] = temp;
						}
						else {
							string temp = sData[i];
							sData[i] = sData[i + 1];
							sData[i + 1] = temp;
						}
						Item temp = inventory[i];
						inventory[i] = inventory[i + 1];
						inventory[i + 1] = temp;
						hasExchanged = true;
					}
				}
				if (!hasExchanged) {
					break;
				}
			}
		}
	}
}
tuple<bool, InventoryManager::SortItemType> InventoryManager::SelectSortItemType(std::vector<string> title, const string extraText, const bool onlyNumeric) {
	title.push_back("Choose Item Type");
	while (true) {
		//Choose item type
		UI::optionList = { "Back" };
		for (UI::TableColumn t : UI::defaultColumns) {
			if (onlyNumeric && !t.isNumber) {
				continue;
			}
			UI::optionList.push_back(t.title);
		}
		UI::optionKeyList = { InputManager::exitKey };
		vector<SortItemType> numericOptions;
		for (int i = 1; i <= UI::defaultColumns.size(); i++) {
			if (onlyNumeric) {
				if (!UI::defaultColumns[i - 1].isNumber) {
					continue;
				}
				numericOptions.push_back(static_cast<SortItemType>(i));
				UI::optionKeyList.push_back(to_string(numericOptions.size()));
			}
			else {
				UI::optionKeyList.push_back(to_string(i));
			}
		}
		int option;
		if (InputManager::GetInteger(option, false, false, false, UI::Title(title), "",
			extraText + UI::GenerateOptionsList() + "\n\n Which data field to sort?")) {
			if (option > 0)
			{
				if ((onlyNumeric)) {
					if (option <= numericOptions.size()) {
						return make_tuple(true, numericOptions[option - 1]);
					}
				}
				else if (option <= UI::defaultColumns.size()) {
					return make_tuple(true, static_cast<SortItemType>(option));
				}
			}
			InputManager::InputError(InputErrorType::GENERAL);
		}
		else {
			return make_tuple(false, SortItemType::GENERAL);
		}
	}
}
tuple<InventoryManager::SortItemType, InventoryManager::SortType, string, bool> InventoryManager::SelectSortType(vector<string> title, const string& initialKeyword) {
	while (true) {
		auto result = SelectSortItemType(title, "", false);
		if (!get<0>(result)) {
			return make_tuple(SortItemType::GENERAL, SortType::GENERAL, initialKeyword, false);

		}
		SortType sortType = SortType::GENERAL;
		SortItemType itemType = get<1>(result);
		while (true) {
			//Choose sort type
			UI::optionList = { "Back", "Ascending", "Descending", "Keyword" };
			UI::optionKeyList = { InputManager::exitKey,"1", "2", "3" };
			title.push_back("Choose Sorting Pattern");
			int option;
			if (InputManager::GetInteger(option, false, false, false, UI::Title(title), "", UI::GenerateOptionsList() + "\n\n Sorting pattern?")) {
				if (option > 0 && option <= 3) {
					sortType = static_cast<SortType>(option);
					if (option == 3) {
						while (true) {
							title.push_back("Keyword");
							string keyword;
							if (InputManager::GetString(keyword, false, UI::Title(title), "Keyword", "")) {
								InputManager::ToLowerCase(keyword);
								return make_tuple(itemType, sortType, keyword, true);
							}
							else {
								title.pop_back();
								break;
							}
						}
					}
					else {
						return make_tuple(itemType, sortType, initialKeyword, true);
					}
				}
				else {
					InputManager::InputError(InputErrorType::GENERAL);
					title.pop_back();
				}
			}
			else {
				title.pop_back(); //Reset title and repeat entire loop
				break;
			}
		}
	}
}
const bool InventoryManager::ViewItems(vector<string> title) {
	title.push_back("View Items");
	SortItemType sortItemType = SortItemType::GENERAL;
	SortType sortType = SortType::GENERAL;
	string keyword;
	if (inventoryLength <= 0) {
		InputManager::InputError(InputErrorType::EMPTY_INVENTORY);
		return false;
	}
	while (true) {
		//Sort Items
		if (inventoryLength > 1) {
			SortItems(sortItemType, sortType, keyword);
			UpdateItems();
		}
		UI::optionList = { "Back", "Sort List",
			sortType == SortType::LOW_INVENTORY ? "Undo Low Inventory" : "Low Inventory"
			, "Add Item", "Modify Item", "Delete Item" };
		UI::optionKeyList = { InputManager::exitKey,"1", "2", "3", "4", "5" };
		char option;
		if (InputManager::GetChar(option, false, UI::Title(title), "",
			GenerateItemsTable(sortType, make_tuple(false, 0)) + UI::GenerateOptionsList())) {
			switch (option) {
			case '1':
			{
				auto sortFormat = SelectSortType(title, keyword); //Unpack tuple
				if (get<3>(sortFormat) == true) {
					tie(sortItemType, sortType, keyword, ignore) = sortFormat;
				}
			}
			break;
			case '2':
				sortItemType = SortItemType::QUANTITY;
				sortType = sortType == SortType::LOW_INVENTORY ? SortType::GENERAL : SortType::LOW_INVENTORY;
				break;
			case '3':
				AddItem(title);
				break;
			case '4':
			{
				auto result = SelectItem(title, sortType);
				if (get<0>(result) == true) {
					ModifyItem(title, get<1>(result));
				}
				break;
			}
			case '5':
			{
				auto result = SelectItem(title, sortType);
				if (get<0>(result) == true) {
					DeleteItem(title, get<1>(result));
				}
				break;
			}
			default: InputManager::InputError(InputErrorType::GENERAL);
			}
		}
		else {
			return false;
		}
	}
}
string InventoryManager::GenerateItemsTable(SortType sortType, tuple<bool, int> specificItem) {
	//Assumes that the inventory is not empty
	string table;
	table += UI::GenerateTableBorder(UI::defaultColumns, unitsMaxLength)
		+ UI::GenerateTableRow(true, UI::defaultColumns, {}, {}, unitsMaxLength)
		+ UI::GenerateTableBorder(UI::defaultColumns, unitsMaxLength);
	//Print rows
	bool specific = get<0>(specificItem);
	int index = get<1>(specificItem);
	for (int a = 0; a < inventoryLength; a++) {
		if (specific) {
			if (a != index) {
				continue;
			}
		}
		Item& i = inventory[a];
		if (sortType == SortType::LOW_INVENTORY && i.quantity >= 10) {
			continue;
		}
		vector<string> values;
		for (int b = 1; b <= UI::defaultColumns.size(); b++) {
			auto results = ValueFromSortItemType(static_cast<SortItemType>(b), i);
			bool isNumeric = UI::defaultColumns[b - 1].isNumber;
			values.push_back(isNumeric ? to_string(get<0>(results)) : get<1>(results));
		}
		string units = (i.useKg) ? " kg" : " g";
		string units2 = (i.useKg) ? " kg-1" : " g-1";
		std::vector<std::string> unitsArray = { "", "", "", units, units, "", units2, "", "", "", "" };
		table += UI::GenerateTableRow(false, UI::defaultColumns, values, unitsArray, unitsMaxLength);
	}
	table += UI::GenerateTableBorder(UI::defaultColumns, unitsMaxLength);
	return table;
}
const bool InventoryManager::AddItem(vector<string> title) {
	title.push_back("Add Item");
	Item temp;
	for (int i = 0; i < itemInfos.size(); i++) {
		ItemTypeInfo info = itemInfos[i];
		title.push_back(info.name);
		auto ptrTuple = PtrFromItemType(info.itemType, temp);
		bool back = false;
		if (info.valueType == VT::INT) {
			int *ptr = get<0>(ptrTuple);
			if (InputManager::GetInteger(*ptr, !(info.itemType == ItemType::NUMBER), false, false, UI::Title(title), info.name, "")) {
				if (info.itemType == ItemType::NUMBER && IsUsedItemNumber(*ptr)) {
					InputManager::InputError(InputErrorType::ITEM_EXISTS);
					i -= 1;
				}
			}
			else {
				back = true;
			}
		}
		else if (info.valueType == VT::DOUBLE) {
			double *ptr = get<1>(ptrTuple);
			if (!InputManager::GetDouble(*ptr, true, false, false, UI::Title(title), info.name, "")) {
				back = true;
			}
		}
		else if (info.valueType == VT::STRING) {
			string *ptr = get<2>(ptrTuple);
			if (!InputManager::GetString(*ptr, false, UI::Title(title), info.name, "")) {
				back = true;
			}

		}
		else if (info.valueType == VT::BOOL) {
			bool *ptr = get<3>(ptrTuple);
			if (!InputManager::GetBool(*ptr, false, UI::Title(title), info.name, "")) {
				back = true;
			}
		}
		if (back) {
			if (i > 0) {
				i -= 2;
			}
			else {
				return false;
			}
		}
		title.pop_back();
	}
	inventory[inventoryLength] = temp;
	UpdateItems();
	return true;
}
tuple<bool, int> InventoryManager::SelectItem(vector<string> title, SortType sortType) {
	title.push_back("Select Item");
	if (inventoryLength <= 0) {
		InputManager::InputError(InputErrorType::EMPTY_INVENTORY);
		return make_tuple(false, 0); //Forced exit
	}
	while (true) {
		int option;
		if (InputManager::GetInteger(option, false, false, false, UI::Title(title), "",
			GenerateItemsTable(sortType, make_tuple(false, 0)) + "\n\n Item number?" + UI::option_Single())) {
			if (IsUsedItemNumber(option)) {
				int b = GetItemIndex(option);
				return make_tuple(true, GetItemIndex(option));
			}
			else {
				InputManager::InputError(InputErrorType::ITEM_NOT_FOUND);
			}
		}
		else {
			return make_tuple(false, 0); //User exit
		}
	}
}
tuple<bool, InventoryManager::ItemTypeInfo> InventoryManager::SelectItemType(vector<string> title,  const string extraText) {
	//Assumes the inventory is not empty
	title.push_back("Select Item Type");
	while (true) {
		UI::optionList = { "Back" };
		UI::optionKeyList = { InputManager::exitKey };
		for (ItemTypeInfo i : itemInfos) {
			UI::optionList.push_back(i.name);
		}
		for (int i = 1; i <= itemInfos.size(); i++) {
			UI::optionKeyList.push_back(to_string(i));
		}
		int option;
		if (InputManager::GetInteger(option, false, false, false, UI::Title(title), "",
			extraText + UI::GenerateOptionsList()+"\n\n Data type?")) {
			if (option > 0 && option <= itemInfos.size()) {
				return make_tuple(true, itemInfos[option - 1]);
			}
			else {
				InputManager::InputError(InputErrorType::GENERAL);
			}
		}
		else {
			return make_tuple(false, itemInfos[0]);// User exit
		}
	}
}
const bool InventoryManager::ModifyItem(vector<string> title, int index) {
	title.push_back("Modify Item");
	Item& item = inventory[index];
	while (true) {
		auto result = SelectItemType(title, GenerateItemsTable(SortType::GENERAL, make_tuple(true, index)));
		if (get<0>(result) == false) {
			UpdateItems();
			return false; // User exit
		}
		ItemTypeInfo info = get<1>(result);
		title.push_back(info.name);
		auto ptrTuple = PtrFromItemType(info.itemType, item);
		if (info.valueType == VT::INT) {
			int *ptr = get<0>(ptrTuple);
			InputManager::GetInteger(*ptr, false, false, true, UI::Title(title), info.name, "");
		}
		else if (info.valueType == VT::DOUBLE) {
			double *ptr = get<1>(ptrTuple);
			InputManager::GetDouble(*ptr, true, false, true, UI::Title(title), info.name, "");
		}
		else if (info.valueType == VT::STRING) {
			string *ptr = get<2>(ptrTuple);
			InputManager::GetString(*ptr, true, UI::Title(title), info.name, "");

		}
		else if (info.valueType == VT::BOOL) {
			bool *ptr = get<3>(ptrTuple);
			InputManager::GetBool(*ptr, true, UI::Title(title), info.name, "");
		}
		UpdateItems();
		title.pop_back();
	}
}
const bool InventoryManager::DeleteItem(vector<string> title, int index) {
	title.push_back("Delete Item");
	Item& item = inventory[index];
	bool option;
	if (!InputManager::GetBool(option, false, UI::Title(title), "",
		GenerateItemsTable(SortType::GENERAL, make_tuple(true, index)) + "\n\n You are about to delete this item.\n Are you sure?")) {
		return false; // User exit
	}
	if (option) {
		item.itemNumber = 0;
		UpdateItems();
		return true;
	}
	else {
		return false; //Forced exit
	}
}
const bool InventoryManager::DeleteInventoryFile(vector<string> title) {
	title.push_back("Delete Profile");
	CLEAR;
	bool option;
	if (!InputManager::GetBool(option, false, UI::Title(title), "", "\n\n Delete profile? (Warning: The file will be permanently erased.)")) {
		return false; //User exit
	}
	if (option) {
		FileManager::DeleteFile(FileManager::GetFilePath());
		return true;
	}
	else {
		return false; //Forced exit
	}
}
const bool InventoryManager::Overview(vector<string> title) {
	title.push_back("Overview");
	while (true) {
		CLEAR;
		string info = "\n\n Company name: " + companyName;
		int totalItems = 0, totalQuantity = 0;
		double totalSize = 0, totalPrice = 0;
		for (Item i : inventory) {
			if (IsValidItem(i)) {
				totalItems++;
				totalQuantity += i.quantity;
				totalSize += i.useKg ? i.netSize : i.netSize / 1000; // Calculate total size in KG
				totalPrice += i.netPrice;
			}
		}
		info += "\n\n Total items: " + to_string(totalItems);
		info += "\n Total quantity: " + to_string(totalQuantity);
		info += "\n Total size: " + to_string(totalSize) + " kg";
		info += "\n Total price value: " + to_string(totalPrice);
		UI::optionList = { "Back" , "Change Company Name",  "Distributions" };
		UI::optionKeyList = { InputManager::exitKey , "1", "2" };
		char option;
		if (InputManager::GetChar(option, false, UI::Title(title), "", info + UI::GenerateOptionsList())) {
			switch (option) {
			case '1':
				ChangeCompanyName(title, false);
				break;
			case '2':
				Distributions(title);
				break;
			default:InputManager::InputError(InputErrorType::GENERAL);
			}
		}
		else {
			return false;// User exit
		}
	}
}
const bool InventoryManager::Distributions(vector<string> title) {
	while (true) {
		if (inventoryLength <= 0) {
			InputManager::InputError(InputErrorType::EMPTY_INVENTORY);
			return false; //Forced exit
		}
		title.push_back("Distributions");
		auto result = SelectSortItemType(title, "", true);
		CLEAR;
		if (get<0>(result) == false) {
			return false; // User exit
		}
		SortItemType info = get<1>(result);
		UI::TableColumn column = UI::defaultColumns[static_cast<int>(info) - 1];
		title.push_back(column.title);
		vector<double> data;
		double meanValue = 0, medianValue = 0, stdDeviation = 0;
		for (int i = 0; i < inventoryLength; i++) {
			double value = get<0>(ValueFromSortItemType(info, inventory[i]));
			if (column.units) {
				if (inventory[i].useKg) {
					value *= 1000;
				}
			}
			data.push_back(value);
			meanValue += value / inventoryLength;
		}
		for (double d : data) {
			stdDeviation += pow(d - meanValue, 2.0);
		}
		stdDeviation = sqrt(stdDeviation / inventoryLength);
		while (true) {
			bool hasExchanged = false;
			for (int i = 0; i < inventoryLength - 1; i++) {
				if (data[i] > data[i + 1]) {
					double temp = data[i];
					data[i] = data[i + 1];
					data[i + 1] = temp;
					hasExchanged = true;
				}
			}
			if (!hasExchanged) {
				break;
			}
		}
		if (data.back() <= 0) {
			InputManager::InputError(InputErrorType::DISTRIBUTION);
			return false;
		}
		medianValue = data[inventoryLength / 2];
		int divisions = 5, subDivisions = 5, maxBars = 10, barThickness = 3, xInitialGap = 0, barGap = 2;
		double percentageError = 0.1;
		double unitY = data.back() / divisions;
		double subUnitY = unitY / subDivisions;
		if (data.size() > maxBars) {
			data.erase(data.begin(), data.begin() + (data.size() - maxBars));
		}
		string text = UI::Title(title);
		text += "\n\n Maximum : " + to_string(data.back());
		text += "\n Minimum : " + to_string(data.front());
		text += "\n Mean : " + to_string(meanValue);
		text += "\n Median : " + to_string(medianValue);
		text += "\n Standard deviation : " + to_string(stdDeviation);
		text += "\n\n " + column.title + (column.units ? " (in grams)" : "") + "\n ^\n |\n |";
		for (int i = 0; i < divisions; i++) {
			int gap = to_string(data.back() - unitY * i).size();
			if (gap > xInitialGap) {
				xInitialGap = gap;
			}
		}
		xInitialGap += barGap;
		for (int i = 0; i < divisions; i++) {
			text += "\n - " + to_string(data.back() - unitY * i);
			text += string(xInitialGap - to_string(data.back() - unitY * i).size(), ' ');
			for (int j = 0; j < subDivisions; j++) {
				double value = data.back() - unitY * i - subUnitY * j;
				text += j == 0 ? "" : "\n | " + string(xInitialGap, ' ');
				for (int x1 = 0; x1 < data.size(); x1 += 2) {
					if (data[x1] >= value || fabs((data[x1] - value) / value) <= percentageError) {
						text += string(barThickness, '*');
					}
					else {
						text += string(barThickness, ' ');
					}
					text += string(barGap, ' ');
				}
				for (int x2 = data.size() % 2 == 0 ? data.size() - 1 : data.size() - 2; x2 > 0; x2 -= 2) {
					if (data[x2] >= value || fabs((data[x2] - value) / value) * 100 <= percentageError) {
						text += string(barThickness, '*');
					}
					else {
						text += string(barThickness, ' ');
					}
					text += string(barGap, ' ');
				}
			}
		}
		text += "\n |" + string(xInitialGap, '-');
		for (int x = 0; x < data.size(); x++) {
			text += string(barThickness + barGap, '-');
		}
		text += "-> Items\n 0\n\n";
		UI::PeriodicPrint(text);
		PAUSE;
		title.pop_back();
		title.pop_back();
	}
}