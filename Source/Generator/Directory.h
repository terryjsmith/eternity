
#ifndef directory_h
#define directory_h

class Directory {
public:
	Directory() = default;
	~Directory() = default;

	void Open(std::string path);

	/**
	 * Get current working directory of application
	 */
	static std::string GetCurrent();

public:
	std::string path;
	std::vector<std::string> files;
	std::vector<Directory*> subdirectories;
};

#endif