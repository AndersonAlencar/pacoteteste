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
        int var[1];
        double *array_answer = (double*)malloc(sizeof(double)*LENGTH_PAC_RES); //allocating an array for sending

        rec = recv(client, var, sizeof(int), FLAGS);
        printf("Print var: %d\n\n", var[0]);
        double *package = (double*)malloc(sizeof(double)*var[0]);
        rec = recv(client, package, sizeof(double)*var[0], FLAGS);
        insert_array_in_request(request, package);

        printf("Tamanho do rec: %d\n", rec);
        if (rec != FALSE) {
            printf("Pacote Recebido!\n\n");
            operation(request, answer);

            insert_answer_in_array(answer, array_answer);
            env = (int) send(client, array_answer, sizeof(double)*LENGTH_PAC_RES, FLAGS);
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

