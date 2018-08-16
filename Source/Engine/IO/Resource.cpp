
#include <IO/Resource.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>

Resource::Resource() {
	m_data = 0;
	m_loaded = false;
	filesize = 0;
	m_fp = 0;
	m_eof = false;
	m_mode = 0;
}

Resource::~Resource() {
	if (m_data) {
		free(m_data);
		m_data = 0;
	}

	if (m_fp) {
		fclose(m_fp);
		m_fp = 0;
	}
}

unsigned char* Resource::GetData() {
	if (m_loaded == false) {
		Load();
	}

	return(m_data);
}

std::string Resource::GetString() {
	if (m_loaded == false) {
		Load();
	}

	std::string retval = (char*)m_data;
	retval.resize(filesize);

	return(retval);
}

void Resource::Initialize(std::string filename, int mode) {
	// If it's already open, close
	if (m_fp) {
		fclose(m_fp);
		m_fp = 0;
	}

	// Save file mode
	m_mode = mode;

	// Save filename, extension, etc.
	this->fullPath = filename;

	// Find just the filename
	size_t pos = filename.find_last_of("\\/");
	if (pos == std::string::npos) {
		this->filename = filename;
	}
	else {
		this->filename = filename.substr(pos, filename.length() - pos);
	}

	// Also save extension
	pos = filename.find_last_of(".");
	this->extension = filename.substr(pos + 1, filename.length() - pos - 1);

	// Use flags to set open mode
	std::string flags = "";
	if (mode & FILEMODE_READWRITE) {
		flags += "w";
	}
	if (mode & FILEMODE_READ) {
		flags += "r";
	}
	if (mode & FILEMODE_APPEND) {
		flags += "a";
	}
	if (mode & FILEMODE_BINARY) {
		flags += "b";
	}

	// Open the file
	m_fp = fopen(filename.c_str(), flags.c_str());
	if (m_fp == 0) {
		// Error
		return;
	}

	// Save filesize
	fseek(m_fp, 0, SEEK_END);
	this->filesize = ftell(m_fp);
	fseek(m_fp, 0, SEEK_SET);

	m_loaded = false;
}

void Resource::Load() {
	if (m_loaded == true) {
		return;
	}

	m_data = (unsigned char*)malloc(filesize);

	fseek(m_fp, 0, SEEK_SET);
	fread(m_data, filesize, 1, m_fp);
	m_loaded = true;
	m_eof = true;
}

void Resource::Unload() {
	if (m_loaded) {
		free(m_data);
		m_data = 0;
	}

	m_loaded = false;
}

bool Resource::Exists(std::string filename) {
	FILE* fp = 0;
	fp = fopen(filename.c_str(), "rb");

	if (fp == 0) {
		return(false);
	}

	fclose(fp);

	return(true);
}

bool Resource::EndOfFile() {
	return(m_eof);
}

void Resource::Close() {
	if (m_fp) {
		fclose(m_fp);
		m_fp = 0;
	}
}

void Resource::Write(unsigned char *data, unsigned long bytes) {
	if (m_fp == 0) {
		return;
	}

	unsigned int written = (unsigned int)fwrite(data, 1, bytes, m_fp);
	fflush(m_fp);
}

void Resource::WriteLine(std::string line) {
	if (m_fp == 0) {
		return;
	}

	Write((unsigned char*)line.c_str(), (unsigned long)line.length());
	Write((unsigned char*)"\n", 1);
}

unsigned char* Resource::Read(unsigned long bytes) {
	if (m_fp == 0) {
		return(0);
	}

	unsigned char* ret = (unsigned char*)malloc(bytes + (m_mode & FILEMODE_BINARY ? 0 : 1));
	size_t actual = fread(ret, 1, bytes, m_fp);

	if (actual == 0) {
		free(ret);
		m_eof = true;
		return(0);
	}

	if ((m_mode & FILEMODE_BINARY) == false) {
		ret[actual] = '\0';
	}

	return(ret);
}

void Resource::Read(unsigned char* dest, unsigned long bytes) {
	if (m_fp == 0) {
		return;
	}

	unsigned long read = (unsigned int)fread(dest, 1, bytes, m_fp);

	if (read == 0) {
		m_eof = true;
	}

	return;
}

unsigned char* Resource::ReadFile() {
	if (m_fp == 0) {
		return(0);
	}

	return(Read(this->filesize));
}

void Resource::SetPosition(unsigned int offset) {
	fseek(m_fp, SEEK_SET, offset);
}
