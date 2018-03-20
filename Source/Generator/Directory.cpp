
#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#include <assert.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <string>
#include <vector>
#include "Directory.h"

std::string Directory::GetCurrent() {
	std::string str;
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
		assert(false);
		return(str);
	}

	str = cCurrentPath;
	return(str);
}

void Directory::Open(std::string path) {
	this->path = path;
	std::string searchPath = path;

#ifdef WIN32
	WIN32_FIND_DATA ffd;
	HANDLE handle = INVALID_HANDLE_VALUE;
	searchPath += "\\*";

	handle = FindFirstFile(searchPath.c_str(), &ffd);
	bool hasFile = true;
	while (hasFile != false) {
		std::string filename = ffd.cFileName;
		if (filename.compare(".") == 0 || filename.compare("..") == 0) {
			hasFile = FindNextFile(handle, &ffd);
			continue;
		}
		
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			Directory* subdir = new Directory();
			subdir->Open(path + "/" + filename);

			subdirectories.push_back(subdir);
		}
		else {
			files.push_back(filename);
		}

		hasFile = FindNextFile(handle, &ffd);
	}

	FindClose(handle);
#else
#endif
}