#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

/*
Fisier ce contine un grup de functii care ajuta la crearea
si manipularea unei stive basic.
Folosim stiva la task 2, la algoritmul lui tarjan
*/

// se initializeaza o stiva cu pointer la NULL si lungime 0
void init_Stack(Stack** s)
{
    *s=(Stack*)malloc(sizeof(Stack));
    (*s)->top=NULL;
    (*s)->len=0;
}

// functie care elibereaza memoria pt stiva
// intai se elibereaza fiecare nod, apoi stiva itself
void free_Stack(Stack** s)
{
    stacknode *p;
    while((*s)->top!=NULL)
    {
        p=(*s)->top;
        (*s)->top=(*s)->top->next;
        free(p);
    }
    free(*s);
}

// functie care adauga un element in stiva si creste lungimea ei
int add_stack(Stack *s, int val)
{
    stacknode *new_stacknode=(stacknode *)malloc(sizeof(stacknode));
    if(new_stacknode==NULL)
        return 0;
    new_stacknode->val=val;
    new_stacknode->next=s->top;
    s->top=new_stacknode;
    s->len++;
    return 1;
}

// functie care returneaza valoare din capul stivei.
// se elibereaza memoria pt nodul respectiv si scade lungimea stivei
int pop_stack(Stack *s)
{
    if(s->len>0)
    {
        stacknode *p=s->top;
        s->top=s->top->next;
        int v=p->val;
        free(p);
        s->len--;
        return v;
    }
    exit(1);
}