
/**************************************************
compile with the command: gcc demo_rx.c rs232.c -Wall -Wextra -o2 -o test_rx

**************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

#define START "Start"
#define DELIM ","
#define MAX_WORD 1024
#define MAX_ANGLE 90    // deg
#define MAX_VELOCITY 50 // m/s
#define MAX_POSITION 20 // m

#define ADDRESS "localhost"
#define PORT 9999
#define CLIENT "./client"

int main()
{
  int i, n,
      cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=9600;       /* 9600 baud */

  unsigned char buf[4096];
  float ivel = 0.0;
  float iang = 0.0;
  float ipos = 0.0;

  char mode[]={'8','N','1',0};


  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }

  while(1)
  {
    n = RS232_PollComport(cport_nr, buf, 4095);

    if(n > 0)
    { 
      buf[n] = 0;   /* always put a "null" at the end of a string! */

      for(i=0; i < n; i++)
      {
        if(buf[i] < 32)  /* replace unreadable control-codes by dots */
        {
          buf[i] = '.';
        }
      }

      printf("Got: [%s]\n", (char *)buf);
      if(strcmp(START, (char *) buf) == 0) {
          printf("Sending data to server.....\n");
          printf("Inital Velocity: %f m/s\n", ivel);
          printf("Initial angle: %f deg\n", iang);
          printf("Initial position: %f m\n", ipos);
          char cmd[50];
          sprintf(cmd, "%s %s %d %f %f", CLIENT, ADDRESS, PORT, ivel, iang);
          printf("Command: %s\n", cmd);
          system(cmd);
          printf("Model complete\n");
      }
      else {
        ivel = (atof(strtok((char *) buf, DELIM))/MAX_WORD) * MAX_VELOCITY;
        iang = (atof(strtok(NULL, DELIM))/MAX_WORD) * MAX_ANGLE;
        ipos = (atof(strtok(NULL, DELIM))/MAX_WORD) * MAX_POSITION;
      } 
    }

#ifdef _WIN32
    Sleep(100);
#else
    usleep(100000);  /* sleep for 100 milliSeconds */
#endif
  }

  return(0);
}

