#ifndef InventoryManager_H
#define InventoryManager_H
#include <iostream>
#include <string>
#include <tuple>
#include "InputManager.h"
#include "FileManager.h"
#include "UI.h"

class InventoryManager {
public:
	enum class ValueType {
		INT, DOUBLE, STRING, BOOL
	};
	struct Item {
		//Fundamental values
		int itemNumber = 0, quantity = 0;
		double size = 0, price = 0;
		bool useKg = false;
		std::string name, contact, brand, supplier;
		//Derived values
		double netSize = 0, priceWeight = 0, netPrice = 0;
	};
	enum class ItemType {
		NUMBER, NAME, QUANTITY, USE_KG, SIZE,  PRICE, BRAND, SUPPLIER, CONTACT
	};
	struct ItemTypeInfo {
		std::string name;
		ItemType itemType;
		ValueType valueType;
		bool allowZero;
	};
	enum class SortType {
		GENERAL = 0, ASCENDING, DESCENDING, KEYWORD, LOW_INVENTORY
	};
	enum class SortItemType { //Same number of elements as the dafaultColumns in UI
		GENERAL = 0, NUMBER, NAME, QUANTITY, SIZE, NET_SIZE, PRICE_UNIT, PRICE_WEIGHT, NET_PRICE, BRAND, SUPPLIER, CONTACT
	};
	//Empty variables for default values
	int e1; double e2; std::string e3; bool e4;
	std::string companyName;
	static constexpr int maxItems = 100;
	//Public Methods
	const bool NewProfile(std::vector<std::string> title);
	const bool ChangeCompanyName(std::vector<std::string> title, const bool firstTime);
	const bool LoadItems();
	const bool ViewItems(std::vector<std::string> title);
	const bool AddItem(std::vector<std::string> title);
	std::tuple<bool, int> SelectItem(std::vector<std::string> title, SortType sortType); //Returns index of item
	const bool ModifyItem(std::vector<std::string> title, const int index);
	const bool DeleteItem(std::vector<std::string> title, const int index);
	const bool DeleteInventoryFile(std::vector<std::string> title);
	const bool Overview(std::vector<std::string> title);
	const bool Distributions(std::vector<std::string> title);
private:
	//Items
	Item inventory[maxItems];
	static const std::vector<ItemTypeInfo> itemInfos;
	static const int unitsMaxLength = 5;
	int inventoryLength;
	void UpdateInventoryLength();
	const int GetItemIndex(const int number); // Returns item index using item number
	const bool IsUsedItemNumber(const int number);
	const bool IsValidItem(Item& item); //Returns true if item is valid
	const bool UpdateItems();
	std::tuple<bool, ItemTypeInfo> SelectItemType(std::vector<std::string> title, const std::string extraText); //Returns selected item type
	std::tuple<int*, double*, std::string*, bool*> PtrFromItemType(ItemType itemType, Item& item);
	std::string GenerateItemsTable(SortType sortType, std::tuple<bool, int> specificItem);
	//Sorting
	void SortItems(const SortItemType& itemType, const SortType& sortType, const std::string& keyword);
	std::tuple<double, std::string> ValueFromSortItemType(SortItemType itemType, Item& item);
	std::tuple<bool, SortItemType> SelectSortItemType(std::vector<std::string> title, const std::string extraText, const bool onlyNumeric);
	std::tuple<SortItemType, SortType, std::string, bool> SelectSortType(std::vector<std::string> title, const std::string& initialKeyword); //Returns selected item type to be sorted and the sort type 
};

#endif
