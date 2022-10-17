// Lista va fi primordial folosita pt a retine vecinii din graf.
// asa ca un nod va contine val (vertex-ul vecin) si cost-ul pana acolo
typedef struct node {
    int val;
    float cost;
    struct node* next;
} node;

void init_list(node **list);
void free_list(node **list);
void add_start(node **list, int val,int *verif,float cost);
int length(node *head);
void add_end(node **list, int val, int *verif,float cost);
void print_list(node *list);
int get_xth_node(node *q,int x);
float get_xth_cost(node *q,int x);
node* get_xth_pointer(node *q,int x);
int in_list(node *p,int val);