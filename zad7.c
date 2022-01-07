#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE 128
#define MAX_LINE 1024

//potrebne su nam dvije strukture:
//1) ispisivanje i stvaranje direktorija
//2) stog za spremanje trenutnih direktorija
struct _Cvor;
typedef struct _Cvor* PositionOne;
typedef struct _Cvor
{
	char name[MAX_SIZE];
	PositionOne sibling;
	PositionOne child;
}Cvor;

struct _Stog;
typedef struct _Stog* PositionTwo;
typedef struct _Stog
{
	PositionOne directory; //PositionOne - jer tu spremamo adresu direktorija iz našeg stabla, onog u koji smo ušli(a njegova je adresa PositionOne)
	PositionTwo next;
	//a to se sve sprema u kućicu stoga koja ima svoju adresu PositionTwo, i npr. ima svoj next(iduću kućicu stoga) koja je isto tipa PositionTwo
}Stog;

PositionOne MakeDirectory(char* name, PositionOne current);
PositionOne InsertDirectory(PositionOne current, PositionOne new);
int PushToStog(PositionTwo head, PositionOne directory);
int PopFromStog(PositionTwo head);
PositionTwo FindLast(PositionTwo head);
PositionTwo FindBeforeLast(PositionTwo head);
PositionOne ChangeDirectory(PositionTwo head, PositionOne current, char* name);
int DisplayInformation(PositionOne current);
int Exit(PositionOne current);
int PrintCurrentDirectories(PositionTwo head);
int Menu(PositionOne current, PositionTwo head);

int main()
{
	Cvor root;
	Stog head;

	strcpy(root.name, "C:");
	root.child = NULL;
	root.sibling = NULL;

	head.directory = NULL;
	head.next = NULL;

	PushToStog(&head, &root);
	Menu(&root, &head);

	return 0;

}

//DOS naredba - md
//potrebni su: funkcija koja stvara direktorija i funkcija koja postavlja direktorij na odredenu poziciju u stablu

PositionOne MakeDirectory(char* name, PositionOne current)
{
	PositionOne new = NULL;
	new = (PositionOne)malloc(sizeof(Cvor));
	if (!new)
	{
		perror("Error: issue with allocation!\n");
		return NULL;
	}

	strcpy(new->name, name);

    //moguce postojanje novih direktorija
	new->child = NULL;
	new->sibling = NULL;

	current->child=InsertDirectory(current->child, new); 

	return current; //vracamo pokazivac koji pokazuje na novostvoreni direktorij
}

//4 uvjeta:
//1) ako nema ništa na pokazivaču (ako je stablo prazno), vracamo novi
//2) ako je new po abecedi ispred current, izvrsavamo ovo
//3) ako je new po abecedi iza current, izvrsavamo ovo
//4) javlja error ako je isto ime

PositionOne InsertDirectory(PositionOne current, PositionOne new)
{
	if (current == NULL)
		return new;

	else if (strcmp(current->name, new->name) > 0)
	{
		new->sibling = current;
		return new;
	}
	else if (strcmp(current->name, new->name) < 0)
	{
		current->sibling = InsertDirectory(current->sibling, new);
	}
	else
	{
		printf("Error: there is a directory with same name!\n");
		free(new);
	}
	return current;
}

int PushToStog(PositionTwo head, PositionOne directory)
{
	PositionTwo new = NULL;
	new = (PositionTwo)malloc(sizeof(Stog));
	if (!new)
	{
		perror("Error: issue with allocation!\n");
		return -1;
	}
	PositionTwo last = FindLast(head);

	new->next = last->next;
	last->next = new;
	new->directory = directory;

	return 0;
}

PositionTwo FindLast(PositionTwo head)
{
	PositionTwo temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}

PositionTwo FindBeforeLast(PositionTwo head)
{
	PositionTwo temp = head;
	while (temp->next->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}

//DOS naredba cd..
int PopFromStog(PositionTwo head) //izlazimo iz direktorija na kojem se nalazimo
{
	if (!head->next)
		return 0;

	PositionTwo before = FindBeforeLast(head);
	PositionTwo toDelete = NULL;

	toDelete = before->next;
	before->next = toDelete->next;
	free(toDelete);
	
	return 0; 
}
// DOS naredba cd ""
PositionOne ChangeDirectory(PositionTwo head, PositionOne current,char* name)
{
	if (NULL == current->child) //ako nema ništa u trenutnom direktoriju
	{
		printf("The system cannot find the path specified (current directory is empty).\n");
		return current;
	}
	else
	{
		PositionOne temp = current->child;
		while (temp != NULL && strcmp(temp->name,name)!=0)
			temp = temp->sibling;

		if (temp == NULL) //ako nikad nismo našli ime koje tražimo
		{
			printf("The system cannot find the path specified.\n");
			return current;
		}
		else
		{
			PushToStog(head, temp); //!!
			return temp;
		}
	}
}

//DOS naredba dir
int DisplayInformation(PositionOne current)
{
	if (NULL == current->child)
	{
		printf("Directory is empty.\n");
	}
	else
	{
		PositionOne temp = current->child;
		while (temp != NULL)
		{
			printf("<DIR>\t\t %s\n", temp->name);
			temp = temp->sibling;
		}
	}

	return 0;
}

int PrintCurrentDirectories(PositionTwo head)
{
	//printamo put kojim smo došli do trenutnog direktorija (tj. direktorij u kojem smo i njegove prethodnike)
	PositionTwo temp = head->next;

	while (temp!=NULL)
	{
		printf("%s\\", temp->directory->name);
		temp = temp->next;
	}
	printf(">");

	return 0;
}

int Menu(PositionOne current, PositionTwo head)
{
	PositionOne first = current;
	char name[MAX_LINE] = { 0 };
	char command[MAX_LINE] = { 0 };
	char line[MAX_LINE] = { 0 };

	while (1)
	{
		PrintCurrentDirectories(head);
		fgets(line, MAX_LINE, stdin);
		sscanf(line, "%s %s", command, name);

		if (strcmp(command, "md")==0)
		{
			current = MakeDirectory(name, current);
		}
		else if(strcmp(command, "cd")==0)
		{
			current=ChangeDirectory(head,current,name);
		}
		else if (strcmp(command, "cd..")==0)
		{
			PopFromStog(head);
			current = FindLast(head)->directory;
		}
		else if (strcmp(command, "dir")==0)
		{
			DisplayInformation(current);
		}
		else if (strcmp(command, "exit")==0)
		{
			return 0;
		}
		else
		{
			printf("'%s' command not recognized.\n", command);
		}
	}
	return 0;
}