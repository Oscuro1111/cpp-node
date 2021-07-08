#ifndef __CQ__MPM__
#define __CQ__MPM__
typedef struct queue__node
{
	void *element;
	struct queue__node *next;
} Queue_Node;

typedef struct circular_queue__
{

	Queue_Node *head;
	Queue_Node *tail;
} queue;



//Interface
int cq_queue(queue *cq, void *ele);

void *cq_dqueue(queue *cq);


queue *allocate_queue();

void cq_free(queue *cq);
#endif