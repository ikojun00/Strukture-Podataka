#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024

struct _poly;
typedef struct _poly* Position;
typedef struct _poly{
    int coef;
    int expo;
    Position next;
}Poly;

int ReadFromFile(char *file, Position headFirst, Position headSecond); 
int AddingPolyToList(Position head, char *buffer); 
int SortedByExpo(Position head, Position newPoly); 
int Add(Position headFirst, Position headSecond, Position headAdd);
int Multiply(Position headFirst, Position headSecond, Position headMultiply); 
int PrintList(Position head); 
Position CreateElement(int coef, int expo); 
int InsertAfter(Position pos, Position newPoly); 
int DeleteAfter(Position previous); 
int Free(Position head); 

int main()
{
    Poly HeadFirst = { .coef = 0, .expo = 0, .next = NULL};
    Poly HeadSecond = { .coef = 0, .expo = 0, .next = NULL};
    Poly HeadAdd = { .coef = 0, .expo = 0, .next = NULL};
    Poly HeadMultipy = { .coef = 0, .expo = 0, .next = NULL};

    char fileName[MAX_LINE] = { 0 };
    printf("File name: ");
    scanf("%s", fileName);

    if(ReadFromFile(fileName, &HeadFirst, &HeadSecond) == EXIT_SUCCESS)
    {
        printf("First Polynom:\n ");
        PrintList(&HeadFirst);
        printf("Second Polynom:\n ");
        PrintList(&HeadSecond);

        printf("Addition of Polynoms:\n ");
        Add(&HeadFirst, &HeadSecond, &HeadAdd);
        PrintList(&HeadAdd);

        printf("Multiplication of Polynoms:\n ");
        Multiply(&HeadFirst, &HeadSecond, &HeadMultipy);
        PrintList(&HeadMultipy);

        //oslobadanje memorije
        Free(&HeadFirst);
        Free(&HeadSecond);
        Free(&HeadAdd);
        Free(&HeadMultipy);
    }
    return 0;
    
}

int ReadFromFile(char *file, Position headFirst, Position headSecond)
{
    FILE *dat = NULL;
    char buffer[MAX_LINE] = { 0 };
    int status = 0; 

    dat = fopen(file, "r");
    if(!dat)
    {
        perror("Greska pri otvaranja datotke\n");
		return -1;
    }

    //ucitavamo prvi redak datoteke i spremamo ga u listu
    fgets(buffer, MAX_LINE, dat); 
    
    status = AddingPolyToList(headFirst, buffer); 
    if (status != EXIT_SUCCESS){
		return EXIT_FAILURE;
    }

    //drugi redak ucitavamo i spremmao u drugu listu
    fgets(buffer, MAX_LINE, dat); 
    status = AddingPolyToList(headSecond, buffer); 
    if (status != EXIT_SUCCESS){
		return EXIT_FAILURE;
    }

    fclose(dat);
    return EXIT_SUCCESS;
}
int AddingPolyToList(Position head, char *buffer)
{
    char* currentBuffer = buffer;
    int co = 0;
    int ex = 0;
    int n = 0;
    int status = 0;
    Position newElement = NULL;

    while(strlen(currentBuffer) > 1) 
    {
        status = sscanf(currentBuffer, " %d %d %n", &co, &ex, &n);
        
        if(status != 2)
        {
            
            printf("Datoteka nije u valjanom obliku.");
            return EXIT_FAILURE;
        }
        
        newElement = CreateElement(co, ex);
        if (!newElement)
		{
			return EXIT_FAILURE;
		}
        SortedByExpo(head, newElement);
        
        currentBuffer +=n; //pomicemo pocetak currentBuffera za onoliko mjesta koliko smo procitali dok smo ucitavali brojeve(parove)

    }
    return EXIT_SUCCESS;
}

