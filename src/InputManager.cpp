#include "InputManager.h"
using namespace std;

const string InputManager::yesKey = "y"; //Define static strings in implementation file
const string InputManager::noKey = "n";
const string InputManager::exitKey = "/";

//Methods
bool InputManager::IsInteger(const string& str) {
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])) {
			if (str[i] == '-' && i == 0) {
				continue;
			}
			return false;
		}
	}
	return true;
}
bool InputManager::IsDouble(const string& str) {
	short dots = 0;
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])) {
			if (str[i] == '-' && i == 0) {
				continue;
			}
			if (str[i] == '.') {
				dots++;
				if (dots > 1) {
					return false;
				}
			}
			else {
				return false;
			}
		}
	}
	return true;
}
const bool InputManager::GetString(string& placeholder, const bool& showPrevious, const string& title, const string& name, const string& custom) {
	while (true) {
		CLEAR;
		string s;
		UI::PeriodicPrint(title);
		if (!name.compare("")) {
			//Custom text//Used when the name string is empty
			s += custom;
			s += (showPrevious) ? "\n Previous value: " + placeholder : "";
		}
		else {
			//Default text
			s += "\n\n Please  type the : " + name;
			s += (showPrevious) ? "\n Previous value: " + placeholder : "";
			s += UI::option_Single();
		}
		s += UI::inputMarker;
		UI::PeriodicPrint(s);
		char temp[inputSize];
		cin.getline(temp, inputSize);
		Flush(cin);
		if (!CheckInputError())
		{
			if (string(temp).compare("")) {
				placeholder = temp;
				return placeholder.compare(exitKey);
			}
			else {
				InputError(InputErrorType::EMPTY_INPUT);
			}
		}
		else {
			InputError(InputErrorType::GENERAL);
		}
	}
}
const bool InputManager::GetChar(char& placeholder, const bool& showPrevious, const string& title, const string& name, const string& custom) {
	while (true) {
		string s;
		if (GetString(s, showPrevious, title, name, custom)) {
			RemoveWhiteSpace(s);
			if (s.length() > 1) {
				InputError(InputErrorType::CHAR);
			}
			else {
				placeholder = s[0];
				return true;
			}
		}
		else {
			return false;
		}
	}
}
const bool InputManager::GetInteger(int& placeholder, const bool& allowZero, const bool& allowNegative, const bool& showPrevious, const string& title, const string& name, const string& custom) {
	while (true) {
		string s = showPrevious ? to_string(placeholder) : "";
		if (GetString(s, showPrevious, title, name, custom)) {
			RemoveWhiteSpace(s);
			if (IsInteger(s)) {
				int n = stoi(s);
				if (n == 0 && !allowZero) {
					InputError(InputErrorType::ZERO);
					continue;
				}
				if (n < 0 && !allowNegative) {
					InputError(InputErrorType::POSITIVE);
					continue;
				}
				placeholder = n;
				return true;
			}
			else {
				InputError(InputErrorType::INTEGER);
			}
		}
		else {
			return false;
		}
	}
}
const bool InputManager::GetDouble(double& placeholder, const bool& allowZero, const bool& allowNegative, const bool& showPrevious, const string& title, const string& name, const string& custom) {
	while (true) {
		string s = showPrevious ? to_string(placeholder) : "";
		if (GetString(s, showPrevious, title, name, custom)) {
			RemoveWhiteSpace(s);
			if (IsDouble(s)) {
				double d = stoi(s);
				if (d == 0 && !allowZero) {
					InputError(InputErrorType::ZERO);
					continue;
				}
				if (d < 0 && !allowNegative) {
					InputError(InputErrorType::POSITIVE);
					continue;
				}
				placeholder = d;
				return true;
			}
			else {
				InputError(InputErrorType::DOUBLE);
			}
		}
		else {
			return false;
		}
	}
}
const bool InputManager::GetBool(bool& placeholder, const bool& showPrevious, const string& title, const string& name, const string& custom) {
	string previous = placeholder ? "Yes" : "No";
	while (true) {
		char option;
		string text;
		if (!name.compare("")) {
			text += custom;
		}
		else {
			text += "\n\n " + name + "?";
		}
		if (showPrevious) {
			text += "\n Previous value: " + previous;
		}
		text += UI::option_YesNo();
		if (InputManager::GetChar(option, false, title, "", text)) {
			option = tolower(option);
			if (option == InputManager::yesKey[0]) {
				placeholder = true;
				return true;
			}
			else if (option == InputManager::noKey[0]) {
				placeholder = false;
				return true;
			}
			else {
				InputManager::InputError(InputErrorType::GENERAL);
			}
		}
		else {
			return false;
		}
	}
}
void InputManager::Flush(istream& in) {
	in.clear();
}
bool InputManager::CheckInputError() {
	if (cin.fail() || cin.eof())
	{
		return true;
	}
	return false;
}
void InputManager::InputError(InputErrorType type) {
	Flush(cin);
	CLEAR;
	Color::ChangeColor(Color::ColorType::TURQUOISE);
	switch (type) {
	case InputErrorType::GENERAL:
		UI::PeriodicPrint("\n Invalid Input! Please try again.\n");
		break;
	case InputErrorType::EMPTY_INPUT:
		UI::PeriodicPrint("\n Please type something!\n");
		break;
	case InputErrorType::CHAR:
		UI::PeriodicPrint("\n Invalid character!\n");
		break;
	case InputErrorType::NUMBER:
		UI::PeriodicPrint("\n Invalid number!\n");
		break;
	case InputErrorType::INTEGER:
		UI::PeriodicPrint("\n Invalid integer! Fractional numbers or characters are not allowed.\n");
		break;
	case InputErrorType::DOUBLE:
		UI::PeriodicPrint("\n Invalid decimal number!\n");
		break;
	case InputErrorType::ZERO:
		UI::PeriodicPrint("\n Zero is not allowed!\n");
		break;
	case InputErrorType::POSITIVE:
		UI::PeriodicPrint("\n The number should be positive!\n");
		break;
	case InputErrorType::ITEM_EXISTS:
		UI::PeriodicPrint("\n The item already exists! Please try a different number.\n");
		break;
	case InputErrorType::ITEM_NOT_FOUND:
		UI::PeriodicPrint("\n The item cannot be found!\n");
		break;
	case InputErrorType::EMPTY_INVENTORY:
		UI::PeriodicPrint("\n The inventory is empty!\n");
		break;
	case InputErrorType::DISTRIBUTION:
		UI::PeriodicPrint("\n The distribution could not be generated!\n");
		break;
	case InputErrorType::PROFILE_NEW:
		UI::PeriodicPrint("\n The profile could not be created!\n");
		break;
	case InputErrorType::PROFILE_LOAD:
		UI::PeriodicPrint("\n The profile failed to load!\n");
		break;
	case InputErrorType::PROFILE_UPDATE:
		UI::PeriodicPrint("\n The profile could not be updated!\n");
		break;
	case InputErrorType::PROFILE_DELETE:
		UI::PeriodicPrint("\n The profile could not be deleted!\n");
		break;
	}
	Sleep(InputManager::sleepInterval);
	Color::ChangeColor(Color::defaultColor);
}
void InputManager::RemoveWhiteSpace(string& s) {
	s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
}
void InputManager::ToLowerCase(string& s) {
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}