#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#define TYPE_REQUEST 0
#define TYPE_ANWSER 1
#define SIZE_DOUBLE 8

#define TYPE 0
#define LENGTH 1
#define ID 2
#define OPERATION 3
#define LENGTH_HEAD 4
#define LENGTH_PAC_RES 5


#define OP_ADD 0
#define OP_SUBTRACT 1
#define OP_MULTIPLY 2
#define OP_DIVIDE 3
#define HISTORY 4
#define FINISH 5
#define TOTAL 5

#define MATH_SUCESS 0
#define MATH_ERROR 1
#define SINTAX_ERROR 2

#define TRUE 1
#define FALSE 0
#define PORT 3000
#define FLAGS 0

struct sockaddr_in local;
struct sockaddr_in remote;

struct Request{
	char head[4];
	double numbers[20];
};

struct Answer{
	char head[4];
	double total;
};

struct  Cell
{
	struct Request req;
	struct Answer asn;
	struct Cell *next;
};


int config_socket(struct sockaddr_in local, struct sockaddr_in remote, int sockfd, int port);

void answer_head(struct Answer *answer, int length, int id, int op, double total);

void request_head(struct Request *request, int length, int id, int op);

void operation(struct Request *request, struct Answer *answer);

void reset_memory(struct Request *request, struct Answer *answer);

void show_data(struct Request *request, struct Answer *answer);

void generate_head(struct Request *pc, int length,int id, int p);

void save_package_req(struct Request *rq, struct Cell *cl);

void save_package_ans(struct Answer *answ, struct Cell *cl);

void insert_array(struct Request *req, int num);

void insert_struct_in_array(struct Request *req, double *arr,int tam);

void insert_array_in_struct(struct Answer *answ,double *arr);

void setup_socket(struct sockaddr_in *server, int port,char *ip);

void show_history(struct Cell *cl);

int menu();

#endif // PROTOCOL_H_INCLUDED
