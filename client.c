#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFSIZE 256
const char *DELIM = ",";

void error(const char *msg) {

    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {

    // Socket variables and initialization 
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char outbuffer[BUFFSIZE];
    char inbuffer[BUFFSIZE];
    bzero(outbuffer,BUFFSIZE);
    bzero(inbuffer,BUFFSIZE);
    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Variables for extracting modeled trajectory
    double xpos[BUFFSIZE];  // Modeled x position
    double ypos[BUFFSIZE];  // Modeled y position
    bool eot = false;    // End of transmission flag
    char *cur;
    int i = 0;

    if (argc < 5) {
       fprintf(stderr,"usage %s <hostname> <port> <velocity> <angle>\n", argv[0]);
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

    printf("Velocity: %s\n",argv[3]);
    printf("Angle: %s\n",argv[4]);
    sprintf(outbuffer, "%s %s", argv[3], argv[4]);
    n = write(sockfd,outbuffer,strlen(outbuffer));    // then send to the server
    if (n < 0)
        error("ERROR writing to socket");

    while(1) {
    n = read(sockfd,inbuffer,BUFFSIZE);                    // get the response
        if (n < 0)
            error("ERROR reading from socket");

        cur = strtok(inbuffer, DELIM);

        while(cur != NULL) {   
            if(strcmp("EOT", cur) == 0) {
                eot = true;
                break;
            }
            if(i == 0) {
                printf("    X\t    Y\n");
            }
            xpos[++i] = atof(cur);
            cur = strtok(NULL, DELIM);
            if(cur == NULL) {
                break;
            }
            ypos[i] = atof(cur);
            printf("%lf %lf\n", xpos[i], ypos[i]);
            cur = strtok(NULL, DELIM);
        }
        if(eot) {
            break;
        }
    }                          
    close(sockfd);     // close the socket
    return EXIT_SUCCESS;
}