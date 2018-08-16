

#ifdef WIN32

#include <Network/UDPSocketWin32.h>
#include <Core/Error.h>
#include <WinSock2.h>

UDPSocketWin32::UDPSocketWin32() {
    memset(&m_sockaddr, 0, sizeof(sockaddr_in));
}

UDPSocketWin32::~UDPSocketWin32() {
    if (m_socket) {
        closesocket(m_socket);
        m_socket = 0;
    }
}

bool UDPSocketWin32::Connect(std::string server, int port) {
    WSADATA wsaData;
    
    // Startup winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // Create a socket
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket < 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, (char*)"Unable to create socket for server", server));
        return(false);
    }

	// Get the IP address
	struct addrinfo hint;
	struct addrinfo *res;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_DGRAM;

	// Create our server structure
	memset(&m_sockaddr, 0, sizeof(sockaddr_in));
	m_sockaddr.sin_family = AF_INET;

	bool uses_ip = false;
	if (getaddrinfo(server.c_str(), "", &hint, &res)) {
		// Check for IP address
		if (inet_pton(AF_INET, server.c_str(), &m_sockaddr.sin_addr) == 0) {
			GIGA_ASSERT(false, "Connect failed.");
			Message::Broadcast(new Error(Error::ERROR_WARN, (char*)"Unable to get host", server));
			return(false);
		}
		
		uses_ip = true;
	}
    
	if (uses_ip == false) {
		memcpy(&m_sockaddr, res->ai_addr, res->ai_addrlen);
	}
    m_sockaddr.sin_port = htons(port);
    
    // Connect
    if (::connect(m_socket, (struct sockaddr*)&m_sockaddr, sizeof(m_sockaddr)) < 0) {
        Message::Broadcast(new Error(Error::ERROR_WARN, (char*)"Unable to connect to host", server));
        return(false);
    }
    
    unsigned long mode = 1;
    ioctlsocket(m_socket, FIONBIO, &mode);
    
    return(true);
}

int UDPSocketWin32::Write(unsigned char* buffer, int length) {
    int ret = ::send(m_socket, (char*)buffer, length, 0);
    return(ret);
}

int UDPSocketWin32::Read(void* buffer, int length) {
    int ret = ::recv(m_socket, (char*)buffer, length, 0);
    return(ret);
}

void UDPSocketWin32::Close() {
    if (m_socket) {
        closesocket(m_socket);
        m_socket = 0;
    }
}

#endif
