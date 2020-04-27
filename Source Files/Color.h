#ifndef COLOR_H
#define COLOR_H
#include <iostream>
#include <windows.h>

namespace Color {
	enum class ColorType {
		DEFAULT = 7, //The default color is a dimmer white.
		WHITE = 15, //Brighter white.
		TURQUOISE = 11,
		GREEN = 10,
		RED = 12,
		PINK = 13,
		YELLOW = 14,
	};
	static ColorType defaultColor = ColorType::WHITE;
	static void ChangeColor(ColorType color) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console, static_cast<int>(color));
	}
	static void ShowAllColors() {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		for (int i = 1; i < 255; i++)
		{
			SetConsoleTextAttribute(console, i);
			std::cout << i << "It's a  colorful world.\n";
		}
		SetConsoleTextAttribute(console, static_cast<int>(ColorType::DEFAULT));
	}
}
#endif