#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
short prima_data=1;

struct fisier
{
    char nume[50];
    char caracter[10001];
    int numar[10001];
    int nn;
    long long nrB;
};

int dimensiune(FILE *p)
{
    long dim;
    fseek(p,0L,SEEK_END);
    dim=ftell(p);
    fseek(p,0L,SEEK_SET);
    return dim;
}

void arhiveaza(char numefisier[], char numearhiva[])
{

    int a_exista=1;
    FILE *f, *fo;
    if ((f = fopen(numefisier, "rb")) == NULL)
    {
        printf("%s nu a fost gasit!\n", numefisier);
        return;
    }

    numearhiva=strcat(numearhiva, ".asd");

    if((fo=fopen(numearhiva, "rb"))==NULL)
    {
        printf("Arhiva '%s' nu exista. Se creeaza una...\n", numearhiva);
        a_exista=0;
    }

    fo=fopen(numearhiva, "ab");

    /*Duc pointerul la capatul fisierului in caz ca arhiva exista deja*/
    if(a_exista==1)
    {
        fseek(fo, 0L, SEEK_END);
        printf("%ld\n", ftell(fo));
    }

    /*Aflu dimensiunea fisierului*/
    fseek(f, 0L, SEEK_END);
    unsigned long fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    char baitAnt=fgetc(f);
    char baitCitit;
    long pointeru=ftell(f);

    fputs("onm",fo);
    fputs(numefisier,fo);
    fputc(']',fo);

    while(pointeru<fsize)
    {
        int repetitii=1;
        while(1)
        {
            baitCitit=fgetc(f);
            if(baitCitit!=baitAnt)
                break;
            repetitii++;
        }

        fputc(baitAnt, fo);
        baitAnt=baitCitit;
        fprintf(fo,"%d",repetitii);

        pointeru=ftell(f);
        printf("\r%.1f%%", ((float)pointeru/(float)fsize)*100.0);

    }

    fputs("mno",fo);

    printf("\rFisierul '%s' a fost adaugat cu succes in arhiva! '%s'\n", numefisier, numearhiva);

    fclose(fo);
    fclose(f);

}

void dezarhiveaza(char numearhiva[], char numefisier[])
{
    FILE *f, *fo;
    numearhiva=strcat(numearhiva, ".asd");

    if ((f = fopen(numearhiva, "rb")) == NULL)
    {
        printf("Arhiva '%s' nu a fost gasita!\n", numearhiva);
        return;
    }

    fo=fopen(numefisier, "wb");

    unsigned long fsize;

    char mno_check[3]= {0};
    mno_check[3]='\0';

    char mno_verif[3]="mno";
    unsigned long i=0;
    while(1)
    {

        /*Daca am gasit un nume de fisier...*/
        if(fgetc(f)=='o')
        {
            if (fgetc(f)=='n')
            {
                if (fgetc(f)=='m')
                {
                    unsigned long j=0;
                    char temp[strlen(numefisier)];
                    while(j<strlen(numefisier))
                    {
                        temp[j]=fgetc(f);
                        j++;
                    }

                    /*... si numele acestuia este numele din input*/
                    if(strcmp(temp, numefisier)==0)
                    {
                        fgetc(f);
                        break;
                    }
                }
                else
                    fseek(f, -2, SEEK_CUR);
            }
            else
                fseek(f,-1,SEEK_CUR);
        }
    }


    char bait_cur;
    unsigned int repetitii;


    while(ftell(f)<fsize)
    {

        bait_cur=fgetc(f);
        repetitii=fgetc(f);
        repetitii-=48;

        if(repetitii>0 && repetitii<10)
        {
            for(int i=0; i<repetitii; i++)
                fputc(bait_cur, fo);
        }

        else
        {
            fputc(bait_cur,fo);
            fseek(f,-1,SEEK_CUR);
        }

        if(fgetc(f)=='m')
        {
            if(fgetc(f)=='n')
            {
                if(fgetc(f)=='o')
                    break;

                else
                    fseek(f,-3,SEEK_CUR);
            }
            else
                fseek(f,-2,SEEK_CUR);
        }
        else
            fseek(f, -1,SEEK_CUR);
    }

    printf("Arhiva '%s' a fost extrasa cu succes in '%s'!\n", numearhiva, numefisier);
    fclose(f);
    fclose(fo);
}

void culoare_text(char culoare[])
{
    if(strcmp("rosu", culoare)==0)
        printf("\033[0;31m");
    else if(strcmp("verde",culoare)==0)
        printf("\033[0;32m");
    else if(strcmp("galben",culoare)==0)
        printf("\033[0;33m");
    else if(strcmp("albastru",culoare)==0)
        printf("\033[0;34m");
    else if(strcmp("magenta", culoare)==0)
        printf("\033[0;35m");
    else if(strcmp("reset", culoare)==0)
        printf("\033[0m");
}


void ajutor()
{
    printf("arhiveaza fisier arhiva %-20c Adauga un fisier intr-o arhiva\n", ' ');
    printf("dezarhiveaza arhiva fisier %-20c Extrage un fisier dintr-o arhiva\n", ' ');
    printf("sterge cu buretele %-20c Sterge cu buretele\n", ' ');
}

void afiseaza(char arhiva[], int buffer)
{
    FILE *f;
    if((f=fopen(arhiva,"rb"))==NULL)
    {
        printf("Arhiva '%s' nu exista!\n", arhiva);
        return;
    }

    unsigned long dim=dimensiune(f);

    while(ftell(f)<dim)
    {
        /*Daca am gasit un nume de fisier...*/
        if(fgetc(f)=='o')
        {
            if (fgetc(f)=='n')
            {
                if (fgetc(f)=='m')
                {
                    while(1)
                    {
                        if(fgetc(f)==']')
                        {
                            printf("\n");
                            break;
                        }
                        else
                            fseek(f,-1,SEEK_CUR);

                        printf("%c", fgetc(f));
                    }
                }
                else
                    fseek(f, -2, SEEK_CUR);
            }
            else fseek(f,-1,SEEK_CUR);
        }
    }

    fclose(f);
    return;

}


int main()
{
    char comanda[100], arg1[100], arg2[100];
    //textcolor(RED);
    do
    {
        FILE* fin, *fo;

        if(prima_data)
        {
            printf("ASDE v0.00001\nBy Sava Razvan and Descas Rares\nScrieti ajutor pentru a afisa lista de comenzi.\n");
            prima_data=0;
        }

        scanf("%s", comanda);

        if(strcmp(comanda, "ajutor")==0)
            ajutor();

        else if(strcmp(comanda, "iesire")==0)
        {
            printf("Va multumim ca ati folosit ASDE!!!\n\n");
            return 0;
        }


        else
        {
            scanf("%s %s", arg1,arg2);

            if(strcmp(comanda, "arhiveaza")==0)
            {
                printf("Se arhiveaza....\n");
                arhiveaza(arg1, arg2);
            }

            else if(strcmp(comanda, "dezarhiveaza")==0)
            {
                printf("Se extrage....\n");
                dezarhiveaza(arg1, arg2);
            }

            else if(strcmp(comanda, "culoare")==0)
            {
                culoare_text(arg2);
            }

            else if(strcmp(comanda, "afiseaza")==0)
            {
                afiseaza(arg1, atoi(arg2));
            }

            else if(strcmp(comanda, "sterge")==0
                    &&strcmp(arg1, "cu")==0
                    &&strcmp(arg2, "buretele")==0)
                system("cls");


            else
                printf("Comanda invalida!\n");
        }

    }
    while(1);

}






