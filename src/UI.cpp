#include "UI.h"
using namespace std;

const ios_base::fmtflags UI::originalCoutFormat = cout.flags();
const string UI::rootTitle = "Mini Market Inventory";
const string UI::inputMarker = "\n\n>> ";
vector<string> UI::optionList { "" };
vector<string> UI::optionKeyList { "" };

//Table columns
const vector<UI::TableColumn> UI::defaultColumns
{
	//Name, precision, width, showpoint, isNumber, hasUnits
{ "Number", 0, 6, false, true, false},
{ "Name", 0, 15, false, false, false},
{ "Quantity", 0, 8, false, true, false},
{ "Size (per unit)", 2, 15, true, true, true},
{ "Net Size", 2, 10, true, true, true},
{ "Price (per unit)", 2, 16, true, true, false},
{ "Price (per weight)", 2, 18, true, true, true},
{ "Net Price", 2, 10, true, true, false},
{ "Brand", 0, 15, false, false, false},
{ "Supplier", 0, 15, false, false, false},
{ "Contact", 0, 15, false, false, false}
};

 //Methods
void UI::PeriodicPrint(const string& text) {
	for (const char c : text) {
		cout << c;
		if (printInterval > 0) {
			Sleep(printInterval);
		}
	}
}
void UI::PeriodicPrint(const string& text, DWORD interval) {
	for (const char c : text) {
		cout << c;
		if (interval > 0) {
			Sleep(interval);
		}
	}
}
string UI::FormatWidth(string& s, int width) {
	int d = s.length() - width;
	if (d > 0) {
		s.replace(width, d, "");
		if (width >= 6) {
			for (int i = 1; i <= UI::maxDots; i++) {
				s.replace(width - i, 1, ".");
			}
		}
	}
	else if (d < 0) {
		d = abs(d);
		if (d % 2 == 0) {
			//Even spacing
			return string(d / 2, ' ') + s + string(d / 2, ' ');
		}
		else {
			//Odd spacing
			return string(d / 2, ' ') + s + string((d / 2) + 1, ' ');
		}
	}
	return s;
}
string UI::GenerateOptionsList() {
	string optionHeader = " [ OPTIONS ] ";
	string text = "\n\n";
	int maxWidth = 0, maxKeyWidth = 0, totalWidth;
	for (string s : optionList) {
		if (s.length() > maxWidth) {
			maxWidth = s.length();
		}
	}
	for (string s : optionKeyList) {
		if (s.length() > maxKeyWidth) {
			maxKeyWidth = s.length();
		}
	}
	totalWidth = optionHeader.size() + maxWidth + maxKeyWidth + 10;
	text += string(totalWidth, '-');
	for (int i = 0; i < optionKeyList.size(); i++) {
		text += "\n";
		text += (i == 0 ? optionHeader : string(optionHeader.size(), ' ')) + "[ " + FormatWidth(optionList[i], maxWidth)
			+ " :: <" + FormatWidth(optionKeyList[i], maxKeyWidth) + "> ]";
	}
	text += "\n" + string(totalWidth, '-');
	return text;
}
/*
string UI::GenerateOptionsMiniList() {
	string text = "\n";
	for (int i = 0; i < optionKeyList.size(); i++) {
		text += " [ <" + optionKeyList[i] + "> :: '" + optionList[i] + "' ]";
	}
	return text;
}
*/
string UI::GenerateTableRow(bool isTitle, vector<TableColumn> format, vector<string> values, vector<string> units, const int unitsMaxLength) {
	string text = "\n";
	for (int i = 0; i < format.size(); i++) {
		text += (i == 0) ? " [ " : " | ";
		TableColumn cFormat = format[i];
		if (isTitle) {
			text += FormatWidth(cFormat.title, cFormat.width + (cFormat.units ? unitsMaxLength : 0));
		}
		else {
			if (cFormat.isNumber) {
				ostringstream stream;
				stream << fixed << setprecision(cFormat.precision) << (cFormat.showPoint ? showpoint : noshowpoint)
					<< setw(cFormat.width) << stod(values[i]);
				string numString = stream.str();
				text += FormatWidth(numString, cFormat.width) + (cFormat.units ? units[i] + string(" ", unitsMaxLength - units[i].length()) : "");
			}
			else {
				//No number formatting//No units
				text += FormatWidth(values[i], cFormat.width);
			}
		}
	}
	text += " ]";
	return text;
	//cout.flags(originalCoutFormat);
}
string UI::GenerateTableBorder(vector<TableColumn> format, const int unitsMaxLength) {
	int count = 4;
	for (int i = 0; i < format.size(); i++) {
		TableColumn cFormat = format[i];
		count += cFormat.width + (cFormat.units ? unitsMaxLength : 0);
		if (i > 0) {
			count += 3;
		}
	}
	return "\n " + string(count, '-');
}
string UI::Title(vector<std::string> subsections) {
	string text = "\n [ "+ rootTitle;
	for (string s:subsections) {
		if (s.compare("")) {
			text += " => " + s;
		}
	}
	text += " ]";
	return text;
}
string UI::option_YesNo() {
	UI::optionList = { "Yes", "No", "Exit" };
	UI::optionKeyList = { InputManager::yesKey,InputManager::noKey, InputManager::exitKey };
	return GenerateOptionsList();
}
string UI::option_Single() {
	UI::optionList = { "Back" };
	UI::optionKeyList = { InputManager::exitKey };
	return GenerateOptionsList();
}