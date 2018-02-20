#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char outbuffer[256];
    char inbuffer[256];
    bzero(outbuffer,256);
    bzero(inbuffer,256);
    bzero((char *) &serv_addr, sizeof(serv_addr));

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    server = gethostbyname(argv[1]);                  // translate the host name into an address
    if (server == NULL)
        error("ERROR, no such host\n");
    portno = atoi(argv[2]);                           // get the port number
    sockfd = socket(AF_INET, SOCK_STREAM, 0);         // create a generic socket
    if (sockfd < 0)
        error("ERROR opening socket");

    serv_addr.sin_family = AF_INET;                   // it's an internet type
    bcopy((char *)server->h_addr,                     // make sure we connect with the IP not name
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

    serv_addr.sin_port = htons(portno);               // port numbers should be in network byte order
    if (connect(sockfd,                               // connect socket to
        (struct sockaddr *) &serv_addr,               // the server address (bind)
        sizeof(serv_addr))                            // where the server address structure is this length
        < 0)
        error("ERROR connecting");

    printf("Please enter the message: ");             // ask user for "hello"
    fgets(outbuffer,255,stdin);                       // and fill up the output buffer
    n = write(sockfd,outbuffer,strlen(outbuffer));    // then send to the server
    if (n < 0)
        error("ERROR writing to socket");

    while(1) {
    n = read(sockfd,inbuffer,255);                    // get the response
        if (n < 0)
            error("ERROR reading from socket");

        printf("%s\n",inbuffer);
    }                          // and dump out what you received
    close(sockfd);                                    // close the socket
    return EXIT_SUCCESS;
}