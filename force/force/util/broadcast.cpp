#include "broadcast.hpp"
#include "bd_tool.hpp"
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
const int PORT = 7774;
std::string local_ip;
void bd_so::BroadcastCenter::startSend(std::string msg,std::string &server_ip) {
	this->is_casting = true;
	strcpy(buf,msg.c_str());
	sendto(socket_fd,buf,strlen(buf),0,(struct sockaddr *)&my_addr,sizeof(my_addr));
	bzero(buf,MAXDATASIZE);
	sockaddr_in server_addr;
	socklen_t len = 0;
	recvfrom(socket_fd,buf,MAXDATASIZE,0,(struct sockaddr*)&server_addr,&len);
	server_ip = std::string(buf);
	printf("server:ip:%s",server_ip.c_str());
	this->is_casting = false;
}

void bd_so::BroadcastCenter::init_addr() {
	my_addr.sin_family = AF_INET;
	if(this->is_sender)
		my_addr.sin_port = htons(PORT); 
	else 
		my_addr.sin_port = htons(0);
	char *ip = (char *)malloc(20);
	bzero(ip,20);
	std::string bd_address = boardcast_addr(ip);
	local_ip = std::string(ip);
	my_addr.sin_addr.s_addr = inet_addr(bd_address.c_str());
	
	user_addr.sin_family = AF_INET;
	if(this->is_sender)
		user_addr.sin_port = htons(0); 
	else 
		user_addr.sin_port = htons(PORT);
	user_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	socket_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(-1 == socket_fd) {
		perror("socket init failed");
		exit(1);
	}
	setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));
	if(!this->is_sender) {
		if((bind(socket_fd,(struct sockaddr *)&user_addr,sizeof(struct sockaddr)))==-1){
			perror("bind failed");
			exit(1);
		}
	}
}


void bd_so::startReceiving(void *) {
	bd_so::BroadcastCenter *center = new bd_so::BroadcastCenter(false);
	center->is_receiving = true;
	socklen_t size = sizeof(center->user_addr);
	recvfrom(center->socket_fd,center->buf,MAXDATASIZE,0,(struct sockaddr *)&(center->user_addr),&size);
	strcpy(center->my_ip,inet_ntoa(center->user_addr.sin_addr));
	bzero(center->buf,MAXDATASIZE);
	strcpy(center->buf,local_ip.c_str());
	sendto(center->socket_fd,center->buf,strlen(center->buf),0,(struct sockaddr *)&center->user_addr,sizeof(sockaddr_in));
}

void bd_so::BroadcastCenter::start_listen_thread(void) {
	pthread_t listen_th;
	void (*func)(void *) = bd_so::startReceiving;
	if(-1 == pthread_create(&listen_th,NULL,(void* (*)(void *))func,(void *)NULL)) {
		perror("listen thread init failed");
		exit(1);
	}
	int *result = new int;
	pthread_join(listen_th,(void **)&result);
}
