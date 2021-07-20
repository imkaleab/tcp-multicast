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
#include <errno.h>

int main()
{
    int sockfd, len, newsockfd;
    socklen_t addrlen;
    struct sockaddr_in multicast, client, server;
    char buffer[1024];
    char recvbuffer[1024];
    struct ip_mreq multicastreq;

    //create a udp socket and bind multicast address to it
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&multicast, sizeof(multicast));
    
    multicast.sin_addr.s_addr = INADDR_ANY; 
    multicast.sin_family = AF_INET;
    multicast.sin_port = htons(10234);

    bind(sockfd, (const struct sockaddr*) &multicast, sizeof(multicast));
    printf("server waiting\n");

    //specify multicast group
    multicastreq.imr_multiaddr.s_addr = inet_addr("225.1.2.1");
    //accept from any interface 
    multicastreq.imr_interface.s_addr = INADDR_ANY;
    //join the multicast address
    setsockopt(sockfd,IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*) &multicastreq, sizeof(multicastreq));


    while(1){

        //receive a multicast message
        bzero(&client, sizeof(client));
        bzero(recvbuffer, 1024);
        addrlen = sizeof(client); 
        len = recvfrom(sockfd, recvbuffer, 1024, 0, (struct sockaddr*) &client, &addrlen);
        printf("received a multicast message '");
        fputs(recvbuffer, stdout);
        printf("' from '%s'\n", inet_ntoa(client.sin_addr));


        //create a new tcp socket for client to connect
        newsockfd = socket(AF_INET, SOCK_STREAM, 0);

        //connect directly to the client using the address obtained from the multicast message
        if(connect(newsockfd, (const struct sockaddr*) &client, sizeof(client)) < 0)
            printf("connect error %s\n", strerror(errno));
        else ("connection successful\n");
        
        //read message from client
        bzero(recvbuffer, 1024);
        if(read(newsockfd, recvbuffer, 1024) != -1)
        {
            printf("received '");
            fputs(recvbuffer, stdout);
            printf("' from '%s'\n", inet_ntoa(client.sin_addr));
        }

        else printf("read error\n");

        //close tcp socket
        close(newsockfd);
        printf("server waiting\n");

    }
    //close udp socket
    close(sockfd);
    return 0;
}