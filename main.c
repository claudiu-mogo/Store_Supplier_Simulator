#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#include "stack.h"
#include "lists.h" 
#include "queue.h"

/*
Fisierul principal care contine functiile principale pt rezolvarea tuturor taskurilor.
Se includ headerele pt a apela functii externe de liste, stack, cozi si heap.
In main se realizeaza citirea, parsarea elementelor date ca input, crearea grafurilor
si lansarea in executie a functiilor aferente fiecarui task.
*/

// tinem grafuri ce au vecinii cu liste de adiacenta
// fiecare nod retine indexul vecinului si costul pana acolo
typedef struct graph {
    int nr_nodes;
    struct node** neighbours;
} graph;

// functie de comparatre generica pt qsort
int compare(const void *a, const void *b) 
{
   return (*(int*)a-*(int*)b);
}

// functie care sorteaza un array de int-uri
void sort_array(int *arr, int len)
{
    qsort(arr,len,sizeof(int),compare);
}

// functie care returneaza minimul dintre 2 int-uri.
// folosim functia de comparare generica
int minim(int a, int b)
{
    return (compare(&a,&b)>0)?b:a;
}

// cauta un element intr-un array de int-uri
// returneaza 1 daca am gasit, 0 in rest
int in_array(int *arr,int val,int len)
{
    for(int i=0;i<len;i++)
        if(arr[i]==val)
            return 1;
    return 0;
}

// functie care returneaza indexul unui element dintr-un vector
// returneaza -1 daca nu s-a gasit
int index_in_array(int *arr, int len, int val)
{
    int i;
    for(i=0;i<len;i++)
    {
        if(arr[i]==val)
            return i;
    }
    return -1;
}

// functie care creeaza recursiv o lista a verticeslor prin care s-a trecut pt a ajunge
// la destinatia finala.
// argumentul verif are ca scop verificarea daca lista este goala sau nu (daca e 0 sau nu)
// Se merge din parinte in parinte pana cand se ajunge la elementul cu parinte -1 (sursa)
void shortest_path(int *parent,int val,node **go_through_list,int *verif)
{
    if(parent[val]>=0)
    {
        // facem lista in ordinea sursa -> destinatie,
        // intai apelam recursiv, apoi adaugam la final in lista
        shortest_path(parent,parent[val],go_through_list,verif);
        add_end(go_through_list,val,verif,0);
        (*verif)++;
    }
}

// functia principala aferenta task-ului 1. Primeste graful, nr de vertices, sursa si destinatia
// Intoarce costul total al calatoriei si lista de noduri prin care se trece
// dijkstra a fost implementat cu heap binar minheap
float dijkstra(graph *g,int n,int source,int destination,node **go_through_list,int *verificare)
{
    Heap *h;
    int i;
    init(&h);
    int *visited,*parent,x,nr_neigh,nod;
    float *dist;
    visited=malloc(n*sizeof(int));
    parent=malloc(n*sizeof(int));
    dist=malloc(n*sizeof(float));
    // initializam distantele ca fiind un numar mare inexistent
    // initial toate sunt considerate ca fiin surse (parinte -1)
    // si marcam nevizitate (poz=0)
    for(i=0;i<n;i++)
    {
        dist[i]=10000;
        visited[i]=0;
        parent[i]=-1;
    }
    // distanta de la sursa la ea insasi este 0 si o marcam vizitata
    dist[source]=0;
    visited[source]=1;
    // for the sake of the algorithm (pt ca trebuie sa dam remove la un nod),
    // adaugam in heap sursa
    insert(h,0,source);
    // pana cand nu mai e nimic in "coada cu prioritati"
    while(h->no_heapnodes!=0)
    {
        // dam remove la nodul prioritar
        if(h->no_heapnodes==1)
        {
            nod=h->root->index;
            free(h->root);
            free(h);
            init(&h);
        }
        else
            nod=remove_root(h);
        // pt fiecare vecin al nodului scos verificam daca se poate actualiza distanta
        nr_neigh=length(g->neighbours[nod]);
        for(x=0;x<nr_neigh;x++)
        {
            if(dist[get_xth_node(g->neighbours[nod],x)]>dist[nod]+get_xth_cost(g->neighbours[nod],x))
            {
                // actualizam distantele si notam ca parinte momentan nod-ul scos din heap
                dist[get_xth_node(g->neighbours[nod],x)]=dist[nod]+get_xth_cost(g->neighbours[nod],x);
                parent[get_xth_node(g->neighbours[nod],x)]=nod;
                // vedem daca nodul a mai fost vizitat
                // daca nu, il bagam in heap si marcam vizitat
                if(!visited[get_xth_node(g->neighbours[nod],x)])
                {
                    visited[get_xth_node(g->neighbours[nod],x)]=1;
                    insert(h,dist[get_xth_node(g->neighbours[nod],x)],
                                        get_xth_node(g->neighbours[nod],x));
                }
                else
                // daca a mai fost vizitat, il cautam in heap si actualizam distanta
                    search_pre_order(h->root,get_xth_node(g->neighbours[nod],x),
                                                dist[get_xth_node(g->neighbours[nod],x)]);
            }
        }
    }
    // afisam distanta pana la destinatie
    printf("%.1f",dist[destination]);
    // generam lista de noduri prin care s-a trecut pe baza vectorului de parinti
    shortest_path(parent,destination,go_through_list,verificare);
    float to_save_dist=dist[destination];
    // eliberare memorie heap si vectori
    free(h);
    free(visited); free(parent); free(dist);
    // intoarcem distanta unui drum
    return to_save_dist;
}

