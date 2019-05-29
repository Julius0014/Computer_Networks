#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <netinet/in.h> 
#define PORT     8080 
#define MAXLINE 1024
#define nMsg 500
int fd,sockfd;
struct sockaddr_in servaddr,me;
char msg[100];
double delta = 0.0;
int sent = 0,count = 0;
typedef struct msg{
    struct timespec start,end;
    int m;
}Msg;
// Julius Jenkins
// create socket and return socket file desciptor
int createSocket(void){
    int socketnum;
    if ( (socketnum = socket(AF_INET,SOCK_DGRAM,0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    return socketnum;

}
int recvMsg(){
    int bytes = 99;
    Msg buffer2;
    struct sockaddr_in temp; 
    int len = sizeof(temp);
    while(1){
        sleep(2);
        if(recvfrom(fd,&buffer2,sizeof(buffer2),MSG_DONTWAIT,(struct sockaddr *)&temp,&len) < 0){
            break;
        }else{
            clock_gettime(CLOCK_MONOTONIC,&buffer2.end);
            float x = ( (double )buffer2.end.tv_nsec - (double)buffer2.start.tv_nsec);
            //convert nansec to secs
            double secs = x / 1000000000;
            
            delta += secs;
            count++;
        }
    }
    fflush(stdout);
    return;
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
int sendMsg(){
    int len = sizeof(servaddr),i =0;
    char buffer[100];
    Msg msg1;
    msg1.m = 1;
    while(i != nMsg){
        clock_gettime(CLOCK_MONOTONIC,&msg1.start);
        if (sendto(sockfd,&msg1,sizeof(msg1),0,(struct sockaddr *)&servaddr,len) < 0){
            perror("ERROR SENDING MESSAGE:");
            exit(-1);
        }
        sleep(1);
        sent++;
        i++;
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
    return 0;
}
int main(int argc,char *argv[]) {
    pthread_t id,id2;
    struct timespec start,end;
    sockfd = createSocket();
    fd = sockfd;
    char ipaddr[100],port[100];
    strcpy(ipaddr,"127.0.0.1");
    strcpy(port,"2000");
    me = MEInfo(me);
    servaddr = fillServerInfo(servaddr,ipaddr,atoi(port));
    bindandConnect(me,servaddr,sockfd);
    pthread_create(&id, NULL,recvMsg,NULL);
    pthread_create(&id2, NULL,sendMsg,NULL);
    pthread_join(id, NULL);
    printf("Packet loss = %.2f percent\n",( 1 - (double)count/nMsg) * 100);
    printf("sent = %d count = %d\nAvg. delta time (RTT) = %.3f m/s\n",sent,count,(delta/count) * 100);
    close(sockfd); 
    return 0; 
} 

