#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 50

struct _Person;
typedef struct _Person* Position;
typedef struct _Person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;
}Person;


int Menu(Position head);
int PrependList(Position head, char* name, char* surname, int birthYear); //dodati na pocetak
int AppendList(Position head, char* name, char* surname, int birthYear); //dodati na kraj
int PrintList(Position first);
Position CreatePerson(char* name, char* surname, int birthYear);
int InsertAfter(Position position, Position newPerson);
Position FindLast(Position head);
Position FindBySurname(Position first, char* surname);
Position FindBefore(Position first, char* surname);
int DeleteAfter(Position head, char* surname);
int AddAfter(Position head, char *previousSurname, char* name, char* surname, int birthYear);
int AddBefore(Position head, char *previousSurname, char* name, char* surname, int birthYear);
int Sort(Position head, Position newPerson);
int WriteInDocument(Position head, char *dat);
int ReadFromDocumnent(Position head, char *dat);


int main(int argc, char** argv) {

	Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	Position p = &Head;
	/*
	Position newPerson = CreatePerson("ivor", "kojund", 2001);

	PrependList(&Head, "Darko", "Daric", 2003);
	AppendList(&Head, "Zarko", "Zarkic", 1977);
	PrependList(&Head, "Slavko", "Horvat", 1966);

	printf("Before deleting user Zarko Zarkic: \n");
	PrintList(Head.next);
	DeleteAfter(&Head, "Zarkic");
	Sort(&Head, newPerson);

	printf("\nAfter deleting user Zarko Zarkic: \n");
	PrintList(Head.next);

	//pod a)
	printf("\nAfter adding user Ante Antic before Darko Daric: \n");
	AddBefore(&Head, "Daric", "Ante", "Antic", 1980);
	PrintList(Head.next);

	//pod b)
	printf("\nAfter adding user Davor Katic after Slavko Horvat: \n");
	AddAfter(&Head, "Horvat", "Davor", "Katic", 1927);
	PrintList(Head.next);

	//Ako korisnik izabire nakon/prije kojeg prezimena cemo dodati, onda u funkciju AddAfter i AddBefore dodajemo scanf

	//pod c
	printf("\nAfter sorting: \n");
	Sort(&Head);
	PrintList(Head.next);

	//pod d
	WriteInDocument(&Head, ""); //potrebno dodati ime dokumenta

	//pod e
	//ReadFromDocumnent(&Head, "");	//potrebno dodati ime dokumenta
	*/
	while(!Menu(&Head)){
		
	}

	return EXIT_SUCCESS;
}

int Menu(Position head)
{
    int izbor = 0;
	char name[MAX_SIZE] = {0};
	char surname[MAX_SIZE] = {0};
	char odabranoPrezime[MAX_SIZE] = {0};
	char datoteka[MAX_SIZE] = {0};
	int birthYear = 0;
	Position newPerson = NULL;

    printf("Odaberi zeljenu opciju:\n\n"
			"1. Dodaj studenta na pocetak liste\n"
			"2. Dodaj studenta na kraj liste\n"
			"3. Dodaj studenta iza postojeceg studenta s odredenim prezimenom\n"
			"4. Dodaj studenta prije postojeceg studenta s odredenim prezimenom\n"
			"5. Izbrisi studenta(po prezimenu)\n"
			"6. Ispisi listu studenta\n"
			"7. Ucitaj studente iz datoteke\n"
			"8. Upisi listu studenata u datoteke\n"
			"9. Sortirani unos studenata\n"
			"0. Izadi iz menu-a\n"
			);

    scanf(" %d", &izbor);

    switch(izbor)
    {
        case (1):
			printf("Unesite podatke studenta: ime prezime godina rodenja: ");
			scanf(" %s %s %d", name, surname, &birthYear);
            PrependList(head, name, surname, birthYear);
            break;
        case (2):
			printf("Unesite podatke studenta: ime prezime godina rodenja: ");
			scanf(" %s %s %d", name, surname, &birthYear);
            AppendList(head, name, surname, birthYear);
            break;
        case (3):
            printf("Unesite podatke studenta - ime prezime godina rodenja: ");
			scanf(" %s %s %d", name, surname, &birthYear);
            printf("Unesite prezime studenta nakon kojeg zelite obaviti unos:");
			scanf(" %s", odabranoPrezime);
            AddAfter(head, odabranoPrezime, name, surname, birthYear);
            break;
		case (4):
            printf("Unesite podatke studenta - ime prezime godina rodenja: ");
			scanf(" %s %s %d", name, surname, &birthYear);
            printf("Unesite prezime studenta ispred kojeg zelite obaviti unos:");
			scanf(" %s", odabranoPrezime);
            AddBefore(head, odabranoPrezime, name, surname, birthYear);
            break;
		case (5):
			printf("Unesite prezime postojeceg studenta u listi kojeg zelite obrisati");
			scanf(" %s", odabranoPrezime);
			DeleteAfter(head, odabranoPrezime);
			break;
        case (6):
            PrintList(head->next);
			break;
        case (7):
            printf("Unesite ime datoteke:");
            scanf(" %s", datoteka);
            ReadFromDocumnent(head, datoteka);
            break;
        case (8):
            printf("Unesite ime datoteke:");
            scanf(" %s", datoteka);
            WriteInDocument(head, datoteka);
            break;
		case (9):
			printf("Unesite podatke studenta - ime prezime godina rodenja: ");
			scanf(" %s %s %d", name, surname, &birthYear);
			newPerson = CreatePerson(name, surname, birthYear);
			Sort(head, newPerson);
			PrintList(head->next);
            break;

        case (0):
            return 1;
    }
    return 0;
}

