#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE (1024)

struct _StackElement;
typedef struct _StackElement* Position;
typedef struct _StackElement {
    double number;
    Position next;
}StackElement;

Position CreateElement(double number);
int InsertAfter(Position position, Position newElement);
int Push(Position head, double number);
int DeleteAfter(Position position);
int Pop(double* resultDestination, Position head);
int PerformOperation(Position head, char operator);
int CalculatePostfixFromFile(double* resultDestination, char* fileName);
int DeleteAll(Position head);


int main()
{
    double result;
    char filename[MAX_LINE] = {0};
    printf("Enter file name: ");
    scanf(" %s", filename);
    
    CalculatePostfixFromFile(&result, filename);
    printf("Result = %f\n", result);

	return EXIT_SUCCESS;
}

//funkcija stvara element
Position CreateElement(double number){
    Position newElement = NULL;

    newElement = (Position)malloc(sizeof(StackElement));
    if(!newElement){
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newElement->number=number;
    newElement->next=NULL;

    return newElement;
}

//funkcija postavlja stvoreni element
int InsertAfter(Position position, Position newElement){
    newElement->next=position->next;
    position->next = newElement;

    return EXIT_SUCCESS;
}

//funkcija dodaje element na stog
int Push(Position head, double number){
    Position newElement = NULL;
    newElement = CreateElement(number);
    if(!newElement){
        return EXIT_FAILURE;
    }

    InsertAfter(head, newElement);
    return EXIT_SUCCESS;
}

//potreban kako bi funkcija Pop radila ispravno
//kada dobijemo broj iz liste, brisemo listu
int DeleteAfter(Position position){
    Position temp= position->next;
    if(!temp){
        return EXIT_SUCCESS;
    }
    position->next =temp->next;
    free(temp);
    return EXIT_SUCCESS;
}

//brisanje elemenata iz liste
//double* resultDestination - adresu na koju spremamo element kojeg brisemo sa stoga
int Pop(double* resultDestination, Position head){
    Position first = head->next;

    if(!first){
        printf("Postfix not valid! Please check your file ");
        return -1;

    }
    *resultDestination = first->number; // zvjezdica - dereferenciranje
    DeleteAfter(head);

    return EXIT_SUCCESS;
}

int PerformOperation(Position head, char operator){
    double operand1 = 0;
    double operand2 = 0;
    double result = 0;
    int status1 = EXIT_SUCCESS;
    int status2 = EXIT_SUCCESS;

    status2 = Pop(&operand2, head); //operand2 - sprema zadnji element sa stoga (last in, first out)
    if(status2 != EXIT_SUCCESS){
        return -1;
    }

    status1 = Pop(&operand1, head); //operand1 - sprema predzadnji element sa stoga (tocnije bi bilo zadnji zbog 98. linije koda)
    if(status1 != EXIT_SUCCESS){
        return -2;
    }

    switch (operator)
    {
    case '+':
        result = operand1 + operand2;
        break;
    case '-':
        result = operand1 - operand2;
        break;
    case '*':
        result = operand1 * operand2;
        break;
    case '/':
        if(operand2 == 0){
            perror("Can't divide with zero!\n");
            return -3;
        }
        result = operand1 / operand2;
        break;
    
    //default - ako jedan od zadanih operatora ne pripada skupu operatora (+, *, - , /) ispisujemo obavijest
    default:
        printf("This operation is not supported yet! Please pay and we will implement it.\n");
        return -4;
    }
    return Push(head, result); //funkcija Push - vraca rezultat
}

//funkcija ucitava postfix iz datoteke
int CalculatePostfixFromFile(double* resultDestination, char* fileName){

   FILE* file = NULL;
   int fileLength = 0;
   char* buffer = NULL;
   char* currentbuffer = NULL;
   int numBytes = 0;
   double number = 0;
   int status = 0;
   char operation = 0;
   StackElement head = {.next = NULL, .number =0}; //stog

//rb - read binary
    file = fopen(fileName, "rb");
    if(!file){
        perror("Can't open file!\n");
        return -1;
    }

    fseek(file, 0, SEEK_END); //fseek - trazi kraj file
    fileLength = ftell(file); //ftell - udaljenost od pocetka file

    buffer = (char*)calloc(fileLength +1, sizeof(char)); // alocira i stavlja sve bajtove na nulu
    if(!buffer)
    {
        perror("Can't allocate memory");
        return -2;
    }

    rewind(file);
    fread(buffer, sizeof(char), fileLength, file); //fread - slicno fgets-u, moze prihvatiti cijeli dokument za razliku od fgets-a
    printf("|%s|\n", buffer);
    fclose(file);

    currentbuffer = buffer;
    /*
        dok postoji nesto u currentbuffer-u
        skeniraj mi broj i broj bitova
        dva su uvjeta:
        ako je status 1 (skeniran je broj)
            onda mi pushaj taj broj
            javi gresku i obrisi sve ako funkcija nije vratila EXIT_SUCCESS
            povecaj broj bitova za trenutni buffer
        ako status nije 1 (nije skeniran broj)
            onda skeniraj operator
            izracunaj mi
            javi gresku i obrisi sve ako funkcija nije vratila EXIT_SUCCESS
            povecaj broj bitova za trenutni buffer
    */
    while(strlen(currentbuffer)>0)
    {
        status = sscanf(currentbuffer, " %lf %n", &number, &numBytes);

        if(status == 1)
        {
            status = Push(&head, number);
            if(status != EXIT_SUCCESS){
                free(buffer);
                DeleteAll(&head);
                return -3;
            }
            currentbuffer +=numBytes;
        }
        else
        {

            sscanf(currentbuffer, " %c %n", &operation, &numBytes);
            status = PerformOperation(&head, operation);
            if(status != EXIT_SUCCESS){

                free(buffer);
                DeleteAll(&head);
                return -4;
            }
            currentbuffer +=numBytes;
        }

    }
    free(buffer);

    status = Pop(resultDestination, &head); //odbacujemo rezultat

    if(status != EXIT_SUCCESS)
    {
        DeleteAll(&head);
        return -5;
    }

    if(head.next)
    {
        printf("Postfix not valid! Please check your file.");
        DeleteAll(&head);
        return -6;
    }

    return EXIT_SUCCESS;
}

//funkcija brise sve elemente sa stoga
int DeleteAll(Position head){
    while(head->next){
        DeleteAfter(head);
    }
}
