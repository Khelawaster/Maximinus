#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

/*
13/01/2025
Задания ... выполняются с текстовым конфигурационным файлом
Он должен быть понятно и подробно закомментирован

Задание 1 ?
В списке целых чисел все элементы между Max и Min заменить полусуммой Max и Min

Задание 2 
реализовать на основе списков класс динамический односвязный список целых чисел и выполнить задание 1
Написать производный класс на основе вот этого и выполнить задание для производного класса (номер 3)
Требования к программам с классами: в каждом классе не менее двух конструкторов 
и деструктор, который чистит динамическую память. К и Д выводят на экран сообщения о своей работе

Задание 3
производные классы: стек, очередь или двух концевая очередь. 
Для каждого определяется операции, которые изменяют состояния. 
Стек: push, pop, очередь: тоже, двухконцевая: взять, положить справа и слева
Только нами написанные классы, не массивы и не переменные
Не требуя прав администратора
*/


int intSetting;
double dblSetting;
char * strSetting;

int ParseSettingString(char* string, char * name, char * value) {
	if (string[0] == ';') return 0;
	char* ch = name;
	int j = 0;
	for (int i=0 ;i< strlen(string);i++, j++)
	{
		if(string[i]=='=') 
		{
			ch[j] = '\0';
			ch = value;
			j = -1;
			continue;
		}
		else if (string[i] == '\n')
		{
			ch[j] = '\0';
			break;
		}
		else
			ch[j] = string[i];		 
	}
	return 1;
}

void SettingsCreate()
{
	const char* filename = "options.dat";
	FILE* fp = fopen(filename, "r");
	if (fp)
	{
		fclose(fp);
		rename(filename, "options.bak");
	}
	fp = fopen(filename, "w"); //текстовый файл открывается для записи. 
	//Если файл ранее существовал, то он пересоздается и записывается заново
	if (fp == NULL)
	{
		printf("Ой всё сломалось!!!\nНе могу создать файл конфигурации.");
		_fcloseall();
		_exit(1);
	}
	fputs(";Файл конфигурации программы Maximinus\n", fp);
	fputs(";Внимание! Длина любого параметра не должна превышать 128 символов!\n", fp);
	fputs(";intSetting\nintSetting=1\n", fp);
	fputs(";dblSetting Разделитель целой и дробной частей - запятая.\ndblSetting=0,1\n", fp);
	fputs(";strSetting\nstrSetting=setting value is 0.1\n", fp);
	fclose(fp);
}


void SettingsRead()
{
	FILE* fp;
	for (;;)
	{
		fp = fopen("options.dat", "r"); //текстовый файл открывается для чтения
		if (fp == NULL)
		{
			printf("Файл конфигурации не найден и будет восстановлен со значениями по умолчанию.\n");
			SettingsCreate();
			continue;
		}
		break;
	}
	char buffer[257];
	char settingName[128];
	char settingValue[128];
	while ((fgets(buffer, 256, fp)) != NULL)
	{
		if (ParseSettingString(buffer, settingName, settingValue) == 1)
		{
			//printf("%s\t%s\n", settingName, settingValue);
			if (strcmp(settingName, "intSetting") ==0)
				intSetting = atoi(settingValue);
			else if (strcmp(settingName, "dblSetting") == 0) 
				dblSetting = atof(settingValue);
			else if  (strcmp(settingName, "strSetting") == 0) 
				strSetting = settingValue;
		}
	}
	printf("%d\t%f\t%s\n", intSetting, dblSetting, strSetting);
	fclose(fp);
	return 0;
}


int main() {
	char* locale = setlocale(LC_ALL, ""); //локализация
	SettingsCreate();
	SettingsRead();
	printf("AVE CAESAR");
	getchar();
}