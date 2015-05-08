#ifndef		SERVERSOCKET__H 
#define		SERVERSOCKET__H 

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <fcntl.h>
#include<ws2spi.h>// ¿Ô√Ê#include<winsock2.h>
#pragma comment(lib,"wsock32.lib") 


using namespace std;

class ServerSocket 
{ 
	protected: 
		WSADATA WSAData;
		int sin_size; 
 		int sockfd, new_fd, socksend; 
 		unsigned short port; 
 		struct sockaddr_in my_addr, their_addr, get_addr; 
  
 	public: 
 		ServerSocket(); 
 		virtual ~ServerSocket(); 
 		virtual int Socket(); 
 		void SetPort(unsigned short in_port); 
 		bool Bind(); 
 		void SetRemotePort(unsigned short in_port); 
		unsigned short GetRemotePort();  
 		bool Connect(); 
 		void Listen(); 
 		void Accept(); 
 		int Send(const char *buf, int len); 
 		int Sendto(const char *buf, int len); 
 		int Recv(char *buf, int len); 
 		int Recvfrom(char *buf, int len); 
 		void Close();  
 		 
}; 

 class TCPserver:public ServerSocket
 {
	public:
		 int Socket(); 
 };
 class UDPserver:public ServerSocket
 {
	public:
		 int Socket(); 		
 };


#endif 


