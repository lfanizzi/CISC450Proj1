


//NEW CODE
/* udp_client.c */ 
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2021 */

#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, sendto, and recvfrom */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */

#define STRING_SIZE 1024
unsigned int packets = 0;
unsigned long bytesR = 0;
unsigned int sum_sequence = 0;
unsigned long checksum = 0;
unsigned int ID = 0;
unsigned int count_field = 0;

typedef struct header{
   unsigned short int requestID;
   unsigned short int count;
} header_t;

typedef struct serverHeader{
   unsigned short int requestID;
   unsigned short int count;
   unsigned short int last;
   unsigned short int sequenceNumber;
} serverHeader_t;

typedef struct packet{
   struct serverHeader currentHeader;
   long int dataInts[25*4];
}packet_t;

int main(void) {

   int sock_client;  /* Socket used by client */ 

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned short client_port;  /* Port number used by client (local port) */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
  
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Client: can't open datagram socket\n");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information
            unless you want to specify a specific local port.
            The local address initialization and binding is done automatically
            when the sendto function is called later, if the socket has not
            already been bound. 
            The code below illustrates how to initialize and bind to a
            specific local port, if that is desired. */

   /* initialize client address information */

   client_port = 0;   /* This allows choice of any available local port */

   /* Uncomment the lines below if you want to specify a particular 
             local port: */
   /*
   printf("Enter port number for client: ");
   scanf("%hu", &client_port);
   */

   /* clear client address structure and initialize with client address */
   memset(&client_addr, 0, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                        any host interface, if more than one 
                                        are present */
   client_addr.sin_port = htons(client_port);

   /* bind the socket to the local client port */

   if (bind(sock_client, (struct sockaddr *) &client_addr,
                                    sizeof (client_addr)) < 0) {
      perror("Client: can't bind to local address\n");
      close(sock_client);
      exit(1);
   }

   /* end of local address initialization and binding */

   /* initialize server address information */

   printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname\n");
      close(sock_client);
      exit(1);
   }
   printf("Enter port number for server: ");
   scanf("%hu", &server_port);

   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(server_port);
unsigned char msg = malloc(sizeof(char));
msg ='y';
   /* user interface */                                 //LOOOOOOOOOP
do{
   printf("Please input a number:\n");
   scanf("%s", sentence);
   struct header *initialHeader = malloc(sizeof(header_t));
   initialHeader->count = atoi(sentence);
   initialHeader->requestID = 1;
   printf("This is the value as an int: %d\n", initialHeader->count);

   /* send message */

   struct packet newPacket;
  
   bytes_sent = sendto(sock_client, initialHeader, sizeof(*initialHeader), 0,
            (struct sockaddr *) &server_addr, sizeof (server_addr));

   /* get response from server */
   int i = 1;
   int lastNumber = 0;
   printf("Waiting for response from server...\n");
   do{
      bytes_recd = recvfrom(sock_client, &newPacket, sizeof(packet_t), 0,
                (struct sockaddr *) 0, (int *) 0);

   packets  += 1;// Add to total number of response packets
   ID += newPacket.currentHeader.requestID;
   sum_sequence += newPacket.currentHeader.sequenceNumber;
   if(newPacket.currentHeader.last == 1){
      bytesR = newPacket.dataInts[newPacket.currentHeader.count];
   }
   else{
         bytesR += 25;
      }
   }while(newPacket.currentHeader.last != 1);

   printf("\nThe response from server is:\n");
   //printf("%s\n\n", modifiedSentence);
   printf("\nThe response from server is:\n");
      for(int i = 0; i < atoi(sentence); i++){
         printf("%ld \n", newPacket.dataInts[i]);
          }
   printf("Would you like to send another message? (y/n) \n\n");
    scanf("%c", &msg);
    scanf("%c", &msg);
}while(msg == 'y');
      
      
      
      


   /* close the socket */

   close (sock_client);
   //After last response, all the below values are printed
printf("The Request ID is: %d \n", ID);
printf("The count field is: %d \n", count_field);
printf("Total number of response packets recieved: %d \n", packets);
printf("Total number of Bytes received: %lu \n", bytesR);
printf("Sum of sequence number fields: %d \n", sum_sequence);
printf("Checksum: %lu\n", checksum);
}
/*
printf("\nThe response from server is:\n");
      for(int i = 0; i < atoi(sentence); i++){
         printf("%ld \n", newPacket.dataInts[i]);
          }
      }while(newPacket.currentHeader.last != 1);
   }while (1);*/