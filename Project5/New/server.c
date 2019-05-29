//ubh226 Julius Jenklins
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT     8080 
#define MAXLINE 1024
typedef struct MSG{
    int id;
    char msg[4096];
} MSG;
struct sockaddr_in servaddr;
int sockfd;
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
int recvMsg(int fd){
    int buffer;
    struct sockaddr_in receiver;
    int size = sizeof(receiver);
    time_t now;
    time(&now);
    while (recvfrom(fd,&buffer,sizeof(buffer),0,(struct sockaddr *)&receiver,&size) > 0){
        if(buffer == -1){
            printf("MSG\n");
            return -1;
        }
    }
    return 1;
}
int sendFILE(char *file){
    int fd;
    char buf[MAXLINE];
    struct MSG x;
    x.id = 1;
    int line = 0,rline;

    fd = open(file,O_RDONLY);
    if(fd < 0){
        perror("FILE");
        exit(-1);
    }

    while(read(fd,&buf,MAXLINE) ){
        strcpy(x.msg,buf);
        rline = sendMsg(x);
        if (rline == -1){
            lseek(fd,0,SEEK_SET); 
            x.id = 0;
        }else{
        x.id++;
        memset(buf,0,sizeof(buf));
        }
    }
    x.id = -99;
    sendMsg(x);
}
    //send messAGE TO SERVER
    int sendMsg(struct MSG LineNum){
        int cLine;
        char msg[50];
        static numMsg = 1;
        char buffer[MAXLINE];
        numMsg++;
        if (sendto(sockfd,&LineNum, sizeof(LineNum),0,(const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
            perror("ERROR SENDING MESSAGE:");
            exit(-1);
        }else{

            cLine = recvMsg(sockfd);
        }
        return cLine;
    }
    //server to socket;
    int bindserver(struct sockaddr_in servaddr,int sockfd){
        unsigned char ttl = 60;
        int b = 1;
        if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &b, sizeof(b)) <  0)
        {
            perror(" 1 sockopt"); 
            exit(EXIT_FAILURE); 
        }
        if ( setsockopt(sockfd,IPPROTO_IP,IP_MULTICAST_TTL,(char*) &ttl,sizeof(ttl)) < 0 ) 
        {
            perror("sockopt"); 
            exit(EXIT_FAILURE); 
        }
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
            perror("Error");
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
        int port; 
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
        sendFILE(argv[2]);
        close(sockfd); 
        return 0; 
    } 

