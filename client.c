#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>

void error(const char *msg)	//error message 
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    if (argc < 3)
    {
       fprintf(stderr,"Usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);		//socket file descriptor created
    if (sockfd < 0) 
        error("ERROR! for opening socket\n");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR! no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length); 
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)		//connection to the server 
        error("ERROR! for connecting\n");
  
    int n1, n2, ans, cont;    
    
R:  bzero(buffer,256);						
    n = read(sockfd,buffer,255);		//messgae for input number 1				
    if (n < 0) 
         error("ERROR! for reading from socket\n");
    printf("Server says:   %s\n",buffer);
    scanf("%d" , &n1);						
    write(sockfd, &n1, sizeof(int));		//accept number 1 and send to server     	
	
    bzero(buffer,256);						
    n = read(sockfd,buffer,255);		//messgae for input number 2								
    if (n < 0) 
         error("ERROR! for reading from socket\n");
    printf("Server says:  %s\n",buffer);
    scanf("%d" , &n2);						
    write(sockfd, &n2, sizeof(int));		//accept number 2 and send to server     	     			
    	
    read(sockfd , &ans , sizeof(int));		//result from server
    printf("Server says: The multiplication product is:  %d\n" , ans);		

    bzero(buffer,256);						
    n = read(sockfd,buffer,255);		//messgae for continuing								
    if (n < 0) 
         error("ERROR! for reading from socket\n");
    printf("Server says:  %s\n",buffer);
    scanf("%d" , &cont);						
    write(sockfd, &cont, sizeof(int));		//accept yes/no and send to server     	     			
	
    if(cont!=0)
      goto R;	

    printf("\n Thank you. You may now close the terminal.\n");	
    close(sockfd);
    return 0;
}

