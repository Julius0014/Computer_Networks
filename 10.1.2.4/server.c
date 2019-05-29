//ubh226 Julius Jenklins
#include <string.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT     8080 
#define MAXLINE 1024

// create socket and return socket file desciptor
int createSocket(void){
    int socketnum;
    if ( (socketnum = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    return socketnum;
}
//Fill server information
struct sockaddr_in fillServerInfo(struct sockaddr_in server,char ipaddress[100],int port){
    server.sin_family = AF_INET; 
    server.sin_port = htons(port); 
    server.sin_addr.s_addr = inet_addr(ipaddress);
    return server;
}
//send messAGE TO SERVER
int sendMsg(int groupNum,struct sockaddr_in servaddr,int sockfd,int num){
    char msg[50];
    static numMsg = 1;
    if( num + 1 == numMsg){
        numMsg = 0;
        return;
    }
    sprintf(msg,"This is message %i from the Group %i beacon",numMsg++,groupNum);
    if (sendto(sockfd,msg, sizeof(msg),0,(const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("ERROR SENDING MESSAGE:");
        exit(-1);
    }else{
        printf("MESSAGE SENT\n");
    }
    sleep(1);
    return sendMsg(groupNum,servaddr,sockfd,num);
}
//server to socket;
int bindserver(struct sockaddr_in servaddr,int sockfd){
    unsigned char ttl = 1;
    if ( setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_TTL,(char*) &ttl,sizeof(ttl)) < 0 ) 
    {
        perror("sockopt"); 
        exit(EXIT_FAILURE); 
    }
    return 0;
}
//get info from stardard in
void checkArg(char *argv){
    if (argv == NULL)
    {
        printf("USAGE ERROR NEED GROUP NUMBER\n");
        exit(-1);
    }else if(isdigit(atoi(argv)) == 0){
        return ;
    }else{
        printf("USAGE ERROR NEED GROUP NUMBER\n");
        exit(-1);
    }
}
int main(int argc,char *argv[]) {
    int sockfd,port; 
    struct sockaddr_in   servaddr;
    char groupNum[3],ip[50];
    checkArg(argv[1]);
    strcpy(groupNum,argv[1]);
    strcpy(ip,"239.10.5.");
    port = atoi(groupNum) + 22200;
    strcat(ip,groupNum);
    servaddr = fillServerInfo(servaddr,ip,port);
    sockfd = createSocket();
    bindserver(servaddr,sockfd);
    printf("Listening on port %i\n",port);
    sendMsg(atoi(groupNum),servaddr,sockfd,10);
    close(sockfd); 
    return 0; 
} 

