#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>


void Logo()
{	
	printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣶⣄⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  \n");
	printf("⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⣀⡀⣠⣾⡇⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀\n");
	printf("⠀⠀⠀⠀ ⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⢿⣿⣿⡇⠀⠀⠀⠀\n");
	printf("⠀⣶⣿⣦⣜⣿⣿⣿⡟⠻⣿⣿⣿⣿⣿⣿⣿⡿⢿⡏⣴⣺⣦⣙⣿⣷⣄⠀⠀⠀\n");
	printf("⠀⣯⡇⣻⣿⣿⣿⣿⣷⣾⣿⣬⣥⣭⣽⣿⣿⣧⣼⡇⣯⣇⣹⣿⣿⣿⣿⣧⠀⠀\n");
	printf("⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠸⣿⣿⣿⣿⣿⣿⣿⣷ \n");
	printf("Bastrakova Valeri IVT-13\n");
}
void Inputarray(int n, int array[])
{
	for(int i = 0;  i < n; i++)
    {
        printf("введите %d элемент масиива ==> ", (i + 1));
        scanf("%d", &array[i]);
    }  
}
void Outputarray(int n, int array[])
{ 	
	for(int i=0; i<n; i++)
    {
        printf("%d ", array[i]);
	}
	        printf("\n");

}


void Time_stamp(FILE * f)
{
	time_t t = time(NULL);
	fprintf(f, "%s\n", ctime(&t));
}
 void Exit()
{
	getchar();
	exit(0);
}
void Close_File(FILE * file_log, FILE * file_inp, FILE * file_out)
{
	fprintf(file_log, "файл заполнения журнала закрылся ");
        Time_stamp(file_log);
        fprintf(file_log, "файл чтения закрылся ");
        Time_stamp(file_log);
        fprintf(file_log, "файл вывода закрылся ");
        Time_stamp(file_log);
        fclose(file_inp);
        fclose(file_out);
        fclose(file_log);
}
void Open_File(FILE * file_log, FILE * file_inp, FILE * file_out, const char* inpfile)
{
	if(file_log == NULL)
    { 
        printf("файл заполнение журнала log.txt не открылся\n");
        Exit();
    }
    else if(file_inp == NULL)
    {
        fprintf(file_log, "файл для чтения не открылся ");
        Time_stamp(file_log);
        printf("файл %s не открылся\n", &inpfile);
        Exit();
    } 
    else if(file_out == NULL)
    {   
        fprintf(file_log, "файл для вывода не открылся ");
        Time_stamp(file_log);
        printf("файл out.txt не открылся\n");
        Exit();
    }     
    else
    {   
        fprintf(file_log, "программа начала работу ");
        Time_stamp(file_log);
        fprintf(file_log, "файл заполнения журнала открылся ");
        Time_stamp(file_log);
        fprintf(file_log, "файл чтения открылся ");
        Time_stamp(file_log);
        fprintf(file_log, "файл вывода открылся ");
        Time_stamp(file_log);
	}

  
}
/*
read conf
*/
void Trim(char *s)
{
     // удаляем пробелы и табы с начала строки:
     int i=0,j;
     while((s[i]==' ')||(s[i]=='\t')) 
     {
          i++;
     }
     if(i>0) 
     {
          for(j=0; j < strlen(s); j++)
          {
              s[j]=s[j+i];
          }
          s[j]='\0';
     }
 
     // удаляем пробелы и табы с конца строки:
     i=strlen(s)-1;
     while((s[i]==' ')||(s[i]=='\t')) 
     {
          i--;
     }
     if(i < (strlen(s)-1))
     {
          s[i+1]='\0';
     }
}
struct My_Conf
{
	int Quan;
	char Input[5];
};
struct My_Conf *Config_File_Read(FILE *Config)
{
	char Buffer[200];// создание буфера
	struct My_Conf *tmp;// переменнная типа структрура 
	tmp = (struct My_Conf *)malloc(sizeof(struct My_Conf));
	while (1)
	{
		fgets(Buffer, 198, Config);// в буфер произвоизводится считывание считывается 198 символов с файла конфиг
		if (feof(Config)) //достигнут конец файла? на очереде строка пустая выход
		{
			break;
		}
		Trim(Buffer); // удаляем пробелы в строки 
		if (Buffer[0] == '#')
		{
			continue;
		}
		if (Buffer[0] == '!')
		{
			puts(Buffer);
			continue;
		}
		if (strstr(Buffer, "quan="))
		{
			tmp->Quan = atoi(Buffer + strlen("quan="));
			continue;
		}
		if (strstr(Buffer, "input="))
		{
			strcpy(tmp->Input, Buffer + strlen("input="));
			continue;
		}
	}
    return tmp;
}
/*
*/
int Chek_words(char first[], char second[])
{
    int flag = 1;// равны по умолчанию 
    int i = 0;
    while (first[i] != first[4])
    {
       if(first[i] != second[i])
       {
        flag = 0;
        return 0;
        break;
       }
       i++;
    }
    return 1;
}