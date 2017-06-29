#include <sys/socket.h>
#include <ifaddrs.h>
#include <cstdio>
#include <arpa/inet.h>
#include <string>
#include <cstring>
std::string boardcast_addr(char *ip){
	struct ifaddrs *ifap;
	int success = getifaddrs(&ifap);
	if(0 == success) {
		while(ifap!=NULL) {
			std::string ifaname(ifap->ifa_name);
			struct sockaddr_in *sockaddr = (struct sockaddr_in*)ifap->ifa_addr;
			if(ifaname == "en0" &&  AF_INET == sockaddr->sin_family) {
				struct sockaddr_in *broadcastaddr= (struct sockaddr_in*)ifap->ifa_dstaddr;
				char *broadcast_ads = inet_ntoa(broadcastaddr->sin_addr);
				struct sockaddr_in *localaddr = (struct sockaddr_in *)ifap->ifa_addr;	
				char *local_ads = inet_ntoa(localaddr->sin_addr);
				memcpy(ip,local_ads,strlen(local_ads));
				return std::string(broadcast_ads);
			}
			ifap = ifap->ifa_next;
		}
	}else {
		printf("get address failed");	
	}
	return "";
}
	
