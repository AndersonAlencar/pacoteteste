#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "protocol.c"

int main()
{
    int sockfd, rec, env, client;
    client = config_socket(local, remote, sockfd, PORT);


    //struct Request *request = (struct Request*)malloc(sizeof(struct Request));
    //struct Answer *answer = (struct Answer*)malloc(sizeof(struct Answer));

	while(TRUE){

    	struct Request *request = (struct Request*)malloc(sizeof(struct Request));
    	struct Answer *answer = (struct Answer*)malloc(sizeof(struct Answer));
    	reset_memory(request, answer);
        rec = recv(client, request, sizeof(struct Request), FLAGS);
		printf("Tmanho do do rec: %d\n", rec);
        if (rec != FALSE) {
            printf("Pacote Recebido!\n\n");
            operation(request, answer);
            env = (int) send(client, answer, sizeof(struct Answer), FLAGS);
            show_data(request, answer);
            free(request); request = NULL;
        	free(answer); answer = NULL;
        }
        else{
        	free(request); request = NULL;
        	free(answer); answer = NULL;
        	break;
        }
     }

    printf("Encerrando protocolo!! \n");
    close(client);
    close(sockfd);
    return 0;
}
