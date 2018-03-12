
#ifndef resource_h
#define resource_h

#include <eternity.h>

/**
* File opening modes (can/should be OR'd together)
*/
enum {
	FILEMODE_READ = 1,
	FILEMODE_READWRITE = 2,
	FILEMODE_BINARY = 4,
	FILEMODE_APPEND = 8,
};

class GIGA_API Resource {
public:
	Resource();
	~Resource();

	/**
	 * Set filename
	 */
	void Initialize(std::string filename, int mode);

	/**
	 * Load/unload
	 */
	void Load();
	void Unload();

	/**
	 * Get raw data
	 */
	unsigned char* GetData();

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

public:
	// Just the filename + extension
	std::string filename;

	// File extension
	std::string extension;

	// Full path including filename
	std::string fullPath;

	// The size of the file/data
	unsigned int filesize;

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