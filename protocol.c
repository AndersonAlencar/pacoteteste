#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <endian.h>
#include "protocol.h"

void answer_head(struct Answer *answer, int length, int id, int op, double total){
    answer->head[TYPE] = TYPE_ANWSER;
    answer->head[LENGTH] = length*sizeof(double); /*Tamanho do pacote*/
    answer->head[ID] = id; /*Identificador da operação(o mesmo do pacote requisição)*/
    answer->head[OPERATION] = op; /*Status da operaçao*/
    answer->total = total;
}

void operation(struct Request *request, struct Answer *answer){
    int id = request->head[ID], zero = FALSE, len = request->head[LENGTH]/8 /*qnte de numbers*/, i;
    double total = request->numbers[0];
	if(len != 1){
    	switch(request->head[OPERATION]){
		    case OP_ADD: /*Operação de adição*/
		        for(i=1; i<len; i++){
		                total += request->numbers[i];
		            }
                answer_head(answer, request->head[LENGTH], id, MATH_SUCESS, total);
                break;

		    case OP_SUBTRACT: /*Operação de subtração*/
		        for(i=1; i<len; i++){
		                total -= request->numbers[i];
                    }
                answer_head(answer, request->head[LENGTH], id, MATH_SUCESS, total);
                break;

		    case OP_MULTIPLY: /*Operação de multiplicação*/
		        for(i=1; i<len; i++){
		                total *= request->numbers[i];
		            }
		            answer_head(answer, request->head[LENGTH], id, MATH_SUCESS, total);
		            break;

		    case OP_DIVIDE: /*Operação de divisão*/
				for(i=1; i<len; i++){
					if(request->numbers[i] == 0){ //caso tenha zero
						zero = TRUE;
						break;
					}
					else{
		                total /= request->numbers[i];
					}
				}

                if(zero == FALSE){
                    answer_head(answer, request->head[LENGTH], id, MATH_SUCESS, total);
                }else{
		            answer_head(answer, request->head[LENGTH], id, MATH_ERROR, total);
                }
                break;
		}
	}else answer_head(answer, request->head[LENGTH], id, SINTAX_ERROR, total);
}

void reset_memory(struct Request *request, struct Answer *answer){
   memset(request->head,0,sizeof(request->head));
   memset(request->numbers,0,sizeof(request->numbers));
   memset(answer->head,0,sizeof(answer->head));
   answer->total = 0;
}

void show_data(struct Request *request, struct Answer *answer){
    printf("HEAD Requisição:\n %d (Tipo Requisição) %d (Tamanho Bytes) %d (ID)\n",request->head[TYPE],request->head[LENGTH],request->head[ID]);
    if(answer->head[OPERATION] == MATH_SUCESS){
        printf("Tipo da Operação: ");
        switch(request->head[OPERATION]){
            case OP_ADD: printf("Adição\n"); break;
            case OP_SUBTRACT: printf("Subtração\n"); break;
            case OP_MULTIPLY: printf("Multiplicação\n"); break;
            case OP_DIVIDE: printf("Divisão\n"); break;
        }
        printf("Números: ");
        int len = request->head[LENGTH]/8;
        for(int j=0; j<len; j++)
            printf("%.2lf ", request->numbers[j]);
        printf("\nResposta: %.2lf \n\n", answer->total);
    }
    else if(answer->head[OPERATION] == MATH_ERROR) printf("Impossibilidade Matemática\n\n");
    else printf("Erro de Sintaxe\n\n");
    printf("Aguardando novo pacote...\n");
}

int config_socket(struct sockaddr_in local, struct sockaddr_in remote, int sockfd, int port){
    socklen_t len = sizeof(remote);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    memset(local.sin_zero, 0x0, 8);

    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local))==-1){
        perror("bind ");
        exit(1);
    }

     if(listen(sockfd, 1)==-1){
		perror("listen ");
		exit(1);
	}

	printf("Aguardando requisição...\n");

    int client = accept(sockfd, (struct sockaddr *)&remote, &len);
    if(client==-1){
        perror("accept ");
        exit(1);
    }
    printf("Requisição conectado! Aguardando pacote...\n");
	return client;
}





