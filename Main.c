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


#define BUFFER_SIZE 257 //размер буфера для чтения файла конфигурации
// 257=128(имя параметра)+128(значение)+1(знак равенства)

//static const int bufferSize = 257; 
// char buffer[bufferSize]
// - такой вариант не работает. 
//Длина массива не может задаваться переменной (то есть может, но через malloc).

#define COMMENT_SYMBOL ';' //с этого символа начинается комментарий в файле конфигурации
#define COMMENT_SEPARATOR '=' //этим символом разделяются наименование и значение параметра

int intSetting;
double dblSetting;
char * strSetting;

void WriteSetting(char* settingComment, char* settingName, char* settingValue, FILE* fp)
{
	//всё так сложно - чтобы можно было менять символы комментария 
	// и разделителя имени и значения параметра
	fputc(COMMENT_SYMBOL, fp);
	fputs(settingComment, fp);
	fputc('\n', fp);
	fputs(settingName, fp);
	fputc(COMMENT_SEPARATOR, fp);
	fputs(settingValue, fp);
	fputc('\n', fp);
}

int ParseSettingString(char* string, char * name, char * value) {
	if (string[0] == COMMENT_SYMBOL) return 0;//комментарий пропускаем
	char* ch = name; //сначала читаем в переменную name
	int j = 0; //порядковый номер прочитанного символа
	for (int i=0 ;i< strlen(string);i++, j++)
	{
		if(string[i]== COMMENT_SEPARATOR)	
		{ //если дочитали до разделителя
			ch[j] = '\0'; //завершаем переменную name
			ch = value; //теперь будем читать в переменную value
			j = -1; //обнуляем номер символа
			continue;
		}
		else if (string[i] == '\n')
		{ //если дочитали до конца
			ch[j] = '\0'; //завершаем переменную value
			break;//выходим из цикла
		}
		else
			ch[j] = string[i];	//пишем символ в текущую переменную	 
	}
	return 1;
}

void SettingsCreate()
{
	printf("Файл конфигурации будет восстановлен со значениями по умолчанию.\nПрежняя версия (если она была) - см. options.bak\n");
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
	fputc(COMMENT_SYMBOL, fp);
	fputs("Файл конфигурации программы Maximinus\n", fp);
	fputc(COMMENT_SYMBOL, fp);
	fputs("Внимание! Длина любого параметра не должна превышать 128 символов!\n", fp);
	WriteSetting("Целочисленный параметр", "intSetting", "1", fp);
	WriteSetting("Дробный параметр. Разделитель целой и дробной частей - запятая.", "dblSetting", "0,1", fp);
	WriteSetting("Строковый параметр.", "strSetting", "setting value is 0.1", fp);
	fclose(fp);
}

void SettingsRead()
{
	FILE* fp;
TRYAGAIN:
	//пытаемся открыть файл конфигурации
	fp = fopen("options.dat", "r"); 
	if (fp == NULL)
	{
		//если его нет - создаём и начинаем сначала
		printf("Файл конфигурации не найден\n");
		SettingsCreate();
		goto TRYAGAIN;
	}
	/* Пример записи параметра
	;intSetting - комментарий
	intSetting=1 */
	char buffer[BUFFER_SIZE]; //сюда читаем строку из файла
	char settingName[BUFFER_SIZE / 2 - 1]; // сюда помещаем имя переменной
	char settingValue[BUFFER_SIZE / 2 - 1];// сюда значение
	while ((fgets(buffer, 257, fp)) != NULL)//читаем строку в буфер
	{
		//передаём буфер в функцию ParseSettingString. Она парсит его и 
		//помещает имя и значение параметра в переменные settingName и settingValue соответственно
		if (ParseSettingString(buffer, settingName, settingValue) == 1)
		{ //ParseSettingString вернула 1 - значит не комментарий
			//конвертируем полученную строку settingValue
			/*
				есть простые конвертеры atoi и atof,
				я использовал более продвинутные strtol и strtod
				с возможностью обработки ошибок
			*/
			char* end;/* нужна функциям strtol и strtod 
			сюда передаётся адрес первого непотребного символа
			*/
			if (strcmp(settingName, "intSetting") == 0) //просто так строки не сравнить
			{
				intSetting = strtol(settingValue, &end, 10);//10 - значит десятичная система
				if (end == settingValue) {//то есть строка начинается с непотребства. Ни одной цифры не удалось прочитать
					printf("Не читается значение параметра intSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN; //без goto здесь никак - два цикла (проверка наличия файла и проверка его читаемости)
				}
			}
			else if (strcmp(settingName, "dblSetting") == 0)
			{ //DRY (don`t repeat yourself)? -нет, не слышал... 
				dblSetting = strtod(settingValue, &end);
				if (end == settingValue) {
					printf("Не читается значение параметра dblSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN;
				}
			}
			else if (strcmp(settingName, "strSetting") == 0)
				strSetting = settingValue;
		}
	}
	printf("%d\t%f\t%s\n", intSetting, dblSetting, strSetting);
	fclose(fp);
}



int main() 
{
	char* locale = setlocale(LC_ALL, ""); //локализация
	SettingsRead();//чтение конфигурации
	printf("AVECAESARGAIUSIULIUSVERUSMAXIMINUS!");
	getchar();//даём пользователю прочитать
}