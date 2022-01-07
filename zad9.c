#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX_LINE (1024)

struct BinTree;
typedef struct BinTree * Position;
typedef struct BinTree {
    int el;
    Position L;
    Position R;
}BinaryTree;

struct _List;
typedef struct _List * ListPosition;
typedef struct _List{
    int number;
    ListPosition next;
}List;


Position Insert(int number, Position pos);
int PrintInOrder(Position pos);
int Replace(Position current);
ListPosition FindLast(ListPosition head);
int AddToList(ListPosition head, Position pos);
int AddElementToList(ListPosition head, int num);
int AddToFile(ListPosition current, char* file);
int PrintList(ListPosition head);
int FreeList(ListPosition head);
Position ClearTree(Position root);

Position Insert(int number, Position pos) //iz struktograma
{
    if(NULL == pos)
    {
        pos = (Position)malloc(sizeof(BinaryTree));
        pos->el = number;
        pos->L = NULL;
        pos->R = NULL; 
    }
    else if( number < pos->el )
    {
        pos->R = Insert( number, pos->R );
    }
    else if( number >= pos->el )
    {
        pos->L = Insert( number, pos->L );
    }
    return pos;
}

int PrintInOrder(Position pos) //od najvećeg do najmanjeg
{
    if(NULL == pos)
        return 0;

    PrintInOrder(pos->L);

    printf("%d ", pos->el);

    PrintInOrder(pos->R);

    return 0;
}

int Replace(Position current)
{

    if(current->L == NULL && current->R == NULL) // ako ovaj cvor nema djece, suma vrijednosti djece ce bit 0
        current->el = 0;

    else if(current->L == NULL && current->R != NULL) //ako ima samo desno dijete, zbrojit cemo trenutnu vrijednost tog desnog i plus ono sta nakon replace-a dode na njegovo mjesto
        current->el = current->R->el + Replace(current->R);

    else if(current->L != NULL && current->R == NULL) // ako ima samo lijevo dijete, isti postupak
        current->el = current->L->el + Replace(current->L);

    else
        current->el = current->L->el + current->R->el + Replace(current->L) + Replace(current->R); // ako ima oba djeteta, zbrajamo njihove vrijednosti + jos vrijednosti djece nakon replace-a

    return current->el; 
}


ListPosition FindLast(ListPosition head){

    ListPosition temp = head;

    while(temp->next != NULL)
        temp = temp->next;
        
    return temp;

}

int AddToList(ListPosition head, Position pos) // dodajemo elemente stabla od najveceg do najmanjeg u listu, pa cemo ih iz liste upisat u dokument
{
    //ideja je ista ko u InOrder printanju na ekran
    if(NULL == pos)
        return 0;
    AddToList(head, pos->L);

    AddElementToList(head, pos->el); // tu se radi korak, kao sto je tamo bilo pritanje, tu je dodavanje

    AddToList(head, pos->R);

    return 0;
}

int AddElementToList(ListPosition head, int n)
{
    ListPosition temp = NULL;
    ListPosition last = NULL;

    temp = (ListPosition)malloc(sizeof(List));
    if(!temp)
    {
        printf("Couldn't allocate memory.");
        return 0;
    }
    temp->number = n;

    last = FindLast(head);
    //ubacujemo svaki novi elemnt na kraj liste
    temp->next = last->next; 
    last->next = temp;

}

int AddToFile(ListPosition head, char* file)
{ 
    ListPosition temp = head;
    FILE *dat = NULL;
    dat = fopen(file, "a"); //"a", a ne "w", jer s "a" mozemo dodavat vise puta na isti dokumnet

    while(temp->next!= NULL){
        fprintf(dat, "%d ", temp->next->number);
        temp = temp->next;
    }

    fprintf(dat, "\n");

    fclose(dat);
    return 0;
}

int PrintList(ListPosition head)
{
    ListPosition temp = head;

    while(temp->next!= NULL)
    {
        printf("%d ", temp->next->number);
        temp=temp->next;
    }
    return 0;
}

int FreeList(ListPosition head)
{
    ListPosition del = NULL;
    while(head->next != NULL)
    {
        del = head->next;
        head->next=del->next;
        free(del);

    }
}

Position ClearTree(Position pos)
{
    if(pos != NULL)
    {    
        ClearTree(pos->L);
        ClearTree(pos->R);
        free(pos);
    }

    return NULL;
}


int main()
{
    int array[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
    int i = 0, n;
    char filename[MAX_LINE];
    Position root = NULL;
    List head = {.number = 0, .next = NULL};

    for(i = 0; i < sizeof(array)/sizeof(int); i++)
    {
        root = Insert(array[i], root);
    }
    //a
    printf("\nFile name: ");
    scanf("%s", filename);

    printf("Binary tree, in order: ");
    PrintInOrder(root);

    AddToList(&head, root);
    AddToFile(&head, filename);

    FreeList(&head); 

    //b
    Replace(root);

    printf("\nReplaced binary tree, in order: ");
    PrintInOrder(root);

    AddToList(&head, root);
    AddToFile(&head, filename);

    //c 
    srand((unsigned) time(NULL));
    printf("\nHow many numbers in a tree: ");
    scanf("%d", &n);
    
    root = ClearTree(root);

    for(i = 0; i < n; i++)
    {
        root = Insert(rand()%(90-10+1) + 10, root); 
    }

    printf("\nRandomized binary tree, in order: ");
    PrintInOrder(root);

    return 0;
}
