#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE (1024)


struct _List;
typedef struct _List* PositionList;
typedef struct _List
{
    int br;
    char city[MAX_LINE];
    PositionList next;
}List;

struct _Tree;
typedef struct _Tree* PositionTree;
typedef struct _Tree{
    char country[MAX_LINE];
    PositionList head;
    PositionTree left;
    PositionTree right;
}Tree;


PositionTree ReadFromFile(char *filename, PositionTree root);
int ReadCitiesFromFile(char* filename, PositionList head);
PositionList CreateNew(char* name, int brStan);
int SortList(PositionList new, PositionList head);
int InsertAfter(PositionList temp, PositionList new);
PositionList FindBefore(PositionList find, PositionList head);
PositionTree SortCountry(PositionTree current, PositionTree temp );
PositionList InitializeList();
int PrintList(PositionList head);
int PrintTree(PositionTree current);

int BiggerBetterStronger(char* country, int number, PositionTree root);
PositionTree FindCountry(char* country, PositionTree current);

PositionTree ReadFromFile(char *filename, PositionTree root)
{
    char buffer[MAX_LINE] = { 0 };
    char fileCity[MAX_LINE] = { 0 };
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionTree tempTree = NULL;

    if(!pDat)
    {
        perror("Cannot open the file!\n");
		return NULL;
    }

    while(!feof(pDat))
    {  
        tempTree = NULL;
        tempTree = (Tree*)malloc(sizeof(Tree));
        tempTree->left= NULL;
        tempTree->right=NULL;
        tempTree->head = InitializeList();

        fgets(buffer, MAX_LINE, pDat);
 
        if(sscanf(buffer, " %s %s",  tempTree->country, fileCity) == 2)
        {
            ReadCitiesFromFile(fileCity, tempTree->head);
            
            root = SortCountry(root, tempTree);
        }
    }

    fclose(pDat);

    return root;
}

int ReadCitiesFromFile(char* filename, PositionList head) 
{
    
    char buffer[MAX_LINE] = { 0 };
    char name[MAX_LINE] = { 0 };
    int brStanovnika = 0;
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionList new = NULL;

    if(!pDat)
    {
        perror("Greska pri otvaranju datoteke\n");
		return -1;
    }

    while(!feof(pDat))
    {  
        new= NULL;
        fgets(buffer, MAX_LINE, pDat);
        if(sscanf(buffer, " %s %d", name, &brStanovnika) == 2)
        {
            new = CreateNew(name, brStanovnika);
            SortList(new, head);
            
        }
    }
    
    fclose(pDat);
    
}

PositionList CreateNew(char* name, int brStan)
{
    PositionList new = NULL;
    new = (List*)malloc(sizeof(List));
    strcpy(new->city,name);
    new->br = brStan;
    new->next = NULL;

    return new;
}

int SortList(PositionList new, PositionList head)
{
    
    PositionList temp = head;
    
    while(temp->next!=NULL && temp->next->br < new->br)
    {
        temp = temp->next;
    }
    if(temp->next == NULL || temp->next->br != new->br)
    {
        InsertAfter(temp, new);
    }
    else if(temp->next->br == new->br)
    {
        if(strcmp(temp->next->city, new->city) < 0)
            InsertAfter(temp->next, new);
        else if(strcmp(temp->next->city, new->city) > 0)
            InsertAfter(temp, new);
        else
        {
            printf("City already exists!");
            return 0;
        }
    }
    else
        free(temp);
    return EXIT_SUCCESS;
}

int InsertAfter(PositionList temp, PositionList new)
{
    new->next = temp->next;
    temp->next = new;
    return EXIT_SUCCESS;
}

PositionList FindBefore(PositionList find, PositionList head)
{
    PositionList temp = head;
    while(temp->next != NULL && temp->next != find)
        temp=temp->next;
    return temp;
}

PositionTree SortCountry(PositionTree current, PositionTree temp)
{
    if(current == NULL)
    {
        current = (Tree*)malloc(sizeof(Tree));
        strcpy(current->country, temp->country);
        current->head = temp->head;
        current->left = NULL;
        current->right = NULL; 
    }
    else if(strcmp(temp->country, current->country) < 0)
        current->left = SortCountry(current->left, temp);
    else if(strcmp(temp->country, current->country) > 0)
        current->right = SortCountry(current->right, temp);

    return current;
}

PositionList InitializeList()
{
    PositionList new = NULL;
    new = (List*)malloc(sizeof(List));
    new->br = 0;
    strcpy(new->city ,  "");
    new->next = NULL;

    return new;

}

int PrintTree(PositionTree current)
{
    if(current == NULL)
        return 0;
    PrintTree(current->left);
    printf("\n%s\n", current->country);
    PrintList(current->head);
    PrintTree(current->right);

    return 0;
}

int PrintList(PositionList head)
{
    PositionList temp = head->next;
    while (temp != NULL)
    {
        printf("  %s = %d\n", temp->city, temp->br );
        temp= temp->next;
    }
    return 0;
}

int BiggerBetterStronger(char* country, int number, PositionTree root)
{
    
    PositionTree chosenCountry = NULL;
    chosenCountry = FindCountry(country, root);
    if(chosenCountry == NULL)
    {
        printf("Country %s not found.\n", country);
        return -1;
    }
    PositionList listchosenCountry = chosenCountry->head;
    while(listchosenCountry->next != NULL && listchosenCountry->next->br < number)
        listchosenCountry = listchosenCountry->next;
    if(listchosenCountry->next == NULL)
    {
        printf("We did not find cities in %s that have larger population than %d.\n", country, number);
        return -2;
    }
    printf("\nCities in %s with larger population than %d:\n", country, number);
    PrintList(listchosenCountry);
    return 0;
    
}

PositionTree FindCountry(char* country, PositionTree current)
{
    PositionTree temp1 = NULL;
    PositionTree temp2 = NULL;
    if(current == NULL)
        return NULL;
    else if(strcmp(current->country, country) == 0)
        return current;
    temp1 = FindCountry(country, current->left);
    temp2 = FindCountry(country, current->right);
    if(temp1!=NULL)
        current = temp1;
    else if(temp2!=NULL)
        current = temp2;
    else 
        current = NULL;
    return current;
}

int main()
{
    PositionTree root = NULL;
    char country[MAX_LINE] = { 0 };
    int number = 0;
    root = ReadFromFile("drzave.txt", root);
    PrintTree(root);

    printf("\nChoose country: ");
    scanf("%s", country);
    printf("Insert number: ");
    scanf("%d", &number);
    BiggerBetterStronger(country, number, root);
    return 0;
}