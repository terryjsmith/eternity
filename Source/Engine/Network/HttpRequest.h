
#ifndef httprequest_h
#define httprequest_h

#include <eternity.h>
#include <Core/GigaObject.h>

#include <curl/curl.h>

/**
 * An HTTP(s) call using the CURL library
 */
GIGA_CLASS()
class HttpRequest : public GigaObject {
public:
    HttpRequest();
    HttpRequest(std::string url);
    ~HttpRequest();
    
    GIGA_CLASS_NAME("HttpRequest");
    
    // Create a new request
    GIGA_FUNCTION() void Create(std::string url);
    
    // Add POST data
    GIGA_FUNCTION() void AddPostData(std::string name, std::string value);
    
    // Read the HTTP response
    static size_t _ProcessResponse(char *buffer, size_t size, size_t nitems, void *userdata);
    void SaveResponse(char *buffer, unsigned int size);
    
    // Execute the request
    GIGA_FUNCTION() std::string Execute();
    
protected:
    // Whether the CURL system is initialized
    static bool m_initialized;
    
    // Store the response
    unsigned char* m_response;
    unsigned int m_bytes;
    
    // A CURL handle
    CURL* m_handle;
    
    // Post data
    std::map<std::string, std::string> m_vars;
    
    // Response HTTP code
    long m_responseCode;
    
    // The URL we are trying to access
    std::string m_url;
};

#endif
