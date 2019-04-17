#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, n;
     socklen_t clilen;
     char buffer[1024];
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR!, port number absent\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);		//socket file descriptor created
     if (sockfd < 0) 
        error("ERROR! for opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));		//socket strut initialization
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)		//bind the host
              error("ERROR! for  binding");

     listen(sockfd,3);		//listen and wait for connections
     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);  
     if (newsockfd < 0) 
          error("ERROR! for accepting");

      int n1 , n2 , ans , cont; 

R:    n = write(newsockfd,"Enter Number 1 : ",strlen("Enter Number 1"));		//ask client for number 1         
      if (n < 0) error("ERROR! for writing to socket");
      read(newsockfd, &n1, sizeof(int));					
      printf("Client says:  Number 1 is : %d\n" , n1);		//display number 1 given by client          
          
      n = write(newsockfd,"Enter Number 2 : ",strlen("Enter Number 2"));		//ask client for number 2          
      if (n < 0) error("ERROR! for writing to socket");
      read(newsockfd, &n2, sizeof(int));					
      printf("Client says:   Number 2 is : %d\n" , n2);				//display number 2 given by client          
      
      ans = n1*n2;

      write(newsockfd , &ans , sizeof(int));		//respond the client with multiplication of two numbers

      n = write(newsockfd,"Repeat? Press 1 to Continue 0 to Exit: ",strlen("Repeat? Press 1 to Continue 0 to Exit: "));		//ask if client wants to continue or exit          
      if (n < 0) error("ERROR! for writing to socket");
      read(newsockfd, &cont, sizeof(int));					
      printf("Repeat? continue-1 / exit-0 : %d\n" , cont);
     
      if(cont==1)
	goto R;
          
      close(newsockfd);
      close(sockfd);		

     return 0; 

}