int PrependList(Position head, char* name, char* surname, int birthYear) {
	
	Position newPerson = NULL;

	newPerson = CreatePerson(name, surname, birthYear);
	if (!newPerson) {
		return -1;
	}

	InsertAfter(head, newPerson);

	return EXIT_SUCCESS;
}

int AppendList(Position head, char* name, char* surname, int birthYear)
{
	Position newPerson = NULL;
	Position last = NULL;

	newPerson = CreatePerson(name, surname, birthYear);
	if (!newPerson) {
		return -1;
	}

	last = FindLast(head);
	InsertAfter(last, newPerson);

	return EXIT_SUCCESS;
}

int PrintList(Position first)
{
	Position temp = first;
	
	while (temp!=NULL) {
		printf("Name: %s, surname: %s, birthyear: %d\n", temp->name, temp->surname, temp->birthYear);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

Position CreatePerson(char* name, char* surname, int birthYear)
{
	Position newPerson = NULL;

	newPerson = (Position)malloc(sizeof(Person));
	if (!newPerson) {
		perror("Can't allocate memory!\n");
		return NULL;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;
	newPerson->next = NULL;
	
	return newPerson;
}

int InsertAfter(Position position, Position newPerson)
{
	newPerson->next = position->next;
	position->next = newPerson;

	return EXIT_SUCCESS;
}

Position FindLast(Position head)
{
	Position temp = head;

	while (temp->next) {
        temp = temp->next;
	}

	return temp;
}

Position FindBySurname(Position first, char* surname)
{
	Position temp = first;
	while (temp) {
		if (strcmp(temp->surname, surname) == 0) {
			return temp;
		}
		temp=temp->next;
	}
	return NULL;
}
// tu pitamo head-a, je li njegov next NULL 
Position FindBefore(Position head, char* surname)
{
    Position temp = head;
	Position wanted = NULL;
	wanted = FindBySurname(head->next, surname);
	if (!wanted) {
		printf("Greska!");
		return NULL;
	}
	
	while(temp->next != NULL && temp->next != wanted){
		
		temp = temp->next;
	}
	if(temp->next == wanted)
		return temp;
	return NULL;

}

int DeleteAfter(Position head, char* surname)
{
	Position before = NULL;
	Position wantToDelete = NULL;
	before = FindBefore(head, surname);

	wantToDelete= before->next;
	before->next = wantToDelete->next;
	free(wantToDelete);

	return 0;
}

int AddAfter(Position head, char* previousSurname, char* name, char* surname, int birthYear)
{
	Position previous = NULL;
	Position newPerson = NULL;

	newPerson = CreatePerson(name, surname, birthYear);
	if (!newPerson) {
		return -1;
	}

	previous = FindBySurname(head->next, previousSurname); 

	InsertAfter(previous, newPerson);
	
	return EXIT_SUCCESS;
}

int AddBefore(Position head, char* previousSurname, char* name, char* surname, int birthYear)
{
	Position next = NULL;
	Position newPerson = NULL;

	newPerson = CreatePerson(name, surname, birthYear);
	if (!newPerson) {
		return -1;
	}

	next = FindBefore(head->next, previousSurname);

	InsertAfter(next, newPerson);
	
	return EXIT_SUCCESS;
	
}

int Sort(Position head, Position newPerson)
{
	Position p = head;
	while(p->next != NULL && strcmp(p->next->surname, newPerson->surname)<0){
		p = p->next;
	}

	InsertAfter(p, newPerson);

	return 0;
}

int WriteInDocument(Position head, char *dat)
{
    FILE* datoteka = NULL;
    Position p = head;

    datoteka = fopen(dat, "w");
    
	if(!datoteka)
    {
        perror("Can't open a document!\n");
        return -1;
    }

    while(p->next != NULL)
    {
        fprintf(datoteka, "Name: %s, surname: %s, birthyear: %d\n", p->next->name, p->next->surname, p->next->birthYear);
		p = p->next;
    }

    fclose(datoteka);
    return 0;
}


int ReadFromDocumnent(Position head, char *dat)
{
    FILE* datoteka = NULL;
	char buffer[MAX_SIZE] = { 0 };
    char *name, *surname;
    int birthYear;
    Position newPerson = NULL;

    datoteka = fopen(dat, "r");
    
	if(!datoteka)
    {
        perror("Can't open a document!\n");
        return -1;
    }

    while(!feof(datoteka))
    {  
        newPerson =(Position)malloc(sizeof(Person));
        fgets(buffer, MAX_SIZE, datoteka);
        if(sscanf(buffer, " %s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear) == 3){
            Sort(head, newPerson);
        }
    }

    fclose(datoteka);
    return 0;
}