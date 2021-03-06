/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   This version runs forever, forking off a separate
   process for each connection
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const
int MAX_CLIENTS = 5;
int BUFFER_SIZE = 256;
char *MSG_HELLO= "I got your message";

void dostuff(int); /* function prototype */

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2)
        error("ERROR, no port provided\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_addr.s_addr = INADDR_ANY;

    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, MAX_CLIENTS);

    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd,  (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0)  {
            close(sockfd);
            dostuff(newsockfd);
            exit(EXIT_SUCCESS);
        } else close(newsockfd);
    } /* end of while */
    return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/

void dostuff (int sock) {
    int n;
    char buffer[BUFFER_SIZE];
    int cnt = 5;
    while (cnt-- > 0) {
        bzero(buffer,BUFFER_SIZE);
        n = read(sock,buffer,BUFFER_SIZE);

        if (n < 0)
            error("ERROR reading from socket");

        printf("Here is the message: %s\n",buffer);
        n = write(sock,MSG_HELLO,strlen(MSG_HELLO));
        if (n < 0)
            error("ERROR writing to socket");
    }
}