// am implementat algoritmul lui tarjan, luat dupa scheletul din laborator
// finalitatea este in a crea un vector de liste (components)
// fiecare lista va contine elementele dintr-o componenta conexa
// realocam vectorul de fiecare data cand gasim o noua componenta tare conexa
// concomitent aflam si numarul de ctc
void tarjan(int nod,int *discovery,int *low_link,Stack **s,int *in_stack,graph *g,
                                                    int *ctc,int *index,node ***components)
{
    int element_scos;
    discovery[nod]=low_link[nod]=*index;
    (*index)++;
    // adaugam in stiva nodul curent
    add_stack(*s,nod);
    in_stack[nod]=1;
    // verifica daca vecinii nodului au fost vizitati
    for(int v=0;v<length(g->neighbours[nod]);v++)
    {
        if(discovery[get_xth_node(g->neighbours[nod],v)]==-1)
        {
            tarjan(get_xth_node(g->neighbours[nod],v),discovery,low_link,s,in_stack,
                                                                    g,ctc,index,components);
            // low_link -> cel mai mic indice cu care nodul are legatura
            low_link[nod]=minim(low_link[nod],low_link[get_xth_node(g->neighbours[nod],v)]);
        }
        else if(in_stack[get_xth_node(g->neighbours[nod],v)])
        {
            // daca nodul e in stiva, vedem cand a fost descoperit
            low_link[nod]=minim(low_link[nod],discovery[get_xth_node(g->neighbours[nod],v)]);
        }
    }
    if(low_link[nod]==discovery[nod])
    {
        // doar cate un nod va avea aceasta proprietate per componenta
        (*ctc)++;
        // mai adaugam o lista la vector
        (*components)=realloc(*components,(*ctc)*sizeof(node*));
        init_list(&(*components)[(*ctc)-1]);
        int size_component=1,verif_comp=0;
        // completam lista de noduri din componenta curenta
        do{
            element_scos=pop_stack(*s);
            in_stack[element_scos]=0;
            add_end(&(*components)[(*ctc)-1],element_scos,&verif_comp,0);
            size_component++;
        }while(element_scos!=nod);
    }
}

