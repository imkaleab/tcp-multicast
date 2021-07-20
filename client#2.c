//Kaleab Alemu
//CSCE 3530.001
//Date: 04/17/2021
//Instructor: Jonathon Doran
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
    unsigned int sockfd, len, addrlen;
    struct sockaddr_in multicast, server, client, test;
    char buffer[1024];
    char recvbuff[1024];

    //create a udp socket to send the multicast message 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //specify multicast address for the socket to send the message
    len = sizeof(multicast);
    bzero(&multicast, len);
    multicast.sin_addr.s_addr = inet_addr("225.1.2.1");
    multicast.sin_family = AF_INET;
    multicast.sin_port = htons(10234);
    

    //send multicast message through socket
    strcpy(buffer, "Hello everyone in multicast group.");
    sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr*) &multicast, sizeof(multicast));
    
    //get the ephemeral port no. used to send the multicast message
    len = sizeof(client);
    getsockname(sockfd, (struct sockaddr*) &client, &len);
    
    //close the udp socket
    close(sockfd);

    //create a tcp socket and bind it to the ephemeral port used to send the multicast message and listen for incoming connections
    int newsockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(newsockfd, (const struct sockaddr*) &client, sizeof(client));
    listen(newsockfd, 5);    

    //accept an incoming tcp connection 
    bzero(&server, sizeof(server));
    addrlen = sizeof(server);
    int servsock = accept(newsockfd, (struct sockaddr*) &server, &addrlen);
    if(servsock == -1) printf("accept error\n");

    //write to server about success of connection
    bzero(buffer, 1024);
    strcpy(buffer, "successfully connected in tcp");
    write(servsock, buffer, strlen(buffer));    

    //close the tcp socket
    close(servsock);

    return 0;
}
