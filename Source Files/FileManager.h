#ifndef FileManager_H
#define FileManager_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>

#ifdef  DeleteFile
#undef DeleteFile
#endif //  DeleteFile

class FileManager {
public:
	static const char delimiter; //Delimiter to differentiate between distinct data fields
	//Public Methods
	static std::string SelectFile(const bool newProfile, const bool& keep);
	static std::ifstream ReadFile(const std::string& path);
	static const bool UpdateFile(const std::string& path, const std::string& data);
	static const bool DeleteFile(const std::string& path);
	static void SetFilePath(const std::string& path);
	static std::string GetFilePath();
private:
	static std::string filePath;
};
#endif