// functia principala a taskului 2
void e2(graph *g,int n,node *depozite)
{
    int *discovery,*low_link,*in_stack,i,ctc=0,index=1,j,k;
    Stack *s;
    node *p=depozite;
    node **components;                 // vectorul de liste
    components=malloc(sizeof(node*));
    discovery=malloc(n*sizeof(int));
    low_link=malloc(n*sizeof(int));
    in_stack=calloc(n,sizeof(int));     // la inceput nu este niciun nod in stack
    init_Stack(&s);
    // niciun nod nu este descoperit si nu are legaturi
    for(i=0;i<n;i++)
    {
        discovery[i]=-1;
        low_link[i]=-1;
    }
    // pt a nu trece prin depozite, le marcam ca vizitate
    for(i=0;i<length(depozite);i++)
    {
        discovery[p->val]=1;
        p=p->next;
    }
    for(i=0;i<n;i++)
    {
        // lansam tarjan daca nu a fost vizitat un nod
        if(discovery[i]==-1)
            tarjan(i,discovery,low_link,&s,in_stack,g,&ctc,&index,&components);
    }
    // vom avea ctc elemente in vectorul de componente
    // sortam cu metoda de neanderthali
    // intai sortam fiecare lista in parte
    for(index=0;index<ctc;index++)
    {
        for(j=0;j<length(components[index]);j++)
        {
            for(k=0;k<length(components[index]);k++)
            {
                if(get_xth_node(components[index],j)<get_xth_node(components[index],k))
                {
                    int aux=(get_xth_pointer(components[index],j))->val;
                    (get_xth_pointer(components[index],j))->val=
                                        (get_xth_pointer(components[index],k))->val;
                    (get_xth_pointer(components[index],k))->val=aux;
                }
            }
        }
    }
    //sortam vectorul de liste dupa primul element din fiecare lista
    for(j=0;j<ctc;j++)
    {
        for(k=0;k<ctc;k++)
        {
            if(components[j]->val<components[k]->val)
            {
                node *aux_node=components[j];
                components[j]=components[k];
                components[k]=aux_node;
            }
        }
    }
    // afisari nr ctc si fiecare lista de componente in parte
    printf("%d\n",ctc);
    for(i=0;i<ctc-1;i++)
    {
        print_list(components[i]);
        printf("\n");
    }
    print_list(components[i]);
    // eliberare memorie
    free_Stack(&s);
    for(i=0;i<ctc;i++)
        free_list(&components[i]);
    free(components); free(discovery); free(low_link); free(in_stack);
}

// functie aferenta task-ului 3. Intoarce un cost al calatoriei.
// am implementat algoritmul cu cozi si masti. 
// se parcurge fiecare nod al subgrafului g, iar masca ce decide daca s-au
// vizitat toate nodurile trebuinicoase este calculata doar pe baza nedepozitelor.
// in vectorul de liste de vecini intai se afla n nedepozite si depo depozite
float modified_dijkstra(graph *g,int n,int depo,int source,int destination)
{
    Queue *actual_q, *was_in_q;
    float sum=10000,new_cost,cost_imediat;
    // masca inseamna full nedepozite
    int total_mask=(1<<n)-1;
    int new_mask,x,masca,nod,nr_neigh;
    init_Queue(&actual_q);      // coada in sine
    init_Queue(&was_in_q);      // retinem starile cu care ne-am mai confruntat
    add_queue(actual_q,source,1<<source,0);     // primul element
    // cat timp mai sunt stari posibile in coada
    while(!is_empty_Queue(actual_q))
    {
        // un pop este format din 3 bucati: masca, valoare nod si costul curent
        masca=actual_q->head->mask;
        cost_imediat=actual_q->head->cost_momentan;
        nod=pop_queue(actual_q);
        // verificam daca am fost in toate nodurile si am ajuns la destinatie
        if(((masca&total_mask)==total_mask)&&nod==destination)
        {
            if(cost_imediat<sum)
                sum=cost_imediat;
        }
        else
        {   // daca dam de o stare pe care nu am mai vizitat-o
            if(!check_in_q(was_in_q,nod,masca))
            {
                nr_neigh=length(g->neighbours[nod]);
                // bagam vecinii in coada, actualizand masca si costul
                for(x=0;x<nr_neigh;x++)
                {
                    new_mask=masca|(1<<(get_xth_node(g->neighbours[nod],x)));
                    new_cost=cost_imediat+get_xth_cost(g->neighbours[nod],x);
                    add_queue(actual_q,get_xth_node(g->neighbours[nod],x),new_mask,new_cost);
                }
            }
            add_queue(was_in_q,nod,masca,cost_imediat);
        }
    }
    // eliberare memorie
    free_Queue(&actual_q);
    free_Queue(&was_in_q);
    return sum;
}

