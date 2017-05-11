/*
-------------------------------------------------
Assignment No.: 2 - Socket Programming using TCP 
Name: Unmesh Suryawanshi
Net id: qd6395
Course: CS 4590 - Computer Networks
--------------------------------------- ---------
*/

/* Client */

/* Header File Declaration */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h> //for gettimeofday() 
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>

/* This is the Client program. It opens a socket, sets the receive
window as requested, and then begins an infinite loop. Each time through 
the loop it accepts a connection and prints out messages from it. When 
the connection breaks or a termination message comes through it, the 
program accepts a new connection.
Note: File size should be : 10KB -> 10000, 20KB -> 20000, 30KB -> 30000, 40KB -> 40000, 50KB -> 50000 
The form of the command line is: 'client hostname portnumber filesize' */

// main function
int main(int argc,char *argv[])
{
  	int sd, sda, rcvwin, i;
  	struct sockaddr_in rcvr;
  	struct hostent *hp, *gethostbyname();
  	char buf[1000];
  	int sndwin, optlen, buflen;
  	struct timeval sndtime;
  	struct timezone zone;
  	int rval, seqno;

  	sndwin = buflen;

  	/* create socket */
  	sd = socket(AF_INET, SOCK_STREAM, 0);
  	if(sd < 0)
	{
    		perror("Opening stream socket");
    		exit(1);
  	}
  
  	/* Create rcvr socket address using command line arguments */
  	rcvr.sin_family = AF_INET;
  	hp = gethostbyname(argv[1]);
  	if(hp == 0)
	{
    		printf("%s: unknown host\n",argv[1]);
    		exit(2);
  		}
  	bcopy(hp->h_addr, &rcvr.sin_addr, hp->h_length);
  	rcvr.sin_port = htons(atoi(argv[2]));

  	/* connect socket using name specified at command line */
  	if(connect(sd, (struct sockaddr *)&rcvr, sizeof(rcvr)) < 0)
	{
    		perror("connecting stream socket");
    		exit(1);
  	}
  	if (gettimeofday(&sndtime, &zone) < 0)
	{
    		perror("Getting time");
  	}
  	printf("Start time: %ld %ld\n", sndtime.tv_sec, sndtime.tv_usec);
	
	/* copy the file size which is given by user into vari. -> buflen */
	buflen=atoi(argv[3]);
 	write(sd, &buflen, sizeof(buflen));
        do
	{ 
		bzero(buf,sizeof(buf));
	        if((rval = recv(sd,buf,sizeof(buf),MSG_WAITALL))<0)
		        perror("reading message stream");
        	if(rval == 0)
          		printf("Ending connection\n");
        	else
		{
			/* getting the data and display it on terminal */
	  		read(sd,&seqno,sizeof(seqno));
			printf("\n-----------------------------------------------\n");          		
			printf("Received packet: Seq No.: %d & Length: %d ",seqno, rval);
			printf("\n-----------------------------------------------\n");          		
			printf("Buffer data: \n %s", buf);
		printf("\n-----------------------------------------------\n");
        	}
      	}while (rval != 0);
  	close(sd);
}
