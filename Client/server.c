//ubh226 Julius Jenklins
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
    server.sin_addr.s_addr = INADDR_ANY;
    return server;
}
// read message form client
int receiveMsg(int sockfd){
    char msg[MAXLINE];
    struct sockaddr_in client;
    int clen = sizeof(client);
    if (recvfrom(sockfd,msg,MAXLINE,0,(struct sockaddr *) &client,&clen) < 0){
        perror("ERROR SENDING MESSAGE:");
        exit(-1);
    }else{
        printf("MESSAGE RECEIVE: %s\n",msg);
    }
    return 0;
}
//send messAGE TO SERVER
int sendMsg(char msg[100],struct sockaddr_in servaddr,int sockfd){
    if (sendto(sockfd,msg, strlen(msg),0,(const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("ERROR SENDING MESSAGE:");
        exit(-1);
    }else{
        printf("MESSAGE SENT\n");
    }
    return 0;
}
//server to socket;
int bindserver(struct sockaddr_in servaddr,int sockfd){
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
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
}   

int main(int argc,char *argv[]) {
    int sockfd; 
    struct sockaddr_in   servaddr;
    char port[100];
    getinfo("Enter Port Number",port);
    servaddr = fillServerInfo(servaddr,"127.0.0.1",atoi(port));
    sockfd = createSocket();
    bindserver(servaddr,sockfd);
    printf("Listening on port %i\n",atoi(port));
    receiveMsg(sockfd);
    close(sockfd); 
    return 0; 
} 

