// stiva este cea mai simpla structura folosita. Retine doar vertices,
// iar Stack-ul in sine are doar lungime si pointer la primul nod (top)

typedef struct stacknode {
    int val;
    struct stacknode* next;
} stacknode;

typedef struct Stack {
    stacknode *top;
    int len;
} Stack;

void init_Stack(Stack** s);
void free_Stack(Stack** s);
int add_stack (Stack *s, int val);
int pop_stack(Stack *s);
void print_Stack(Stack *s);
int is_empty_Stack(Stack *s);
