#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <math.h>

#define BUFFSIZE 256
#define GRAVITY 9.81 
#define TIMESTEP 0.1
#define EOT "EOT"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

// this function is run by the second thread 
void *Trajectory(int *newsockfd) {
    int n = 0;
    char inbuffer[BUFFSIZE];
    char outbuffer[BUFFSIZE];
    double thrust = 0.0;
    double angle = 0.0;

    bzero(inbuffer, BUFFSIZE);
    bzero(outbuffer, BUFFSIZE);

    printf("Client Successfully connected,\n");

    n = read(*newsockfd,inbuffer,BUFFSIZE-1);   // get the string sent from the client
    if (n < 0)
        error("ERROR reading from socket");

    sscanf(inbuffer, "%lf %lf\n", &thrust, &angle);
    printf("Thrust: [%lf]\n", thrust);         // and dump it to string
    printf("Angle: [%lf]\n", angle);         // and dump it to string

    double x = 0.0;     // X position of projectile
    double vy = thrust; // Velovity initialized to initial velocity
    double y = 0.0;     // Y position of projectile
    double xo = 0.0;    // Previous x position 
    double vi = 0.0;    // Previous velocity in y direction
    double yo = 0.0;    // Previous y position

    do {
        xo = x; 
        vi = vy;
        yo = y;

        x = xo + (thrust * cos((angle * M_PI) / 180.0f) * TIMESTEP);
        vy = (vi * sin((angle * M_PI) / 180.0f)) - (GRAVITY * TIMESTEP);
        y = yo + (vy * TIMESTEP);

        sprintf(outbuffer, "%lf  %lf", x, y); 

        n = write(*newsockfd,outbuffer,strlen(outbuffer+1)); // then send it back
        if (n < 0)
            error("ERROR writing to socket");
        printf("Sent    : [%s]\n",outbuffer);
        usleep(1000);
    } while(y > 0);
    // Tell client it's the end of transmission
    n = write(*newsockfd,EOT,strlen(outbuffer+1));
    if (n < 0)
        error("ERROR writing to socket");
    printf("Sent    : [%s]\n",outbuffer);

printf("Exiting Thread\n");
//close(*newsockfd); 

// the function must return something
return NULL;

}

int main(int argc, char *argv[]) {
    struct addrinfo cli_hints, *cli_res;
    int status;
    int sockfd = 0;
    int newsockfd = 0;
    socklen_t clilen;
    struct sockaddr_in cli_addr;
 
    if (argc < 2) {
        error("ERROR, no port provided\n");
    }

    memset(&cli_hints, 0, sizeof cli_hints);
    cli_hints.ai_family = AF_UNSPEC;
    cli_hints.ai_socktype = SOCK_STREAM;
    cli_hints.ai_flags = AI_PASSIVE; 

    if ((status = getaddrinfo(NULL, argv[1], &cli_hints, &cli_res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    } 

    sockfd = socket(cli_res->ai_family, cli_res->ai_socktype, cli_res->ai_protocol); 
    if (sockfd < 0) {                                                     
        error("ERROR opening socket");
    }
    
    if (bind(sockfd, cli_res->ai_addr, cli_res->ai_addrlen) < 0) {
        error("ERROR on binding"); 
    }    
    while(1) {
        listen(sockfd,5);     // block and wait for a new connection from a client
        clilen = sizeof(cli_addr);     // get address of client
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        printf("New client connecting.\n");

        if (newsockfd < 0) {
            error("ERROR on accept");
        }

        // this variable is our reference to the second thread 
        pthread_t inc_x_thread;

        // create a second thread
        if(pthread_create(&inc_x_thread, NULL, (void *)Trajectory, &newsockfd)) {
            error("Error creating thread");
        }
    }
  
    close(sockfd);    
    return EXIT_SUCCESS;  
}


