#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

/*
Fisier ce contine un grup de functii care ajuta la crearea
si manipularea unei cozi basic (fara prioritati).
Folosim coada normala la task 3, la modified_dijkstra
*/

// Functie care aloca memorie pt coada itself,
// cu pointerii de head si tail NULL
void init_Queue(Queue** q)
{
    *q=(Queue*)malloc(sizeof(Queue));
    (*q)->head=NULL;
    (*q)->tail=NULL;
    (*q)->len=0;
}

// functie care elibereaza memoria pt o coada
// intai se elibereaza fiecare nod, apoi coada itself
void free_Queue(Queue** q)
{
    queuenode *p;
    while((*q)->head!=NULL)
    {
        p=(*q)->head;
        (*q)->head=(*q)->head->next;
        free(p);
    }
    free(*q);
}

// functie care adauga in coada un vertex (val),
// cu masca de la momentul actual (mask)
// si cu costul pe traseul actual (cost_momentan)
int add_queue (Queue *q, int val,int mask,float cost_momentan)
{
    queuenode *new_queuenode=(queuenode *)malloc(sizeof(queuenode));
    if(new_queuenode==NULL)
        return 0;
    new_queuenode->val=val;
    new_queuenode->mask=mask;
    new_queuenode->cost_momentan=cost_momentan;
    new_queuenode->next=NULL;
    // vedem daca coada este goala
    if(q->len==0)
    {
        q->tail=q->head=new_queuenode;
    }
    else
    {
        q->tail->next=new_queuenode;
        q->tail=new_queuenode;
    }
    // crestem nr de elemente din coada
    q->len++;
    return 1;
}

// functie care returneaza valoarea (vertex-ul) din capul cozii,
// scade lungimea si elibereaza memoria pt nodul respectiv
int pop_queue(Queue *q)
{
    if(q->len>0)
    {
        queuenode *p=q->head;
        q->head=q->head->next;
        int v=p->val;
        free(p);
        q->len--;
        return v;
    }
    exit(1);
}

// verificare daca exista elemente in coada (sa vedem daca ne oprim)
int is_empty_Queue(Queue *q)
{
    if(q->len==0)
        return 1;
    return 0;
}

// verificam daca perechea {valoare, masca} exista in coada
int check_in_q(Queue *q,int val,int mask)
{
    queuenode *p=q->head;
    int i;
    for(i=0;i<q->len;i++)
    {
        if(p->mask==mask&&p->val==val)
            return 1;
        p=p->next;
    }
    return 0;
}