#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

/*
Fisier ce contine functii necesare pt implementarea unei cozi cu priroitati,
aka heap. Il folosim la algoritmul lui dijkstra la task 1.
Heapul este de tip minheap, in functie de cost.
Heapul este reprezentat prin arbore, nu vector
*/

// se initializeaza heapul, cu radacina momentan nula si nr de noduri 0
void init(Heap **h)
{
    (*h)=malloc(sizeof(Heap));
    (*h)->root=NULL;
    (*h)->no_heapnodes=0;
}

// functie care transforma un numar in baza 2
void dec_to_bin(int n, char *s)
{
    int i;
    for(i=0;i<8;i++)
        s[i]='0';
    while(n)
    {
        s[--i]=n%2+'0';
        n/=2;
    }
    s[8]='\0';
}

// functie care aduce un nod pe pozitia care trebuie
// daca un nod este adaugat la final, trebuie sa refacem minheapul
void to_up(Heap *h,heapnode *p)
{
    // interschimbam fiul cu parintele daca fiu->cost<parinte->cost
    if(p!=h->root&&p->cost<p->parent->cost)
    {
        int aux=p->cost;
        p->cost=p->parent->cost;
        p->parent->cost=aux;
        float aux2=p->index;
        p->index=p->parent->index;
        p->parent->index=aux2;
        to_up(h,p->parent);
    }
}

// functie care adauga un element in minheap (il pune la final)
// si il aduce pe pozitia care trebuie
// indexul pe care ar trebui sa adaugam in minheap pt a ramane complet este dat 
// de scrierea in baza 2 a numarului de noduri
// 0->merge stanga; 1->merge dreapta
void insert(Heap *h,float cost,int index)
{
    if(h->root==NULL)
    {
        h->root=malloc(sizeof(heapnode));
        h->no_heapnodes=1;
        h->root->cost=cost;
        h->root->index=index;
        h->root->left=h->root->right=h->root->parent=NULL;
        return;
    }
    // aflam ce pozitie este "urmatoarea" pe care trebuie adaugat
    h->no_heapnodes++;
    char sir[9];
    dec_to_bin(h->no_heapnodes,sir);
    int i=0;
    heapnode *p=h->root;
    while(sir[i]!='1')
    {
        i++;
    }
    i++;
    // nu avem reprezentare mai mare de 8 biti, asa ca mergem pana la 7
    // si lasam ultimul bit separat pt a si adauga la el
    while(i<7)
    {
        if(sir[i]=='0')
            p=p->left;
        else
            p=p->right;
        i++;
    }
    // alocare si adaugare fiu stang sau drept in functie de ultimul bit
    // al scrierii in baza 2
    if(sir[i]=='0')
    {
        p->left=malloc(sizeof(heapnode));
        p->left->cost=cost;
        p->left->index=index;
        p->left->parent=p;
        p->left->left=p->left->right=NULL;
        to_up(h,p->left);
    }
    else
    {
        p->right=malloc(sizeof(heapnode));
        p->right->cost=cost;
        p->right->index=index;
        p->right->parent=p;
        p->right->left=p->right->right=NULL;
        to_up(h,p->right);
    }
}

// functie care reconstruieste minheapul dupa ce am scos radacina
// si am pus ultimul nod drept radacina
// acest nod trebuie sa se duca in jos
void heapify(heapnode *p)
{
    if(p==NULL)
        return;
    int l=0,r=0;
    float smallest_son=10000;
    // aflam care este fiul cel mai mic pt a interschimba cu el
    if(p->left&&p->left->cost<smallest_son)
    {
        smallest_son=p->left->cost;
        l=1;
    }
    if(p->right&&p->right->cost<smallest_son)
    {
        smallest_son=p->right->cost;
        r=1;
    }
    // interschimbam indexul si costul, iar apoi continuam recursiv
    if(r==1&&p->right->cost<p->cost)
    {
        float aux2=p->right->cost;
        p->right->cost=p->cost;
        p->cost=aux2;
        int aux=p->right->index;
        p->right->index=p->index;
        p->index=aux;
        heapify(p->right);
    }
    else if(l==1&&p->left->cost<p->cost)
    {
        float aux2=p->left->cost;
        p->left->cost=p->cost;
        p->cost=aux2;
        int aux=p->left->index;
        p->left->index=p->index;
        p->index=aux;
        heapify(p->left);
    }
}


// functie care returneaza valoarea din nodul radacina
// si inlocuieste radacina cu ultimul element din heap
// se afla pozitia ultimului element cu aceeasi schema cu bitii
int remove_root(Heap *h)
{
    if(h->root==NULL)
        return -1;
    char sir[9];
    dec_to_bin(h->no_heapnodes,sir);
    int i=0,aux,aux2;
    heapnode *p=h->root;
    // parcurgem tot heapul
    while(sir[i]!='1')
    {
        i++;
    }
    i++;
    while(i<7)
    {
        if(sir[i]=='0')
            p=p->left;
        else
            p=p->right;
        i++;
    }
    // am ajuns inainte de ultimul pas, vedem daca este stanga sau dreapta
    if(sir[i]=='0')
    {
        aux=h->root->cost;
        h->root->cost=p->left->cost;
        p->left->cost=aux;
        aux2=h->root->index;
        h->root->index=p->left->index;
        p->left->index=aux2;
        free(p->left);
        p->left=NULL;
    }
    else
    {
        aux=h->root->cost;
        h->root->cost=p->right->cost;
        p->right->cost=aux;
        aux2=h->root->index;
        h->root->index=p->right->index;
        p->right->index=aux2;
        free(p->right);
        p->right=NULL;
    }
    // se reface minheapul pornind de la radacina
    heapify(h->root);
    //scadem numarul de noduri
    h->no_heapnodes--;
    return aux2;
}

// actualizare distanta pt elementul index din heap
// cautam prin heap pt a il gasi. cand il gasim, punem distanta noua
void search_pre_order(heapnode *h,int index,float new_dist)
{
    if(h!=NULL)
    {
        if(index==h->index)
        {
            h->cost=new_dist;
            return ;
        }
        search_pre_order(h->left,index,new_dist);
        search_pre_order(h->right,index,new_dist);
    }
}