// functie care construieste un subgraf dintr-un graf dat.
// nodes_e3 contine intai k nedepozite, apoi k depozite
// nedepozitele sunt sortate in ordine crescatoare a indicilor, iar depozitele descrescator
void generate_new_graph(int *nodes_e3,int k,int d,int *verif,graph *new_graph,graph *g,int n)
{
    int i,j,current_index=0,get_index;
    // contruim listele de adiacenta pt noduri
    for(i=0;i<n;i++)
    {
        if(in_array(nodes_e3,i,k)) // nodurile sunt pana in indicele k din nodes_e3
        {
            for(j=0;j<length(g->neighbours[i]);j++)
            {
                // vecinii pot fi atat noduri, cat si depozite
                if(in_array(nodes_e3,get_xth_node(g->neighbours[i],j),k+d)) 
                {
                    get_index=index_in_array(nodes_e3,k+d,get_xth_node(g->neighbours[i],j));
                    add_end(&new_graph->neighbours[current_index],get_index,
                                &verif[current_index],get_xth_cost(g->neighbours[i],j));
                }
            }
            current_index++;
        }
    }
    // contruim listele de adiacenta pt depozite
    for(i=n-1;i>=0;i--)
    {
        // depozitele se gasesc intre indicii k si d
        if(in_array(nodes_e3+k,i,d))
        {
            for(j=0;j<length(g->neighbours[i]);j++)
            {
                if(in_array(nodes_e3,get_xth_node(g->neighbours[i],j),k+d))
                {
                    // punem si costul muchiei
                    get_index=index_in_array(nodes_e3,k+d,get_xth_node(g->neighbours[i],j));
                    add_end(&new_graph->neighbours[current_index],get_index,
                                &verif[current_index],get_xth_cost(g->neighbours[i],j));
                }
            }
            current_index++;
        }
    }
}

// functie care calculeaza suma unei zone
// analizam toate cazurile posibile (sa plece si sa se intoarca in toate depozitele),
// pe toate nodurile posibile (atat la dus, cat si la intors)
// nodes_e3 -> vector ce contine nodurile din componenta (k) si toate depozitele (d).
// depozitele sunt stocate si in lista depozite.
// new_graph -> subgraful curent; graph -> graful mare
float suma_componenta(int *nodes_e3,int k,int d,graph *new_graph,graph *g,int n,node *depozite)
{
    // cum luam toate cazurile, o sa retinem costul momentan in case_sum
    // suma cautata este in total_sum
    float case_sum=0,total_sum=10000,save_sum;
    int i,j,s,sursa,destinatie,get_index,depozit_curent;
    // pornim din toate depozitele
    for(i=0;i<d;i++)
    {
        depozit_curent=get_xth_node(depozite,i);
        // aflam sursa
        for(j=0;j<length(g->neighbours[depozit_curent]);j++)
        {
            // vedem din vecinii depozitelor care sunt noduri (nedepozite) in subgraf
            if(in_array(nodes_e3,get_xth_node(g->neighbours[depozit_curent],j),k))
            {
                // luam pe rand cate o sursa
                sursa=index_in_array(nodes_e3,k,get_xth_node(g->neighbours[depozit_curent],j));
                case_sum=get_xth_cost(g->neighbours[depozit_curent],j);
                // aflam destinatia
                for(s=0;s<k;s++)
                {
                    save_sum=case_sum;
                    if(in_list(g->neighbours[nodes_e3[s]],depozit_curent)>=0)
                    {
                        // pot exista mai multe destinatii, luam fiecare caz in parte
                        destinatie=s;
                        get_index=in_list(g->neighbours[nodes_e3[s]],depozit_curent);
                        case_sum+=get_xth_cost(g->neighbours[nodes_e3[s]],get_index);
                        case_sum+=modified_dijkstra(new_graph,k,d,sursa,destinatie);
                        // verificam daca s-a gasit un drum mai scurt
                        if(case_sum<total_sum)
                            total_sum=case_sum;
                        // scadem datele pt destinatia curenta si cautam alta cu aceeasi sursa
                        case_sum=save_sum;
                    }
                }
            }
        }
    }
    return total_sum;
}

