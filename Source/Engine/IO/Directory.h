
#ifndef directory_h
#define directory_h

#include <eternity.h>
#include <Core/GigaObject.h>

GIGA_CLASS()
class GIGA_API Directory : public GigaObject {
public:
	Directory() = default;
	~Directory() = default;
    
    GIGA_CLASS_NAME("Directory");

	void Open(std::string path);
    
    /**
     * Get current working directory of application
     */
    GIGA_FUNCTION() static std::string GetCurrentDirectory();

protected:
	std::string path;
	std::vector<std::string> files;
	std::vector<Directory*> subdirectories;
};

#endif
