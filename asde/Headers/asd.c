#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "asd.h"

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

    unsigned long i=0;

    while(1)
    {
        /*Daca am gasit un nume de fisier...*/
		for (int i = 0; i < 3; i++)
			mno_check[i] = fgetc(f);

		if (strcmp(mno_check, "onm") == 0)
		{
			unsigned long j = 0;
			char temp[strlen(numefisier)];
			while (j < strlen(numefisier))
			{
				temp[j] = fgetc(f);
				j++;
			}

			/*... si numele acestuia este numele din input*/
			if (strcmp(temp, numefisier) == 0)
			{
				fgetc(f);
				break;
			}
		}

		else fseek(f, -2, SEEK_CUR);
        
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

		for (int i = 0; i < 3; i++)
			mno_check[i] = fgetc(f);

        if(strcmp(mno_check,"mno")==0)
				break;

		fseek(f, -2, SEEK_CUR);
    }

    printf("Arhiva '%s' a fost extrasa cu succes in '%s'!\n", numearhiva, numefisier);
    fclose(f);
    fclose(fo);
}





