#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "asd.h"

int main()
{
    int prima_data=1;
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


        if(strcmp(comanda, "iesire")==0)
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
