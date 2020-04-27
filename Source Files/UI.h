#ifndef UI_H
#define UI_H
#include "InputManager.h"
#include <iostream>
#include <sstream>
#include <windows.h>
#include <string>
#include <vector>
#include "Color.h"
class UI {
public:
	static const std::ios_base::fmtflags originalCoutFormat;
	struct TableColumn {
		std::string title;
		//Number formatting
		int precision;
		int width;
		bool showPoint;
		bool isNumber;
		bool units;// Has measurement units // String is formatted first before displayed // Relevant functions do not alter the output/cout flags.
	};
	static bool showItems;
	static constexpr DWORD printInterval = 0;
	static constexpr int maxDots = 2; //Trailing dots if parameter exceeds output width
	static const std::string rootTitle;
	static const std::string inputMarker; //Indicates user input
	static std::vector<std::string> optionList, optionKeyList;
	//Prefixed table column settings
	static const std::vector<TableColumn> defaultColumns;
	//Public Methods
	static void PeriodicPrint(const std::string&);
	static void PeriodicPrint(const std::string& text, DWORD interval);
	static std::string FormatWidth(std::string& s, int width);
	static std::string GenerateOptionsList();
	//static std::string GenerateOptionsMiniList();
	static std::string GenerateTableRow(bool isTitle, std::vector<TableColumn> format, std::vector<std::string> values, std::vector<std::string> units, const int unitsMaxLength);
	static std::string GenerateTableBorder(std::vector<TableColumn> format, const int unitsMaxLength);
	static std::string Title(std::vector<std::string> subsections);
	static std::string option_YesNo(); //For binary options
	static std::string option_Single(); // For single input options
};

#endif