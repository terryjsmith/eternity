
#ifndef resource_h
#define resource_h

#include <eternity.h>
#include <Core/GigaObject.h>

/**
* File opening modes (can/should be OR'd together)
*/
enum {
	FILEMODE_READ = 1,
	FILEMODE_READWRITE = 2,
	FILEMODE_BINARY = 4,
	FILEMODE_APPEND = 8,
};

GIGA_CLASS()
class GIGA_API Resource : public GigaObject {
public:
	Resource();
	~Resource();

	GIGA_CLASS_NAME("Resource");
    

	/**
	 * Set filename
	 */
	void Initialize(std::string filename, int mode);

	/**
	 * Load/unload
	 */
	GIGA_FUNCTION() void Load();
	GIGA_FUNCTION() void Unload();

	/**
	 * Get raw data
	 */
	GIGA_FUNCTION() unsigned char* GetData();

	/**
	 * Get data as string
	 */
	std::string GetString();

	/**
	* Check whether a file exists
	*/
	static bool Exists(std::string filename);

	/**
	* Close the current file
	*/
	void Close();

	/**
	* Whether we have reached the end of the file yet
	*/
	bool EndOfFile();

	/**
	* Read from the file
	*/
	unsigned char* Read(unsigned long bytes);

	/**
	* Read from a file - dest must be initialized already
	*/
	void Read(unsigned char* dest, unsigned long bytes);

	/**
	* Write to file
	*/
	void Write(unsigned char* data, unsigned long bytes);

	/**
	* Write a single line (+ newline) to file
	*/
	void WriteLine(std::string line);

	/**
	* Get the entire contents of a file
	*/
	unsigned char* ReadFile();

	/**
	* Go to a specific position in the file
	*/
	void SetPosition(unsigned int offset);
    
    /**
     * Shortcut to load all file data (mostly for scripting)
     */
    GIGA_FUNCTION() static std::string Load(std::string filename);

public:
	// Just the filename + extension
    GIGA_VARIABLE(Get, Serialize) std::string filename;

	// File extension
	GIGA_VARIABLE(Get) std::string extension;

	// Full path including filename
	GIGA_VARIABLE(Get) std::string fullPath;

	// The size of the file/data
	GIGA_VARIABLE(Get) unsigned int filesize;

	// Loaded resource type
	GIGA_VARIABLE(Get, Serialize) std::string type;

protected:
	// Loaded data
	unsigned char* m_data;

	// Whether the data is loaded or now
	bool m_loaded;

	// File handle
	FILE* m_fp;

	// Whether we are at the end of the file
	bool m_eof;

	// File mode
	int m_mode;
};

#endif
