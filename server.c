/*
-------------------------------------------------
Assignment No.: 2 - Socket Programming using TCP 
Name: Unmesh Suryawanshi
Net id: qd6395
Course: CS 4590 - Computer Networks
--------------------------------------- ---------
*/

/* Server */

/* Header File Declaration */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>

#define TRUE 1
#define SERVER_PORT 50057 //declaration of server port number

/* This is the server program. It creates a socket and initiates a connection with the socket given in the command line. It checks the size of the file given at the client side then it opens the file in read mode and It will send the send buffer (size of buffer is 1000) is set as requested. Once all data is sent over the connection and then the socket is closed.
The form of the command line is `server' */

int main()
{
	int sd, i, sndwin, length;
	size_t temp1;
  	struct sockaddr_in rcvr;
	int sda, buflen;
	char buf[1000];
  	int rcvwin, optlen;
  	int rval;
  	int seqno;
	FILE *fp; //declaration of file pointer
  	sndwin = buflen;

  	/* create socket */
  	sd = socket(AF_INET, SOCK_STREAM, 0);
  	if(sd < 0)
	{
    		perror("opening stream socket");
    		exit(1);
  	}

  	/* name socket using wildcards */
  	rcvr.sin_family = AF_INET;
  	rcvr.sin_addr.s_addr = INADDR_ANY;
  	rcvr.sin_port = htons(SERVER_PORT);
  	if(bind(sd, (struct sockaddr *)&rcvr, sizeof(rcvr)))
	{
    		perror("binding socket name");
    		exit(1);
 	}

  	/* find out assigned port number and print out */
  	length = sizeof(rcvr);
  	if(getsockname(sd, (struct sockaddr *)&rcvr, &length))
	{
    		perror("getting socket name");
    		exit(1);
  	}
  	printf("Socket has port #%d\n",ntohs(rcvr.sin_port));
	
  	/* Can get same effect by doing reads of the correct size. */
  	sndwin = buflen;

  	/* Accept connections from the transmitter */
  	listen(sd, 5);
	sda = accept(sd,0,0);
    	if(sda == -1)
      		perror("accept");
	else
	{
		/* reading the file size from th client */
		read(sda,&sndwin,sizeof(sndwin));
		if(sndwin==10000)
		{		
			/* opening a file in read mode */ 			
			fp = fopen("file10KB.txt", "r");	
				}
		else if(sndwin==20000)
		{		
			fp = fopen("file20KB.txt", "r");	
				}
		else if(sndwin==30000)
		{		
			fp = fopen("file30KB.txt", "r");	
				}
		else if(sndwin==40000)
		{		
			fp = fopen("file40KB.txt", "r");	
				}	
		else if(sndwin==50000)
		{		
			fp = fopen("file50KB.txt", "r");	
				}			
 		
		/* create a packet (size of packet is 1000) */
		for(seqno=1; seqno<=sndwin/sizeof(buf); seqno++)
		{
			/* reading the data using file pointer */
			temp1=fread(buf,sizeof(buf),1,fp);
			buf[temp1]=' '; // initialize buffer to null value
  			
			/* send the packet */
			if((rval=send(sda, buf, sizeof(buf), 0))<0)
			{
   				perror(" Writing on stream socket ");
			}
			else
			{					
				write(sda, &seqno, sizeof(seqno));
     				printf(" Sent packet of length : %d\n", rval);
				usleep(1000000); //delay
  			}
		}
		fclose(fp);
	}
        close(sda);
}
