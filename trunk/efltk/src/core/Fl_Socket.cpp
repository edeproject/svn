#include <efltk/Fl_Socket.h>
#include <efltk/Fl_Exception.h>

#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#else
typedef int socklen_t;
#endif

int 	Fl_Socket::m_socketCount;
bool	Fl_Socket::m_inited = false;

void Fl_Socket::init() {
   if (m_inited) return;
   m_inited =  true;
#ifdef _WIN32
   // This does what we need here :)
   extern void fl_private_init(); //Fl_Init.cpp
   fl_private_init();
#endif
}

void Fl_Socket::cleanup() {
   m_inited =  false;
}

// Constructor
Fl_Socket::Fl_Socket(int domain,int type,int protocol) {
   init();

   m_receiveLowWaterMark = 2048;	// Default for TCP/UDP
   m_socketCount++;

   m_sockfd   = INVALID_SOCKET;
   m_domain   = domain;
   m_type     = type;
   m_protocol = protocol;
   m_host     = NULL;
   m_port     = 0;

   FD_ZERO(&inputs);
   FD_ZERO(&outputs);
}

// Destructor
Fl_Socket::~Fl_Socket() {
   close();
   m_socketCount--;
   if (!m_socketCount)
      cleanup();
}

int Fl_Socket::control(int flag, unsigned long *check)
{
#ifdef _WIN32
   return ioctlsocket(m_sockfd, flag, check);
#else
   return fcntl(m_sockfd, flag, *check);
#endif
}

void Fl_Socket::host(Fl_String hostName) {
   m_host = hostName;
}

void Fl_Socket::port(int portNumber) {
   m_port = portNumber;
}

// Connect & disconnect
void Fl_Socket::open(Fl_String hostName,int portNumber) {
   if (hostName.length())
      host(hostName);
   if (portNumber)
      port(portNumber);

   sockaddr_in addr;

   struct hostent *host_info;

   if ( active() ) close();

   // Creat a new socket
   m_sockfd = socket(m_domain, m_type, m_protocol);
   if (m_sockfd == INVALID_SOCKET)
      fl_throw("Can't create a new socket");

   host_info = gethostbyname(m_host);
   if (!host_info) {
      close();
      fl_throw("Can't connect. Host is unknown.");
   }

   memset(&addr, 0, sizeof(addr));
   addr.sin_family = m_domain;
   memcpy(&addr.sin_addr, host_info->h_addr, host_info->h_length);
   addr.sin_port = htons(m_port);

   int rc = connect( m_sockfd, (struct sockaddr *)&addr,sizeof( addr));
   if (rc) {
      close();
      fl_throw("Can't connect. Host is unreachible.");
   }

   FD_SET(m_sockfd, &inputs);
   FD_SET(m_sockfd, &outputs);

#ifdef _WIN32
   bool optval = true;
   setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &optval, sizeof(bool));
#endif

   socklen_t len = sizeof(socklen_t);
   getsockopt(m_sockfd, SOL_SOCKET, SO_RCVLOWAT, (char *) &m_receiveLowWaterMark, &len);
   //printf("Default receive low watermark is %i\n", m_receiveLowWaterMark);
}

void Fl_Socket::close() {
   if (m_sockfd != INVALID_SOCKET) {
      FD_CLR(m_sockfd,&inputs);
      FD_CLR(m_sockfd,&outputs);
#ifndef _WIN32
      shutdown(m_sockfd, SHUT_RDWR);
#else
      closesocket(m_sockfd);
#endif
      m_sockfd = INVALID_SOCKET;
   }
}

// Read & write
int  Fl_Socket::read(char *buffer,int size) {
   int 		bytes;
   int		total = 0;
   char 		*p = buffer;
   while (size > 0) {
#ifdef _WIN32
      bytes = recv(m_sockfd, p, size, NULL);
#else
      bytes = ::read(m_sockfd, p, size);
#endif
      if (bytes <= 0)
         break;
      size -= bytes;
      p += bytes;
      total += bytes;
   }
   return total;
}

int  Fl_Socket::read(Fl_Buffer& buffer) {
   int rc = read(buffer.data(),buffer.size());
   buffer.bytes(rc);
   return rc;
}

int  Fl_Socket::write(const char *buffer,int size) {
   int 	bytes;
   const char *p = buffer;
   int	total = size;
   while (size > 0) {
#ifdef _WIN32
      bytes = send(m_sockfd, p, size, NULL);
#else
      bytes = ::write(m_sockfd, p, size);
#endif
      size -= bytes;
      p += bytes;
   }
   return total;
}

int  Fl_Socket::write(const Fl_Buffer& buffer) {
   char *ptr = buffer.data();
   int	bytes = buffer.bytes();
   while (bytes > 0) {
      int rc = write(ptr,bytes);
      bytes -= rc;
      ptr += rc;
   }
   return buffer.bytes();
}

bool Fl_Socket::ready_to_read(int wait_msec) {
   struct timeval timeout;
   timeout.tv_sec = wait_msec / 1000;
   timeout.tv_usec = wait_msec % 1000 * 1000;

   FD_ZERO(&inputs);
   FD_SET(m_sockfd,&inputs);

   select(FD_SETSIZE, &inputs, NULL, NULL, &timeout);
   return (FD_ISSET(m_sockfd ,&inputs)>0);
}

bool Fl_Socket::ready_to_write() {
   return true;
}

#ifdef _WIN32
# define VALUE_TYPE(val) (char*)(val)
#else
# define VALUE_TYPE(val) (void*)(val)
#endif

int Fl_Socket::set_option(int level,int option,int  value) {
   socklen_t len = sizeof(socklen_t);
   return setsockopt(m_sockfd, level, option, VALUE_TYPE(&value), len);
}

int Fl_Socket::get_option(int level,int option,int& value) {
   socklen_t len = sizeof(socklen_t);
   return getsockopt(m_sockfd, level, option, VALUE_TYPE(&value), &len);
}
