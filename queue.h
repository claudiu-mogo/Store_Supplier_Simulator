// folosim coada la task 3 pt a retine perechi de tipul:
// val -> vertex-ul in sine la care am ajuns
// mask -> masca de biti pana in starea actuala
// cost_momentan -> costul necesar pt a ajunge in starea aceasta
typedef struct queuenode{
    int val,mask;
    float cost_momentan;
    struct queuenode* next;
}queuenode;

// coada in sine retine un pointer la primul nod si unul la ultimul
// asa ne va fi usor sa adaugam si sa scoatem
// + lungimea cozii
typedef struct Queue {
    queuenode *head, *tail;
    int len;
} Queue;

void init_Queue(Queue** q);
void free_Queue(Queue** q);
int add_queue (Queue *q, int val,int mask,float cost_momentan);
int pop_queue(Queue *q);
int is_empty_Queue(Queue *q);
int check_in_q(Queue *q,int val,int mask);