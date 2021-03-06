CLIENT::

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    if(argc<3){
        fprintf(stderr, "Usage: %s Hostname Port \n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[256];
    struct hostent *server;

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd<0)
        error("Error opening socket.");
    
    server = gethostbyname(argv[1]);
    if(server==NULL){
        fprintf(stderr, "No such host");
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    
    //Transferring info. from *server into serv_addr
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
        error("Connection failed");
    
    int num1, num2, choice, ans;
    S : bzero(buffer, 256);
    	n = read(sockfd, buffer, 255);
    	if(n<0){
    	error("Error reading from socket");
    	}
    	printf("Server: %s\n ", buffer);
    	scanf("%d", &num1);
    	write(sockfd, &num1, sizeof(int));
    	
    	bzero(buffer, 256);
    	n = read(sockfd, buffer, 255);
    	if(n<0){
    	error("Error reading from socket");
    	}
    	printf("Server: %s\n ", buffer);
    	scanf("%d", &num2);
    	write(sockfd, &num2, sizeof(int));
    	
    	bzero(buffer, 256);
    	n = read(sockfd, buffer, 255);
    	if(n<0){
    	error("Error reading from socket");
    	}
    	printf("Server: %s\n ", buffer);
    	scanf("%d", &choice);
    	write(sockfd, &choice, sizeof(int));
    	
    	if(choice==5)
    	goto Q;
    	
    	read(sockfd, &ans, sizeof(int));
    	printf("Server: The answer is: %d\n", ans);
    	
    	if(choice!=5){
    	goto S;
    	}
    	
    Q: printf("You have selected to exit");
    close(sockfd);
    return 0;
}



SERVER::


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){

    if(argc<2){
        fprintf(stderr, "Port no. not provided. Program terminated\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    // sockaddr_in gives us the internet address, it is included in the file <netinet/in.h>
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        error("Error opening socket.");
    }

    //bzero clears any data or text into whatever it is referenced to
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);    //htons = host to network short


//we need to typecast sockaddr_in into sockaddr
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Binding failed.");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr); 

    //next, whenever we accept a connection, we get a new file descriptor
    // again we need to typecast sockaddr_in into sockaddr
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if(newsockfd<0){
        error("Error on accept!");
    }

 	int num1, num2, ans, choice;
 S:	n = write(newsockfd, "Enter number 1: ", strlen("Enter number 1: "));			//Ask for Number 1
 	if(n<0) error("Error writing to socket");
 	read(newsockfd, &num1, sizeof(int));							//Read Number 1
 	printf("Client-number 1 is: %d\n", num1);
 	
 	
 	n = write(newsockfd, "Enter number 2: ", strlen("Enter number 2: "));
 	if(n<0) error("Error writing to socket");
 	read(newsockfd, &num2, sizeof(int));
 	printf("Client-number 2 is: %d\n", num2);
 	
 	n = write(newsockfd, "Enter your choice: \n1.Addition \n2.Subtraction\n3.Multiplication \n4.Division\n5.Exit ", strlen("Enter your choice: \n1.Addition \n2.Subtraction\n3.Multiplication \n4.Division\n5.Exit "));
 	
 	if(n<0) error("Error writing to socket");
 	read(newsockfd, &choice, sizeof(int));
 	printf("Client-choice is: %d\n", choice);
 	
 	switch(choice)
 	{
 	case 1: ans = num1 + num2;
 		break;
 	case 2: ans = num1 - num2;
 		break;
 	case 3: ans = num1 * num2;
 		break;
 	case 4: ans = num1 / num2;
 		break;
 	case 5: goto Q;
 		break;	
 	}
 	
 	write(newsockfd, &ans, sizeof(int));
 	if(choice!=5){
 	goto S;
 	}
 	
  Q:    close(newsockfd);
        close(sockfd);
        return 0;
    }


