#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <time.h>
#define PORT     8080 
#define MAXLINE 1024
#define Group join_group.imr_multiaddr.s_addr
#define Interface join_group.imr_interface.s_addr
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
    struct sockaddr_in receiver;
    int size = sizeof(receiver);
    time_t now;
    time(&now);

    while (recvfrom(fd,buffer,MAXLINE,0,(struct sockaddr *)&receiver,&size) > 0){
    printf("%s%s\n",ctime(&now),buffer);
    }
    fflush(stdout);
    close(fd);
}
struct sockaddr_in MEInfo(struct sockaddr_in server){
	//bzero((char*
    server.sin_port = htons(22210); 
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    return server;
}
//Fill server information
struct sockaddr_in fillServerInfo(struct sockaddr_in server,char ipaddress[100],int port){
    server.sin_family = AF_INET;
    server.sin_port = htons(port); 
    //set the server address with ip address as a string 
    server.sin_addr.s_addr = inet_addr("239.10.5.10");
    return server;
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

int bindserver(struct sockaddr_in servaddr,int sockfd, char *ip){
    struct ip_mreq join_group;
    Group = inet_addr(ip);
    Interface  = htonl(INADDR_ANY);
    if ( setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&join_group,sizeof(struct ip_mreq)) < 0 ) 
    {   
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) > 0 )
        {
            perror("Binding datagram socket error");
        }

    return 0;
}
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
    struct sockaddr_in servaddr,me; 
    sockfd = createSocket();
    char ipaddr[100],msg[100];
    char groupNum[3],ip[50]; 
    checkArg(argv[1]);
    strcpy(groupNum,argv[1]);
    strcpy(ip,"239.10.5.");
    port = atoi(groupNum) + 22200;
    strcat(ip,groupNum);
    me = MEInfo(me);
    servaddr = fillServerInfo(servaddr,ip,port);
    bindserver(me,sockfd,ip);
    recvMsg(sockfd);
    close(sockfd); 
    return 0; 
}

