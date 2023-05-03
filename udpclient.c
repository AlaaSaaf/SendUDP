/* 
 * udpclient.c - A simple UDP client
 * usage: udpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //sleep
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024


/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}
// Send File 
void send_file_data(FILE *fp, int sockfd, struct sockaddr_in addr){
  int n;
  char buffer[BUFSIZE];

  // Sending the data
  while(fgets(buffer, BUFSIZE, fp) != NULL){
    printf("[SENDING] Data: %s", buffer);

    n = sendto(sockfd, buffer, BUFSIZE, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (n == -1){
      perror("[ERROR] sending data to the server.");
      exit(1);
    }
    bzero(buffer, BUFSIZE);
    sleep(1);
  }

  // Sending the 'END'
  strcpy(buffer, "END");
  sendto(sockfd, buffer, BUFSIZE, 0, (struct sockaddr*)&addr, sizeof(addr));

  fclose(fp);
  return;
}


int main(int argc, char **argv) {
    int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
    FILE *fp;
    char *filename = "GPSData";
    /* check command line arguments */
    /*if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }*/
    hostname = "10.42.0.171";
    portno = atoi("2947");
    
    //hostname = argv[1];
    //portno = atoi(argv[2]);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    // Reading the text file
    fp = fopen(filename, "r");
    if (fp == NULL){
      perror("[ERROR] reading the file");
      exit(1);
    }

     // Sending the file data to the server
     send_file_data(fp, sockfd, serveraddr);

     printf("[SUCCESS] Data transfer complete.\n");
     printf("[CLOSING] Disconnecting from the server.\n");	
    /* get a message from the user */
    /*bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin);*/

    /* send the message to the server */
    /*serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
    if (n < 0) 
      error("ERROR in sendto");*/
    
    /* print the server's reply */
    /*n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");
    printf("Echo from server: %s", buf);
    */
    close(sockfd);
    return 0;
}
