
#include <Network/HttpRequest.h>
#include <Core/Error.h>
#include <IO/Directory.h>

bool HttpRequest::m_initialized = false;

HttpRequest::HttpRequest() {
    m_response = 0;
    m_handle = 0;
    m_bytes = 0;
    m_responseCode = 0;
}

HttpRequest::~HttpRequest() {
    if (m_handle) {
        curl_easy_cleanup(m_handle);
        m_handle = 0;
    }
    
    if (m_response) {
        free(m_response);
        m_response = 0;
    }
}

HttpRequest::HttpRequest(std::string url) {
    m_response = 0;
    m_handle = 0;
    m_bytes = 0;
    m_responseCode = 0;
    
    this->Create(url);
}

void HttpRequest::Create(std::string url) {
    if (HttpRequest::m_initialized == false) {
        // Initialize the CURL library
        curl_global_init(CURL_GLOBAL_ALL);
    }
    
    m_url = url;
    
    m_handle = curl_easy_init();
    curl_easy_setopt(m_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, _ProcessResponse);
    
    HttpRequest::m_initialized = true;
}

std::string HttpRequest::Execute() {
    assert(m_handle != 0);
    
    // Concatenate POST variables before we send
    std::string postvars;
    for (std::map<std::string, std::string>::iterator i = m_vars.begin(); i != m_vars.end(); i++) {
        postvars += "&" + i->first + "=" + curl_easy_escape(m_handle, i->second.c_str(), (int)i->second.length());
    }
    
    std::string cadir = Directory::GetCurrentDirectory() + "/cacert.pem";
    
    curl_easy_setopt(m_handle, CURLOPT_POSTFIELDS, postvars.c_str());
    curl_easy_setopt(m_handle, CURLOPT_CAINFO, cadir.c_str());
    
    // Execute
    int success = curl_easy_perform(m_handle);
    if (success != CURLE_OK) {
        Message::Broadcast(new Error(Error::ERROR_WARN, "Unable to perform HTTP request", m_url));
        return(0);
    }
    
    // Get the info we want out
    curl_easy_getinfo(m_handle, CURLINFO_RESPONSE_CODE, &m_responseCode);
    
    // Then clean up
    curl_easy_cleanup(m_handle);
    m_handle = 0;
    
    // Return response
    std::string response = (char*)m_response;
    return(response);
}

size_t HttpRequest::_ProcessResponse(char *buffer, size_t size, size_t nitems, void *userdata) {
    HttpRequest* obj = (HttpRequest*)userdata;
    obj->SaveResponse(buffer, (int)size * (int)nitems);
    return(size * nitems);
}

void HttpRequest::SaveResponse(char *buffer, unsigned int size) {
    // Copy new data into temp buffer, adding null byte to end
    unsigned char* tempbuffer = (unsigned char*)malloc(size + 1);
    memcpy(tempbuffer, buffer, size);
    tempbuffer[size] = '\0';
    
    // If we don't have a response stored, save and exit
    if (m_response == 0) {
        m_response = tempbuffer;
        m_bytes = size + 1;
        return;
    }
    
    // If we do have a response saved, copy it back
    unsigned char* old = m_response;
    m_response = (unsigned char*)malloc(size + m_bytes);
    memcpy(m_response, old, m_bytes - 1); // Don't copy the null byte from the last string
    memcpy(m_response + m_bytes, tempbuffer, size + 1);
    
    // Free memory
    free(tempbuffer);
    free(old);
    
    // Store new number of bytes
    m_bytes = m_bytes + size; // (m_bytes - 1 for old null byte, size + 1 for new null byte)
}

void HttpRequest::AddPostData(std::string name, std::string value) {
    m_vars[name] = value;
}
