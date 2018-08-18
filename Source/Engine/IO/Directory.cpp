
#include <IO/Directory.h>
#include <Core/Error.h>

#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

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

std::string Directory::GetCurrentDirectory() {
    std::string str;
    char cCurrentPath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to get current working directory."));
        return(str);
    }
    
    str = cCurrentPath;
    return(str);
}
