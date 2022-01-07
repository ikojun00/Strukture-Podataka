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

int PrependList(Position head, char* name, char* surname, int birthYear); //dodati na pocetak
int AppendList(Position head, char* name, char* surname, int birthYear); //dodati na kraj
int PrintList(Position first);
Position CreatePerson(char* name, char* surname, int birthYear);
int InsertAfter(Position position, Position newPerson);
Position FindLast(Position head);
Position FindBySurname(Position first, char* surname);
Position FindBefore(Position first, char* surname);
int DeleteAfter(Position head, char* surname);

int main(int argc, char** argv) {

	Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };
	Position p = &Head;

	PrependList(&Head, "Darko", "Daric", 2003);
	AppendList(&Head, "Zarko", "Zarkic", 1977);
	PrependList(&Head, "Slavko", "Horvat", 1966);

	printf("Before deleting user Zarko Zarkic: \n");
	PrintList(Head.next);

	DeleteAfter(&Head, "Zarkic");

	printf("\nAfter deleting user Zarko Zarkic: \n");

	PrintList(Head.next);

	return EXIT_SUCCESS;
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
/*
// tu pitamo jesi li ti bas prvi elementu(first-u) NULL? 
Position FindBefore(Position first, char* surname)
{
    Position temp = first;
	Position wanted = NULL;
	wanted = FindBySurname(first, surname);
	if (!wanted) {
		printf("Greska!");
		return NULL;
	}
	
	while(temp != NULL && temp->next != wanted){
		
		temp = temp->next;
	}
	if(temp->next == wanted)
		return temp;
	return NULL;

}
*/