// main care creeaza grafuri si lanseaza in executie task-urile ori de cate ori e nevoie
// am incercat sa pastrez, pe cat posibil, notatiile cu litere din enunt
int main()
{
    int i,k,n,m,d,u,v,s,r,j,*verif,dep,nr_verif=0,nr_cerinte,cer;
    node *depozite,*go_through_list;
    float w;
    char sir_cerinta[4];    // retine structuri de forma "e1" "e2"
    // citim nr de varfuri, muchii si depozite
    scanf("%d%d%d",&n,&m,&d);
    graph *g,*new_graph;
    g=malloc(sizeof(graph));
    g->neighbours=malloc(n*sizeof(node*));
    verif=malloc(n*sizeof(int));
    // tinem grafuri cu liste de adiacenta, le initializam si marcam listele ca fiind goale
    // verif=0 inseamna ca lista e goala
    for(i=0;i<n;i++)
    {
        init_list(&g->neighbours[i]);
        verif[i]=0;
    }
    // citim fiecare muchie in parte si o adaugam in lista de vecini corespunzatoare
    for(i=0;i<m;i++)
    {
        fscanf(stdin,"%d%d%f",&u,&v,&w);
        add_start(&g->neighbours[u],v,&verif[u],w);
    }
    free(verif);
    init_list(&depozite);
    // facem o lista de depozite (punem costul 0, nu ne intereseaza, dar structura are si cost)
    for(i=0;i<d;i++)
    {
        scanf("%d",&dep);
        add_start(&depozite,dep,&nr_verif,0);
        nr_verif++;
    }
    // citim nr de cerinte si le luam pe rand
    scanf("%d",&nr_cerinte);
    for(cer=0;cer<nr_cerinte;cer++)
    {
        getchar();      // aveam un \n ramas in buffer
        // citim sirul care reprezinta nr cerintei
        fgets(sir_cerinta,4,stdin);
        // apelam functii conform numarului din sir
        switch (sir_cerinta[1])
        {
            case '1':
            {
                // task 1
                float total_sum=0;      // suma totala a tuturor drumurilor
                // citim sursa si nr de noduri la care vrem sa ajungem
                scanf("%d%d",&s,&k);
                for(i=0;i<k;i++)
                {
                    // citim depozitul destinatie
                    scanf("%d",&u);
                    printf("%d\n",u);
                    // cream o lista goala de noduri prin care se trece
                    init_list(&go_through_list);
                    int verificare=0;
                    // punem sursa in lista go_through
                    add_end(&go_through_list,s,&verificare,0);
                    // facem 2 drumuri: de la sursa la destinatie
                    // si de la destinatie inapoi la sursa
                    // in functia dijkstra se printeaza costul
                    total_sum+=dijkstra(g,n,s,u,&go_through_list,&verificare);
                    printf(" ");
                    total_sum+=dijkstra(g,n,u,s,&go_through_list,&verificare);
                    printf("\n");
                    // se printeaza lista de noduri prin care s-a trecut
                    print_list(go_through_list);
                    printf("\n");
                    free_list(&go_through_list);
                }
                // afisam costul total al traseului
                if(cer==nr_cerinte-1)
                    printf("%.1f",total_sum);
                else
                    printf("%.1f\n",total_sum);
                break;
            }
            case '2':
            {
                // lansam in executie functia care cauta componentele tare conexe
                e2(g,n,depozite);
                break;
            }
            case '3':
            {
                // task 3
                // citim nr de zone
                scanf("%d",&r);
                for(j=0;j<r;j++)
                {
                    // citim nr de elemente din componenta
                    scanf("%d",&k);
                    // nodes_e3 contine nodurile si depozitele din componenta
                    int *nodes_e3;
                    nodes_e3=malloc((k+d)*sizeof(int));
                    // adaugam nodurile in sine
                    for(i=0;i<k;i++)
                        scanf("%d",&nodes_e3[i]);
                    // adaugam in vectorul de trebuincioase depozitele
                    for(;i<k+d;i++)
                        nodes_e3[i]=get_xth_node(depozite,i-k);
                    // luam mereu indicii in ordine crescatoare pt nedepozite
                    sort_array(nodes_e3,k);
                    // depozitele o sa fie de la indicele k incolo in ordine descrescatoare
                    // generam un graf nou doar cu nodurile selectate si toate depozitele
                    new_graph=malloc(sizeof(graph));
                    new_graph->neighbours=malloc((k+d)*sizeof(node*));
                    verif=malloc((k+d)*sizeof(int));
                    for(i=0;i<k+d;i++)
                    {
                        init_list(&new_graph->neighbours[i]);
                        verif[i]=0;
                    }
                    // facem concret graful
                    generate_new_graph(nodes_e3,k,d,verif,new_graph,g,n);
                    // suma pt componenta actuala
                    float total_sum=suma_componenta(nodes_e3,k,d,new_graph,g,n,depozite);
                    if(j==r-1)
                        printf("%.1f",total_sum);
                    else
                        printf("%.1f\n",total_sum);
                    //free-uri (facem cate un graf nou pt fiecare cerinta)
                    for(i=0;i<k+d;i++)
                    {
                        free_list(&new_graph->neighbours[i]);
                        free(new_graph->neighbours[i]);
                    }
                    free(new_graph->neighbours);
                    free(new_graph);
                    free(verif);
                    free(nodes_e3);
                }
            }
            default:
                break;
        }
    }
    // eliberare memorie vectori si graf (cu tot cu vecini)
    free_list(&depozite);
    for(i=0;i<n;i++)
    {
        free_list(&g->neighbours[i]);
        free(g->neighbours[i]);
    }
    free(g->neighbours);
    free(g);
    return 0;
}