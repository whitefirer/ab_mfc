#include "stdafx.h"
#include "serversocket.h"

ServerSocket::ServerSocket()
{   
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0) //³õÊ¼»¯Ì×½Ó×Ö
	{
		printf("sock init fail!\n");
	}	
	else printf("sock init success!\n");
}  

ServerSocket::~ServerSocket()
{   
        
  Close();   
  closesocket(sockfd);   
  WSACleanup();

}   

int UDPserver::Socket()
{   	

     sockfd = socket(AF_INET , SOCK_DGRAM, 0);  
	 
     return sockfd;     
}   

int TCPserver::Socket()
{
     sockfd = socket(AF_INET, SOCK_STREAM, 0);  

     return sockfd;   
}   

int ServerSocket::Socket()
{    
     sockfd = socket(AF_INET, SOCK_STREAM, 0); 

     return sockfd;   
}   
   
void ServerSocket::SetPort(unsigned short in_port)
{   
	struct hostent *host;
	char strIP[128];

	gethostname(strIP, 128);
	host = gethostbyname(strIP);
	my_addr.sin_family = AF_INET;   
 	port = in_port;   
  	my_addr.sin_port = htons(port);   
    my_addr.sin_addr.S_un.S_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
} 

void ServerSocket::SetRemotePort(unsigned short in_port)
{   
        
     their_addr.sin_port = htons(in_port);   
       
}   
    
 unsigned short ServerSocket::GetRemotePort(){   
        
   return ntohs(get_addr.sin_port);   
        
 }   
  
 bool ServerSocket::Bind()
 {   
	if(bind(sockfd, (struct sockaddr FAR *)&my_addr, sizeof(my_addr)) == SOCKET_ERROR)  
	{
		perror("bind error!\n");
		return false;   
	}
  
	return true;    
 }   
   
 int ServerSocket::Recvfrom(char *buf,int len)
 {   
	int numbytes;   
	int addr_len = sizeof(get_addr);   

	numbytes = recvfrom(sockfd, buf, len, 0, (struct sockaddr *)&get_addr, &addr_len);   
	if(numbytes == -1)   
	{   
		return false;   
	}  
	
	buf[numbytes] = '\0';   
   
	return numbytes;    
 }   

 int ServerSocket::Sendto(const char *buf,int len)
 {     
     return sendto(sockfd,buf, len, 0,(struct sockaddr *)&their_addr,sizeof(struct sockaddr));        
 }    
    
 void ServerSocket::Close()
 {   
	closesocket(new_fd);   
 }    
    
 void ServerSocket::Listen()
 {   
        
   if(listen(sockfd, 10)== -1 )
		perror("listen err!\n"); 
 }   
    
void ServerSocket::Accept()
{   
	int size = sizeof(struct sockaddr_in);   
	new_fd = accept(sockfd, (struct sockaddr *)&get_addr, &size);   

	if(new_fd==-1)
	{
		perror("accept err!\n");
		exit(1);
	}
 }   
    
 int ServerSocket::Recv(char *buf,int len)
 {   
	return recv(new_fd, buf, len, 0);        
 }   
    
 int ServerSocket::Send(const char * buf, int len)
 {   
     return send(new_fd, buf, len, 0);   
 }   
    
 bool ServerSocket::Connect()
 {        
	if(connect(sockfd,(struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == false)   
		return false;
	
	return true;
 }   
