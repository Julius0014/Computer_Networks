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
// Julius Jenkins
// create socket and return socket file desciptor
int createSocket(void){
    int socketnum;
    if ( (socketnum = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    return socketnum;
}
int recvMsg(int fd){
    char buffer[MAXLINE];
    int bytes = 99;
    if(recv(fd,buffer,MAXLINE,0) < 0){
        perror("RECV ERROR");
        close(fd);
        exit(-1);
    }
    printf("MESSAGE Receive:\n %s\n",buffer);
    fflush(stdout);
    close(fd);
}
struct sockaddr_in MEInfo(struct sockaddr_in server){
    server.sin_port = htons(0); 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    return server;
}
//Fill server information
struct sockaddr_in fillServerInfo(struct sockaddr_in server,char ipaddress[100],int port){
    server.sin_family = AF_INET;
    server.sin_port = htons(port); 
    //set the server address with ip address as a string 
    server.sin_addr.s_addr = inet_addr(ipaddress);
    return server;
}
//send message to server 
int sendMsg(char msg[100],struct sockaddr_in servaddr,int sockfd){
    if (send(sockfd,msg, strlen(msg),0) < 0){
        perror("ERROR SENDING MESSAGE:");
        exit(-1);
    }else{
        printf("MESSAGE SENT\n");
    }
    return 0;
}
//get info from stardard in
char getinfo(char msg[100],char *buffer){
    printf("%s\n",msg);
    if(fgets(buffer,MAXLINE,stdin)){
        return;
    }else{
        perror("READ ERROR\n");
        exit(-1);
    }
    return;
}
int bindandConnect(struct sockaddr_in me,struct sockaddr_in servaddr, int sockfd ){
    if ( bind(sockfd,(const struct sockaddr *)& me,sizeof(me)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if ( connect(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
    {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}
int main(int argc,char *argv[]) {
    int sockfd; 
    struct sockaddr_in servaddr,me; 
    sockfd = createSocket();
    char ipaddr[100],port[100],msg[100];
    getinfo("Enter Server Address",ipaddr);
    getinfo("Enter Server Port",port);
    getinfo("Enter Message to Server",msg);
    me = MEInfo(me);
    servaddr = fillServerInfo(servaddr,ipaddr,atoi(port));
    bindandConnect(me,servaddr,sockfd);
    sendMsg(msg,servaddr,sockfd);
    close(sockfd); 
    return 0; 
} 

