#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

/*
Fisier ce contine functiile necesare pt manipularea unor liste
*/

// functie care aloca memorie prima data pt o lista
void init_list(node **list)
{
    *list=(node *)malloc(sizeof(node));
    (*list)->val=0;
    (*list)->cost=0;
    (*list)->next=NULL;
}

// functie care elibereaza memoria unei liste
// intai se elibereaza fiecare nod, apoi lista itself
void free_list(node **list)
{
    node *p1;
    while((*list)!=NULL)
    {
        p1=(*list);
        (*list)=(*list)->next;
        free(p1);
    }
    free(*list);
}

// functie care adauga un element de forma {valoare, cost} la inceputul unei liste
// verif este doar un fel de element santinela.
// El verifica daca lista e goala sau nu
void add_start(node **list, int val,int *verif,float cost)
{
    if(*verif==0)
    {
        (*list)->val=val;
        (*list)->cost=cost;
        (*verif)=1;
        return;
    }
    // Lista nu e goala si se creeaza un nod nou
    node* new_node;
    new_node=(node *)malloc(sizeof(node));
    new_node->next=NULL;
    new_node->val=val;
    new_node->cost=cost;
    new_node->next=*list;
    *list=new_node;
}

// functie care returneaza nr de elemente dintr-o lista
int length(node *head)
{
    node *p=head;
    int i=0;
    while(p!=NULL)
    {
        i++;
        p=p->next;
    }
    return i;
}


// functie care adauga un element de forma {valoare, cost} la sfarsitul unei liste
// verif are acelasi rol de checker
void add_end(node **list, int val, int *verif,float cost)
{
    if(*verif==0)
    {
        (*list)->val=val;
        (*list)->cost=cost;
        (*verif)=1;
        return;
    }
    // lista nu e vida si se creeaza nod nou
    node* new_node,*p;
    new_node=(node *)malloc(sizeof(node));
    new_node->next=NULL;
    new_node->val=val;
    new_node->cost=cost;
    p=(*list);
    // parcurgem pana la finalul listei
    for(int i=0;i<length((*list))-1;i++)
    {
        p=p->next;
    }
    p->next=new_node;
}

// functie care afiseaza elementele dintr-o lista
// dupa ultimul element nu se pune nici " ", nici \n
void print_list(node *list)
{
    node *p=list;
    while(p!=NULL)
    {
        printf("%d",p->val);
        if(p->next!=NULL)
            printf(" ");
        p=p->next;
    }
}

// functie care returneaza valoarea dintr-un nod de la indicele x
int get_xth_node(node *q,int x)
{
    int i;
    node *p=q;
    for(i=0;i<x;i++)
        p=p->next;
    return p->val;
}

// functie care returneaza costul aflat intr-un nod de la indicele x
float get_xth_cost(node *q,int x)
{
    int i;
    node *p=q;
    for(i=0;i<x;i++)
        p=p->next;
    return p->cost;
}

// functie care returneaza adresa unui nod aflat in lista la indicele x
node* get_xth_pointer(node *q,int x)
{
    int i;
    node *p=q;
    for(i=0;i<x;i++)
        p=p->next;
    return p;
}

// functie care verifica daca un anumit vertex din graf se gaseste in lista
// returneaza indicele sau -1 daca nu se gaseste
int in_list(node *p,int val)
{
    node *q=p;
    int index=0;
    while(q!=NULL)
    {
        if(val==q->val)
            return index;
        q=q->next;
        index++;
    }
    // nu am gasit elementul
    return -1;
}