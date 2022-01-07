#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE (128)
#define MAX_LINE (1024)

typedef struct
{
    char ime[MAX_SIZE];
    char prezime[MAX_SIZE];
    double bodovi;
}student;

int BrojRedaka(char *dokument1); //naziv dokumenta s imenima, prezimenima i brojem bodova
student* Alokacija(int brojac, char *dokument1);

int main()
{
    int i=0;
    double max=0;

    student* _student = NULL;
    _student = Alokacija(BrojRedaka("dokument1.txt"), "dokument1.txt");

    max = _student[0].bodovi;

    for(i=0; i<BrojRedaka("dokument1.txt"); i++)
    {
        if(max<_student[i].bodovi)
                max = _student[i].bodovi;
    }

    for(i=0; i<BrojRedaka("dokument1.txt"); i++)
        printf("Ime: %s\nPrezime: %s\nBroj bodova: %lf\nRelativan broj bodova: %lf\n", _student[i].ime, _student[i].prezime, _student[i].bodovi, _student[i].bodovi/max*100);
    return 0;
}

int BrojRedaka(char *dokument1)
{
    int brojac=0;
    FILE* datoteka = NULL;
    char buffer[MAX_LINE] = { 0 };

    datoteka = fopen(dokument1, "r");
    if(!datoteka)
    {
        printf("Greska!\n");
        return -1;
    }

    while(!feof(datoteka))
    {
        fgets(buffer, MAX_LINE, datoteka);
        brojac++;
    }

    fclose(datoteka);
    return brojac;
}

student* Alokacija(int brojac, char *dokument1)
{
    int br=0;
    FILE* datoteka = NULL;
    student* studenti = NULL;

    datoteka = fopen(dokument1, "r");
    studenti=(student*)malloc(brojac*sizeof(student));
    if(!datoteka)
    {
        printf("Greska s alokacijom!");
        free(studenti);
        return NULL;
    }

    while(!feof(datoteka))
    {
        fscanf(datoteka, " %s %s %lf", studenti[br].ime, studenti[br].prezime, &studenti[br].bodovi);
        br++;
    }

    fclose(datoteka);
    return studenti;
}