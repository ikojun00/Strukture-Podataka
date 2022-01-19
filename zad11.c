#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define HASH_SIZE (11)
#define MAX_LINE (1024)

struct _Tree;
typedef struct _Tree* PositionTree;
typedef struct _Tree{
    int br;
    char city[MAX_LINE];
    PositionTree left;
    PositionTree right;
}Tree;


struct _List;
typedef struct _List* PositionList;
typedef struct _List
{
    char country[MAX_LINE];
    PositionTree root;
    PositionList next;
}List;




int ReadFromFile(char *filename, PositionList head, List HashTabl[]);
PositionTree ReadCitiesFromFile(char* filename);
int SortList(PositionList new, PositionList head);
int InsertAfter(PositionList temp, PositionList new);
PositionTree SortTree(char *name, int brojSt, PositionTree current);
int PrintList(PositionList head);
int PrintTreeInOrder(PositionTree current);
int BiggerBetterStronger(char* country, int number, List HashTabl[]);
int FindMax(PositionTree current);
PositionList FindCountry(char* country, PositionList head);
int PrintCities(int number, PositionTree current);
int HashTab(List HashTabl[],PositionList new);
int HashNumber(char* country);


int ReadFromFile(char *filename, PositionList head, List HashTabl[])
{
    char buffer[MAX_LINE] = { 0 };
    char fileCity[MAX_LINE] = { 0 };
    int number;
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionList temp = NULL;

    if(!pDat)
    {
        perror("Cannot open the file!\n");
		return -1;
    }

    while(!feof(pDat))
    {  
        
        PositionList temp = NULL;
        temp = (List*)malloc(sizeof(List));
        temp->next = NULL;

        fgets(buffer, MAX_LINE, pDat);

        if(sscanf(buffer, " %s %s",  temp->country, fileCity) == 2)
        {
            temp->root = NULL;
            temp->root = ReadCitiesFromFile(fileCity);
            
            HashTab(HashTabl, temp);
        }
    }

    fclose(pDat);

    return 0;
}

PositionTree ReadCitiesFromFile(char* filename)
{
    char buffer[MAX_LINE] = { 0 };
    char name[MAX_LINE] = { 0 };
    int brojSt = 0;
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionTree root = NULL;

    if(!pDat)
    {
        perror("Cannot open the file!\n");
		return NULL;
    }

    while(!feof(pDat))
    {  
        fgets(buffer, MAX_LINE, pDat);
        if(sscanf(buffer, " %s %d", name, &brojSt) == 2)
        {
            root = SortTree(name, brojSt, root);
        }
    }
    fclose(pDat);
    return root;
    
}

int SortList(PositionList new, PositionList head)
{
    PositionList temp = head;
    
    while (temp->next != NULL && strcmp(temp->next->country, new->country) < 0)
    {
        temp=temp->next;
    }
    if(temp->next == NULL || strcmp(temp->next->country, new->country) != 0)
    {   
        InsertAfter(temp, new); 
    }
    else if(strcmp(temp->next->country, new->country) == 0)
    {
        return -1;
    }
    return EXIT_SUCCESS;
}

int InsertAfter(PositionList temp, PositionList new)
{
    new->next = temp->next;
    temp->next = new;
}

PositionTree SortTree(char *name, int brojSt, PositionTree current)
{
    if(current == NULL)
    {
        current = (Tree*)malloc(sizeof(Tree));
        strcpy(current->city, name);
        current->br = brojSt;
        current->left = NULL;
        current->right = NULL; 
    }
    else if(brojSt < current->br)
        current->left = SortTree(name, brojSt, current->left);
    else if(brojSt > current->br)
        current->right = SortTree(name, brojSt, current->right);
    else if(brojSt == current->br)
    {
        if(strcmp(name, current->city) < 0)
            current->left = SortTree(name, brojSt, current->left);
        else if(strcmp(name, current->city) > 0)
            current->right = SortTree(name, brojSt, current->right);
        else
        {
            return current;
        }   
    }
    else
        free(current);

    return current;
}

int PrintList(PositionList head)
{
    PositionList temp = head->next;
    while(temp!=NULL)
    {
        
        printf("\n%s\n", temp->country);
        PrintTreeInOrder(temp->root);
        temp = temp->next;
    }
    return 0;

}

int PrintTreeInOrder(PositionTree current)
{
    if(current == NULL)
        return 0;
    
    PrintTreeInOrder(current->left);
    printf("  %s = %d\n", current->city, current->br);
    PrintTreeInOrder(current->right);

    return 0;
}

int BiggerBetterStronger(char* country, int number, List HashTabl[])
{
    int num = HashNumber(country);
    PositionList chosenCountry = FindCountry(country, &HashTabl[num]);
    int max = 0;
    if(chosenCountry == NULL)
    {
        printf("Country %s not found.\n", country);
        return -1;
    }
    max = FindMax(chosenCountry->root);
    if (number > max)
    {
        printf("We did not find cities in %s that have larger population than %d.\n", country, number);
        return -2;
    }
    
    printf("\nCities in %s with larger population than %d:\n", chosenCountry->country, number);
    PrintCities(number, chosenCountry->root);

    return 0;
}

int FindMax(PositionTree current)
{
    int maxi = 0;
    if(current == NULL)
        return 0;
    if(current->right == NULL)
        return current->br;
    maxi = FindMax(current->right);

    return maxi;
}

PositionList FindCountry(char* country, PositionList head)
{
    PositionList temp = head;
    while(temp->next!=NULL && strcmp(temp->next->country,country) != 0)
        temp=temp->next;
    return temp->next;
}

int PrintCities(int number, PositionTree current)
{
    if(current == NULL)
        return 0;
    PrintCities(number, current->left);
    if(current->br >= number)
        printf("  %s = %d\n", current->city, current->br);
    PrintCities(number, current->right);

    return 0;
}

int HashTab(List HashTabl[],PositionList new)
{
	int num = HashNumber(new->country);

    SortList(new, &HashTabl[num]);

	return 0;
}

int HashNumber(char* country)
{
	int num = 0;
	num = country[0] + country[1] + country[2] + country[3] + country[4];

	return num % 11;
}

PositionList InitializeList()
{
    PositionList new = NULL;
    new = (List*)malloc(sizeof(List));
    strcpy(new->country ,  "");
    new->next = NULL;
    new->root = NULL;

    return new;

} 

int main()
{
    List head = { .country = 0 ,  .root = NULL, .next = NULL };
    char country[MAX_LINE] = { 0 };
    int number = 0;

    List HashTabl[HASH_SIZE];

    for (int i = 0; i < HASH_SIZE; i ++)
	{
        HashTabl[i] = *InitializeList();
	}

    ReadFromFile("drzave.txt", &head, HashTabl);
    
    for (int i = 0; i < HASH_SIZE; i++)
	{
		if (HashTabl[i].next != NULL)
		{
			PrintList(&HashTabl[i]);
		}
	}

    printf("\nChoose country: ");
    scanf("%s", country);
    printf("Insert number: ");
    scanf("%d", &number);
    BiggerBetterStronger(country, number, HashTabl);
    return 0;
}


