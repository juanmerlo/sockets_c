#include <stdio.h> // Por dependencia de readline en algunas distros de linux :)
#include <string.h>
#include <stdlib.h> // Para malloc
#include <sys/socket.h> // Para crear sockets, enviar, recibir, etc
#include <netdb.h> // Para getaddrinfo
#include <unistd.h> // Para close
#include <commons/log.h>
#include <commons/collections/list.h>

// Definimos algunas constantes para nuestro código
#define IP "127.0.0.1"
#define PUERTO "3490"

int  connect_to_server(char * ip, char * port);
void exit_gracefully(int return_nr);
void _exit_with_error(int socket, char* error_msg, void * buffer);

int main(){

	  int socket = connect_to_server(IP, PUERTO);

}

int connect_to_server(char * ip, char * port) {

	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(ip, port, &hints, &server_info);

	int server_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	int res = connect(server_socket, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);
	if (res < 0) {
	   _exit_with_error(server_socket, "No me pude conectar al servidor", NULL);
	}else{

		printf("Conectado");
	}

	//Enviando mensaje

	char *msg = "Beej was here";
	int len, bytes_sent;


	len = strlen(msg);
	bytes_sent = send(server_socket, msg, len, 0);

	printf("Bytes enviados: %i \n",bytes_sent);

	return server_socket;

}


void exit_gracefully(int return_nr) {
  exit(return_nr);
}

void _exit_with_error(int socket, char* error_msg, void * buffer) {
   if (buffer != NULL) {
       free(buffer);
   }

   printf(error_msg);
   close(socket);
   exit_gracefully(1);
}

///*
//    ** client.c -- Ejemplo de cliente de sockets de flujo
//    */
//
//    #include <stdio.h>
//    #include <stdlib.h>
//    #include <unistd.h>
//    #include <errno.h>
//    #include <string.h>
//    #include <netdb.h>
//    #include <sys/types.h>
//    #include <netinet/in.h>
//    #include <sys/socket.h>
//
//    #define PORT 3490 // puerto al que vamos a conectar
//
//    #define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez
//
//    int main(int argc, char *argv[])
//    {
//        int sockfd, numbytes;
//        char buf[MAXDATASIZE];
//        struct hostent *he;
//        struct sockaddr_in their_addr; // información de la dirección de destino
//
//        if (argc != 2) {
//            fprintf(stderr,"usage: client hostname\n");
//            exit(1);
//        }
//
//        if ((he=gethostbyname(argv[1])) == NULL) {  // obtener información de máquina
//            perror("gethostbyname");
//            exit(1);
//        }
//
//        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
//            perror("socket");
//            exit(1);
//        }
//
//        their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
//        their_addr.sin_port = htons(PORT);  // short, Ordenación de bytes de la red
//        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
//        memset(&(their_addr.sin_zero), '\0',  8);  // poner a cero el resto de la estructura
//
//        if (connect(sockfd, (struct sockaddr *)&their_addr,
//                                              sizeof(struct sockaddr)) == -1) {
//            perror("connect");
//            exit(1);
//        }
//
//        if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
//            perror("recv");
//            exit(1);
//        }
//
//        buf[numbytes] = '\0';
//
//        printf("Received: %s",buf);
//
//        close(sockfd);
//
//        return 0;
//    }
//
