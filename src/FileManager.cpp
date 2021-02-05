#include "FileManager.h"
using namespace std;


const char FileManager::delimiter = '\n'; //Define static string in implementation file
string FileManager::filePath = "";

//Methods
string FileManager::SelectFile(const bool newProfile, const bool& keep) {
	OPENFILENAME ofn;
	char buffer[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrTitle = newProfile ? "Select File Path" : "Select Profile";
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = (LPSTR) "";
	static const DWORD loadFlags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_DONTADDTORECENT | OFN_NODEREFERENCELINKS;
	static const DWORD saveflags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_DONTADDTORECENT | OFN_NODEREFERENCELINKS;
	ofn.Flags = newProfile ? saveflags : loadFlags;
	ofn.lpstrDefExt = "";
	string filePathStr;
	if (newProfile ? GetSaveFileName(&ofn) : GetOpenFileName(&ofn)) {
		filePathStr = buffer;
	}
	fstream file(filePathStr.c_str(), newProfile ? ios::out : ios::in);
	if (keep) {
		//Store the file path in the FileManager class
		if (file.is_open()) {
			filePath = filePathStr;
		}
	}
	file.close();
	return filePathStr;
}
ifstream FileManager::ReadFile(const string& path) {
	ifstream file(path);
	return file;
}
const bool FileManager::UpdateFile(const string& path, const string& data) {
	ofstream file(path, ios::out | ios::trunc);
	if (file.fail()) {
		return false;
	}
	file << data;
	file.close();
	return true;
}
const bool FileManager::DeleteFile(const string& path) {
	if (remove(path.c_str()) != 0) {
		return true;
	}
	else {
		return false;
	}
}
void FileManager::SetFilePath(const string& path) {
	filePath = path;
}
string FileManager::GetFilePath() {
	return filePath;
}