void generate_head(struct Request *pc, int length,int id, int Op)
{
    pc->head[TYPE] = TYPE_REQUEST;
    pc->head[LENGTH] = length * SIZE_DOUBLE;
    pc->head[ID] = id;
    pc->head[OPERATION] = Op;
}

void save_package_req(struct Request *rq, struct Cell *cl)
{
    cl->req = *rq;
}

void save_package_ans(struct Answer *answ, struct Cell *cl)
{
    cl->asn = *answ;
    /*if(answ->head[OPERATION] == MATH_SUCESS)
    {
        cl->asn = *answ;
    }
    */
}

void show_package_ans(struct Answer *answ)
{
    if( answ->head[OPERATION] == MATH_SUCESS)
    {
        printf("\nResposta: %.2lf\n\n",answ->total);

    }
    else if(answ->head[OPERATION] == MATH_ERROR)
    {
        printf("\nResposta: Impossibilidade Matemática!\n");
    }
    else if (answ->head[OPERATION] == SINTAX_ERROR)
    {
        printf("\nResposta: Error de Sintaxe!\n");
    }

}

void insert_array(struct Request *req, int num)
{
    double valor;
    int i;
    for(i = 0; i<num ; i++)
    {
        printf("Digite o %d° numero:  ",i+1);
        scanf("%lf",&valor);
        req->numbers[i] = valor;
    }
}

void insert_struct_in_array(struct Request *req, double *arr,int tam)
{
    int i,k = 0;
    arr[TYPE] = req->head[TYPE];
    arr[LENGTH] = req->head[LENGTH];
    arr[ID] = req->head[ID];
    arr[OPERATION] = req->head[OPERATION];
    for(i = 4;i<tam;i++)
    {
        arr[i] = req->numbers[k];
        k += 1;
    }
}

void  insert_array_in_struct(struct Answer *answ,double *arr)
{
    answ->head[TYPE] = arr[TYPE];
    answ->head[LENGTH] = arr[LENGTH];
    answ->head[ID] = arr[ID];
    answ->head[OPERATION] = arr[OPERATION];
    answ->total = arr[TOTAL];

}

void setup_socket(struct sockaddr_in *server, int porta,char *ip)
{
    server->sin_family = AF_INET;
    server->sin_port = htons(porta);
    inet_pton(AF_INET, ip, &remote.sin_addr);
    memset(server->sin_zero,0x0,8);
}

void show_history(struct Cell *cl)
{
    int i;
    do
    {
        for(i = 0; i< cl->req.head[LENGTH]/SIZE_DOUBLE - 1 ; i++)
        {
            if(cl->req.head[OPERATION] == OP_ADD)
            {
                printf("%.2lf+",cl->req.numbers[i]);
            }
            else if(cl->req.head[OPERATION] == OP_SUBTRACT)
            {
                printf("%.2lf-",cl->req.numbers[i]);
            }
            else if(cl->req.head[OPERATION] == OP_MULTIPLY)
            {
                printf("%.2lfx",cl->req.numbers[i]);
            }
            else if(cl->req.head[OPERATION] == OP_DIVIDE)
            {
                printf("%.2lf/",cl->req.numbers[i]);
            }
        }
        if(cl->asn.head[OPERATION] == MATH_SUCESS)
        {
            printf("%.2lf ",cl->req.numbers[i]);
            printf(" = %.2lf\n",cl->asn.total);
        }

        else if(cl->asn.head[OPERATION] == MATH_ERROR || cl->asn.head[OPERATION] == SINTAX_ERROR)
        {
            printf("%.2lf ",cl->req.numbers[i]);
            printf(" = Error na Resposta.\n");
        }


        cl = cl->next;


    }while(cl != NULL);


}

int menu()
{
    int op;
    printf("(------------CALCULADORA--------------)\n");
    printf("0 - Somar \n");
    printf("1 - Subtrair  \n");
    printf("2 - Multiplicar \n");
    printf("3 - Dividir \n");
    printf("4 - Historico\n");
    printf("5 - Sair da aplicação\n");
    printf("Selecione a operação: ");
    scanf("%d", &op);
    return op;
}

