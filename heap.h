// un nod din heap retine index (nr vertex-ului) si costul panala el
// + pointerii la copii si parinte
typedef struct heapnode {
    int index;
    float cost;
    struct heapnode *left, *right,*parent;
} heapnode;

// heap-ul in sine tine un pointer catre nodul radacina si nr de noduri
typedef struct Heap {
    int no_heapnodes;
    struct heapnode *root;
} Heap;

void init(Heap **h);
void dec_to_bin(int n, char *s);
void to_up(Heap *h,heapnode *p);
void insert(Heap *h,float cost,int index);
void heapify(heapnode *p);
int remove_root(Heap *h);
void search_pre_order(heapnode *h,int index,float new_dist);
