#include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #define MYPORT 3490    // Puerto al que conectarán los usuarios
    #define BACKLOG 10     // Cuántas conexiones vamos a mantener en cola

    void exit_gracefully(int return_nr);
    void _exit_with_error(int socket, char* error_msg, void * buffer);

main()
    {
        int sockfd, new_fd;  // se escucha sobre sock_fd, Nuevas conexiones sobre new_fd
        struct sockaddr_in my_addr;    // Información sobre mi dirección
        struct sockaddr_in their_addr; // Información sobre la dirección remota
        int sin_size;
        sockfd = socket(AF_INET, SOCK_STREAM, 0); // ¡Comprobar errores!
        my_addr.sin_family = AF_INET;         // Ordenación de máquina
        my_addr.sin_port = htons(MYPORT);     // short, Ordenación de la red
        my_addr.sin_addr.s_addr = INADDR_ANY; // Rellenar con mi dirección IP
        memset(&(my_addr.sin_zero), '\0', 8); // Poner a cero el resto de la estructura
        // no olvides comprobar errores para estas llamadas:
        bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
        listen(sockfd, BACKLOG);
        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        char * hola = "Beej was here";
        char * buffer = (char*) calloc(sizeof(char), strlen(hola));

        int result_recv = recv(socket, buffer, strlen(hola), MSG_WAITALL);

        printf("Bytes recibidos: %i \n",result_recv);

        if(result_recv <= 0) {
          _exit_with_error(sockfd, "No se pudo recibir hola", buffer);


        }

        if (strcmp(buffer, hola) != 0) {
          _exit_with_error(sockfd, "No se pudo recibir hola", buffer);
        }

        printf("Mensaje de hola recibido : '%s",buffer);
        free(buffer);

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


//    /*
//       ** server.c -- Ejemplo de servidor de sockets de flujo
//       */
//
//       #include <stdio.h>
//       #include <stdlib.h>
//       #include <unistd.h>
//       #include <errno.h>
//       #include <string.h>
//       #include <sys/types.h>
//       #include <sys/socket.h>
//       #include <netinet/in.h>
//       #include <arpa/inet.h>
//       #include <sys/wait.h>
//       #include <signal.h>
//
//       #define MYPORT 3490    // Puerto al que conectarán los usuarios
//
//       #define BACKLOG 10     // Cuántas conexiones pendientes se mantienen en cola
//
//       void sigchld_handler(int s)
//       {
//           while(wait(NULL) > 0);
//       }
//
//       int main(void)
//       {
//           int sockfd, new_fd;  // Escuchar sobre sock_fd, nuevas conexiones sobre new_fd
//           struct sockaddr_in my_addr;    // información sobre mi dirección
//           struct sockaddr_in their_addr; // información sobre la dirección del cliente
//           int sin_size;
//           struct sigaction sa;
//           int yes=1;
//
//           if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
//               perror("socket");
//               exit(1);
//           }
//
//           if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
//               perror("setsockopt");
//               exit(1);
//           }
//
//           my_addr.sin_family = AF_INET;         // Ordenación de bytes de la máquina
//           my_addr.sin_port = htons(MYPORT);     // short, Ordenación de bytes de la red
//           my_addr.sin_addr.s_addr = INADDR_ANY; // Rellenar con mi dirección IP
//           memset(&(my_addr.sin_zero), '\0', 8); // Poner a cero el resto de la estructura
//
//           if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))
//                                                                          == -1) {
//               perror("bind");
//               exit(1);
//           }
//
//           if (listen(sockfd, BACKLOG) == -1) {
//               perror("listen");
//               exit(1);
//           }
//
//           sa.sa_handler = sigchld_handler; // Eliminar procesos muertos
//           sigemptyset(&sa.sa_mask);
//           sa.sa_flags = SA_RESTART;
//           if (sigaction(SIGCHLD, &sa, NULL) == -1) {
//               perror("sigaction");
//               exit(1);
//           }
//
//           while(1) {  // main accept() loop
//               sin_size = sizeof(struct sockaddr_in);
//               if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
//                                                              &sin_size)) == -1) {
//                   perror("accept");
//                   continue;
//               }
//               printf("server: got connection from %s\n",
//                                                  inet_ntoa(their_addr.sin_addr));
//               if (!fork()) { // Este es el proceso hijo
//                   close(sockfd); // El hijo no necesita este descriptor
//                   if (send(new_fd, "Holaaaaa!\n", 14, 0) == -1)
//                       perror("send");
//                   close(new_fd);
//                   exit(0);
//               }
//               close(new_fd);  // El proceso padre no lo necesita
//           }
//
//           return 0;
//       }
