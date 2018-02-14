#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//#include <arpa/inet.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
     struct addrinfo cli_hints, *cli_res;
     int status;
     int sockfd = 0;
     int newsockfd = 0;
     socklen_t clilen;
     struct sockaddr_in cli_addr;
     char inbuffer[256];
     char outbuffer[256];
     int n;

     n = 0; 
     bzero(inbuffer,256);
     bzero(outbuffer,256);
     sprintf(outbuffer,"World\n"); 

     if (argc < 2)
         error("ERROR, no port provided\n");

     memset(&cli_hints, 0, sizeof cli_hints);
     cli_hints.ai_family = AF_UNSPEC;
     cli_hints.ai_socktype = SOCK_STREAM;
     cli_hints.ai_flags = AI_PASSIVE; 

     if ((status = getaddrinfo(NULL, argv[1], &cli_hints, &cli_res)) != 0) {
         fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
         exit(1);
     } 
     sockfd = socket(cli_res->ai_family, cli_res->ai_socktype, cli_res->ai_protocol); 
     if (sockfd < 0)                                                     
         error("ERROR opening socket");

     if (bind(sockfd, cli_res->ai_addr, cli_res->ai_addrlen) < 0)
         error("ERROR on binding");                                                  
     listen(sockfd,5);     // block and wait for a new connection from a client

     clilen = sizeof(cli_addr);     // get address of client
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");

     n = read(newsockfd,inbuffer,255);   // get the string sent from the client
     if (n < 0)
         error("ERROR reading from socket");
     printf("Received: [%s]\n",inbuffer);         // and dump it to string

     n = write(newsockfd,outbuffer,strlen(outbuffer+1)); // then send it back
     if (n < 0)
         error("ERROR writing to socket");
     printf("Sent    : [%s]\n",outbuffer);

     close(newsockfd);   
     close(sockfd);    
     return EXIT_SUCCESS;  
}


