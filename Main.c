#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Additions.c"
#include "MyList.h"
void main(int argc, char* argv[])
{

    Logo();
    printf("задание: в списке целых чисел подсчитать количество локальных миноров вывести их номера a(i-1)>a(i)<a(i+1)\n");
    printf("количество элментов и формат ввода предоставить в файле 6conf.cfg\n");
    getchar();
    FILE* Config;
    //open cfg
    if ((Config = fopen("6conf.cfg", "r")) == NULL)
    {
        puts("Ошибка в конфигурационном файле!!!");
        puts("выполнение невозможно!!!");
        Exit();
    };
    //read cfg
    struct My_Conf* Con;
    Con = Config_File_Read(Config);
    //get data cfg
    int quantity = Con->Quan;
    int tmp_el;
    //fill struct
    struct MyList* List;
    List = Create();
    char r[4] = "rand";
    char h[4] = "hand";
    if (Chek_words(h, Con->Input) == 1)
    {
        for (int i = 0; i < quantity; i++)
        {
            printf("введите %d элемент списка ==> ", (i + 1));
            scanf("%d", &tmp_el);
            Append(List, tmp_el);
        }
    }
    else if (Chek_words(r, Con->Input) == 1)
    {
        for (int i = 0; i < quantity; i++)
        {
            Append(List, rand() % 100);
        }
    }
    else
    {
        printf("неверно введенны данные в 6conf.cfg\n");
        Exit();
    }

    printf("список введенные целых чисел\n");
    Print(List);
    //calculate
    int cur, last;
    if (quantity <= 2)
    {
        printf("локального минора нет");
        Exit();
    }
    for (int i = 2; i <= (quantity - 1); i++)
    {
        if (Get_Value(List, i - 1) > Get_Value(List, i))
        {
            if (Get_Value(List, i) < Get_Value(List, i + 1))
            {
                printf("номер элемента локального минора = %d\n", i);
            }
        }
        else
        {
            continue;
        }
    }
    fclose(Config);
    Purge(List, 1);

    Exit();

}
