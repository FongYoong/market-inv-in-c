#include "InventoryManager.h"
using namespace std;

void MainMenu(InventoryManager& inventory);
void About(vector<string> title);

int main()
{
	Color::ChangeColor(Color::ColorType::WHITE);
	while (true) {
		bool option;
		if (InputManager::GetBool(option, false, UI::Title({}), "", "\n\n Load file?")) {
			InventoryManager inventory;
			if (option) {
				if (inventory.LoadItems()) {
					MainMenu(inventory);
				}
				else {
					InputManager::InputError(InputErrorType::PROFILE_LOAD);
				}
			}
			else {
				if (inventory.NewProfile({})) {
					MainMenu(inventory);
				}
				else {
					InputManager::InputError(InputErrorType::PROFILE_NEW);
				}
			}
		}
		else {
			//exitKey
			CLEAR;
			UI::PeriodicPrint("\n Thank you for using the program.\n\n", 10);
			Sleep(500);
			exit(0);
		}
	}
	exit(0);
}
void MainMenu(InventoryManager& inventory) {
	while (true) {
		UI::optionList = { "Exit Profile", "View Items", "Add Item", "Modify Item", "Delete Item", "Profile Overview", "Delete Profile", "About" };
		UI::optionKeyList = { InputManager::exitKey,"1", "2", "3", "4", "5", "6", "7" };
		vector<string> title = { "Main Menu" };
		char option;
		if (InputManager::GetChar(option, false, UI::Title(title), "",
			"\n\n Company name: " + inventory.companyName + UI::GenerateOptionsList() + "\n\n What would you like to do?")) {
			option = tolower(option);
			switch (option) {
			case '1':
				inventory.ViewItems(title);
				break;
			case '2':
				inventory.AddItem(title);
				break;
			case '3':
			{
				auto result = inventory.SelectItem(title, InventoryManager::SortType::GENERAL);
				if (get<0>(result) == true) {
					inventory.ModifyItem(title, get<1>(result));
				}
			}
				break;
			case '4':
			{
				auto result = inventory.SelectItem(title, InventoryManager::SortType::GENERAL);
				if (get<0>(result) == true) {
					inventory.DeleteItem(title, get<1>(result));
				}
			}
				break;
			case '5':
				inventory.Overview(title);
				break;
			case '6':
			{
				if (inventory.DeleteInventoryFile(title)) {
					return;
				}
			}
				break;
			case '7':
				About(title);
				break;
			default: InputManager::InputError(InputErrorType::GENERAL);
			}
		}
		else {
			//exitKey
			int btn = MessageBox(0, "Are you sure?", "Exit User Profile", MB_YESNO + MB_ICONWARNING);
			if (btn == IDYES) {
				return;
			}
		}
	}
}
void About(vector<string> title) {
	CLEAR;
	title.push_back("About");
	string text = UI::Title(title);
	text += "\n\n ~ Program name: " + UI::rootTitle;
	text += "\n ~ Programming Language: The Infamous C++, the one which gives programmers the Big C.";
	text += "\n ~ Purpose: To pass our current semester.";
	text += "\n\n ~ Credits:";
	text += "\n  > UTAR, for delaying our journey towards dementia.";
	text += "\n  > UTAR lecturers for their guidance, especially Dr.Sugumaran a/l Nallusamy, our practical lecturer.";
	text += "\n  > The janitors, for preventing the re-emergence of the Black Plague.";
	text += "\n  > God, for keeping us alive.";
	text += "\n\n ~ Team:\n  > Fong Chien Yoong\n  > Ling Sze Chie\n  > Sim Jia Wei \n  > Tong Zhi-Shun";
	text += "\n\n ~ Team motto: \"Rakyat didahulukan, pencapaian diutamakan.\"";
	text += "\n\n ~ Disclaimer: No animals were harmed in the making of this program.";
	UI::PeriodicPrint(text + "\n\n ");
	PAUSE;
}