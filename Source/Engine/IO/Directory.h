
#ifndef directory_h
#define directory_h

#include <eternity.h>

class GIGA_API Directory {
public:
	Directory() = default;
	~Directory() = default;

	void Open(std::string path);

protected:
	std::string path;
	std::vector<std::string> files;
	std::vector<Directory*> subdirectories;
};

#endif