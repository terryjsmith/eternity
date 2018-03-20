
#include <IO/Directory.h>

void Directory::Open(std::string path) {
	this->path = path;
	std::string searchPath = path;

#ifdef WIN32
	WIN32_FIND_DATA ffd;
	HANDLE handle = INVALID_HANDLE_VALUE;
	searchPath += "\\*";

	handle = FindFirstFile(searchPath.c_str(), &ffd);
	while (handle != 0) {
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			Directory* subdir = new Directory();
			subdir->Open(path + "/" + ffd.cFileName);

			subdirectories.push_back(subdir);
		}
		else {
			files.push_back(ffd.cFileName);
		}

		FindNextFile(handle, &ffd);
	}

	FindClose(handle);
#else
#endif
}