int SortedByExpo(Position head, Position newPoly)
{
    Position temp = head;
    while(temp->next!=NULL && temp->next->expo < newPoly->expo)
    {
        temp=temp->next;
    }// petlja stane na kraju ili ako nademo neki eksponent veci od novog eksponenta
   
    if(temp->next == NULL || temp->next->expo != newPoly->expo)
    {
       InsertAfter(temp, newPoly);
    }//ako smo dosli do kraja/ naisli na eksponent veci i razlicit od novog, ubacujemo iza elementa temp na kojem je while petlja zaustavljena
    else  //ako smo nasli medu postojecim eksponentima ovaj novi
    {
        if(temp->next->coef + newPoly->coef == 0) //ako je zbroj njihovih koefiijenata nula onda nemamo zasto drzat tu strukturu u listu pa je brisemo
            DeleteAfter(temp);
        else
            temp->next->coef = temp->next->coef + newPoly->coef; //ako nije nula onda pribajamo novi koeficijent onom koji je vec tu
    }

    return EXIT_SUCCESS;
}


int Add(Position headFirst, Position headSecond, Position headAdd)
{
	Position i = NULL;
	Position j = NULL;
    Position newElement = NULL;

	for (i = headFirst->next; i != NULL; i = i->next)
	{
		newElement = CreateElement(i->coef, i->expo);
		if (!newElement)
		{
			return EXIT_FAILURE;
		}
		SortedByExpo(headAdd, newElement);
        //sve clanove prve liste sortirano dodamo u listu zbrojenih
	}
	for (j = headSecond->next; j != NULL; j = j->next)
	{
		newElement = CreateElement(j->coef, j->expo);
		if (!newElement)
		{
			return EXIT_FAILURE;
		}
		SortedByExpo(headAdd, newElement);
        //sve clanove druge liste sortirano dodamo u listu zbrojenih
	}
	return EXIT_SUCCESS;
    //ovime smo obje pocetne liste dodali u trecu listu, sortirano, i tako izvrsili zbrajanje(jer se zbrajanje zapravo dogada u sortu)
}


int Multiply(Position headFirst, Position headSecond, Position headMultiply)
{
	Position i = NULL;
	Position j = NULL;
	Position newElement = NULL;

	for (i = headFirst->next; i != NULL; i = i->next)
	{
		for (j = headSecond->next; j != NULL; j = j->next)
		{
            //svaki clan prve liste mnozimo sa svakim elementom druge liste, svaki taj umnozak je newElement struktura koju sortirano dodamo u listu koja pocinje s headMulitply
			newElement = CreateElement(i->coef * j->coef, i->expo + j->expo);
			if (!newElement)
			{
				return -1;
			}
            SortedByExpo(headMultiply, newElement);
		}
	}
	return 0;
}

int PrintList(Position head)
{
    Position temp= head;
    while(temp->next!=NULL)
    {
        if(temp->next->next== NULL) //ako je kraj necemo printat plus
        printf("%d*x^(%d)\n", temp->next->coef, temp->next->expo); 
        else
        printf("%d*x^(%d) + ", temp->next->coef, temp->next->expo);

        temp=temp->next;
    }
    return EXIT_SUCCESS;
}

Position CreateElement(int coef, int expo)
{
    Position newPoly = (Position)malloc(sizeof(Poly));

    if(!newPoly){
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newPoly->expo=expo;
    newPoly->coef=coef;
    newPoly->next=NULL;

    return newPoly;
}

int InsertAfter(Position pos, Position newPoly)
{
    newPoly->next=pos->next;
    pos->next=newPoly;
    return EXIT_SUCCESS;
}

int DeleteAfter(Position previous)
{
	Position toDelete=previous->next;
	previous->next = toDelete->next;
	free(toDelete);

	return EXIT_SUCCESS;
}
int Free(Position head)
{
    //na kraju programa
    //brisemo sve elemente tako zapravo brisemo uvijek onaj iza heada, dok iza heada ne ostane samo null
	Position temp = head;
	while (temp->next != NULL)
	{
		DeleteAfter(temp);
	}
	return EXIT_SUCCESS;
}