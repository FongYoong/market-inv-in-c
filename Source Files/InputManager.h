#ifndef InputManager_H
#define InputManager_H
#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <limits>
#include <algorithm>
#include "UI.h"
#define PAUSE system("Pause")
#define CLEAR system("CLS")

enum class InputErrorType {
	GENERAL, EMPTY_INPUT, CHAR, NUMBER, INTEGER, DOUBLE, ZERO, POSITIVE, ITEM_NOT_FOUND, ITEM_EXISTS, EMPTY_INVENTORY,
	DISTRIBUTION, PROFILE_NEW, PROFILE_LOAD, PROFILE_UPDATE, PROFILE_DELETE,
};
class InputManager{
public:
	//Defined in implementation file
	static const std::string yesKey;
	static const std::string noKey;
	static const std::string exitKey;
	//Input validation
	static bool IsInteger(const std::string&);
	static bool IsDouble(const std::string&);
	//Input
	//All functions return false when exitKey is used
	static const bool GetString(std::string& placeholder, const bool& showPrevious, const std::string& title, const std::string& name, const std::string& custom);
	static const bool GetChar(char& placeholder, const bool& showPrevious, const std::string& title, const std::string& name, const std::string& custom);
	static const bool GetInteger(int& placeholder, const bool& allowZero, const bool& allowNegative, const bool& showPrevious, const std::string& title, const std ::string& name, const std::string& custom);
	static const bool GetDouble(double& placeholder, const bool& allowZero, const bool& allowNegative, const bool& showPrevious, const std::string& title, const std::string& name, const std::string& custom);
	static const bool GetBool(bool& placeholder, const bool& showPrevious, const std::string& title, const std::string& name, const std::string& custom);
	//Misc
	static constexpr DWORD sleepInterval = 1000;
	static constexpr int inputSize = 30;
	static void Flush(std::istream&);
	static bool CheckInputError();
	static void InputError(InputErrorType type);
	static void RemoveWhiteSpace(std::string&);
	static void ToLowerCase(std::string&);
};
#endif
