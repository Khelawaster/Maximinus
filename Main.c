#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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

Вопросы:
1) К заданию 2 - что значит динамический список

1-13 определение, Create
14-26 Append *
29-41 Print
43-53 GetAddress *
58-63 Remove *
64-69 Purge *
74-85 Insert *
*/
const int InfoMaxRandomValue = 99;// максимальное значение Info в случайном списке

#pragma region Settings
#define BUFFER_SIZE 257 //размер буфера для чтения файла конфигурации
// 257=128(имя параметра)+128(значение)+1(знак равенства)

//static const int bufferSize = 257; 
// char buffer[bufferSize]
// - такой вариант не работает. 
//Длина массива не может задаваться переменной (то есть может, но через malloc).

#define COMMENT_SYMBOL ';' //с этого символа начинается комментарий в файле конфигурации
#define COMMENT_SEPARATOR '=' //этим символом разделяются наименование и значение параметра

int ListDefaultLength;
int ListElementDefaultInfo;
double dblSetting;
char * strSetting;
const int SettingsCount = 4;//для контроля числа прочитанных параметров

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

int ParseSettingString(char* string, char* name, char* value) {
	if (string[0] == COMMENT_SYMBOL) return 0;//комментарий пропускаем
	char* ch = name; //сначала читаем в переменную name
	int j = 0; //порядковый номер прочитанного символа
	for (int i = 0; i < strlen(string); i++, j++)
	{
		if (string[i] == COMMENT_SEPARATOR)
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
	WriteSetting("Длина списка по умолчанию", "ListDefaultLength", "9", fp);
	WriteSetting("Значение элемента списка по умолчанию", "ListElementDefaultInfo", "0", fp);
	WriteSetting("Дробный параметр. Разделитель целой и дробной частей - запятая.", "dblSetting", "0,1", fp);
	WriteSetting("Строковый параметр.", "strSetting", "setting value is 0.1", fp);
	fclose(fp);
}

void SettingsRead()
{
	FILE* fp;
	int cnt = 0; //счётчик прочитанных параметров
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
			if (strcmp(settingName, "ListDefaultLength") == 0) //просто так строки не сравнить
			{
				ListDefaultLength = strtol(settingValue, &end, 10);//10 - значит десятичная система
				if (end == settingValue) {//то есть строка начинается с непотребства. Ни одной цифры не удалось прочитать
					printf("Не читается значение параметра intSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN; //без goto здесь никак - два цикла (проверка наличия файла и проверка его читаемости)
				}
				cnt++;
			}
			if (strcmp(settingName, "ListElementDefaultInfo") == 0) //просто так строки не сравнить
			{
				ListElementDefaultInfo = strtol(settingValue, &end, 10);//10 - значит десятичная система
				if (end == settingValue) {//то есть строка начинается с непотребства. Ни одной цифры не удалось прочитать
					printf("Не читается значение параметра intSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN; //без goto здесь никак - два цикла (проверка наличия файла и проверка его читаемости)
				}
				cnt++;
			}
			else if (strcmp(settingName, "dblSetting") == 0)
			{ //DRY (don`t repeat yourself)? -нет, не слышал... 
				dblSetting = strtod(settingValue, &end);
				if (end == settingValue) {
					printf("Не читается значение параметра dblSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN;
				}
				cnt++;
			}
			else if (strcmp(settingName, "strSetting") == 0)
			{
				strSetting = settingValue;
				cnt++;
			}
		}
	}
	if (cnt != SettingsCount) {
		printf("Прочитаны не все параметры\n");
		SettingsCreate();
		goto TRYAGAIN;
	}
	//printf("%d\t%f\t%s\n", ListDefaultLength, dblSetting, strSetting);
	fclose(fp);
}
#pragma endregion

#pragma region List
struct MyList {
	int Info; //у первого звена это значение равно числу всех остальных элементов
	struct MyList* Next;
};

struct MyList* Create(void) {
	struct MyList* tmp;
	tmp = (struct MyList*)malloc(sizeof(struct MyList));
	tmp->Info = 0;
	tmp->Next = NULL;
	return tmp;
}

void Append(struct MyList* start, int data) {
	struct MyList* tmp, * last;
	tmp = Create();
	tmp->Info = data;
	last = start;
	while (1) {
		if (last->Next == NULL) break;
		last = last->Next;
	}
	last->Next = tmp;
	start->Info++;
}

struct MyList* GetAddress(struct MyList* start, int num) {
	if (num < 1) return start;
	if (num > start->Info) num = start->Info; //если адрес выходит за пределы списка - возврашается последний
	struct MyList* tmp = start;
	for (int i = 0; i < num; i++) tmp = tmp->Next;
	return tmp;
}

void Remove(struct MyList* start, int num) {
	struct MyList* before, * out;
	before = GetAddress(start, num - 1);
	out = before->Next;
	before->Next = out->Next;
	free(out);
	start->Info--;
}

void Insert(struct MyList* start, int num, int data) {
	struct MyList* before, * new;
	before = GetAddress(start, num - 1);
	new = (struct MyList*) malloc(sizeof(struct  MyList));
	new->Info = data;
	new->Next = before->Next;
	before->Next = new;
	start->Info++;
}

void Purge(struct MyList* start) {
	while (start->Info > 0) Remove(start, 1);
	return;
}

void Print(struct MyList* start) {
	if (start->Info < 1) {
		puts("My List is EMPTY! ");
		return;
	}
	struct MyList* tmp = start;
	while (1)
	{
		tmp = tmp->Next;
		printf("%d -> %p\n", tmp->Info, tmp->Next);
		if (tmp->Next == NULL) break;
	}
	printf("\n");
	return;
}

//это я сам добавил. Генерация списка заданной длины со случайныи значениями.
struct MyList* PopulateRandom(int length) {
	srand(time(NULL));
	int max = InfoMaxRandomValue + 1;
	struct MyList* ret =  Create();
	for (int i = 0; i < length; i++)
		Append(ret, rand() % max);
	return ret;
}
//это тоже я добавил. Вытащить элемент списка по номеру.
struct MyList* GetItem(struct MyList* list, int number) {
	struct MyList* tmp = list->Next;
	int i=0;
	while (tmp->Next != NULL) {
		if (i == number)
			return tmp;
		i++;
	}
	return NULL;
}

void MaximineListByValue(struct MyList* list) {
	int min = INT_MAX;	int max = INT_MIN;
	struct MyList* tmp = list->Next; //ищем, начиная с первого элемента
	while(1) {
		if (tmp->Info <= min)
			min = tmp->Info;
		if (tmp->Info >= max)
			max = tmp->Info;
		if (tmp->Next == NULL)
			break;
		tmp = tmp->Next;
	}
	int Maximinus = (min + max) / 2;
	printf("Максимум %d, минимум %d, полусумма %d\n\n", max, min, Maximinus);
	//теперь проходим по второму разу и меняем значения элементов
	tmp = list->Next;
	while (tmp->Next != NULL) {
		if (tmp->Info != min && tmp->Info != max) 
			tmp->Info = Maximinus;
		tmp = tmp->Next;
	}
}

void MaximineListByPosition(struct MyList* list) {
	/*min->Info - минимальное значение
	min->Next - адрес элемента с этим значением*/
	struct MyList* min = Create();
	min->Info=INT_MAX;
	struct MyList* max = Create();
	max->Info = INT_MIN;
	//проходим первый раз - ищем макс и мин
	struct MyList* tmp = list->Next;
	while (1) {
		if (tmp->Info <= min->Info){
			min->Info = tmp->Info;
			min->Next = tmp;
		}
		if (tmp->Info >= max->Info) {
			max->Info = tmp->Info;
			max->Next = tmp;
		}
		if (tmp->Next == NULL)
			break;
		tmp = tmp->Next;
	}
	int Maximinus = (min->Info + max->Info) / 2;
	printf("Максимум %d, минимум %d, полусумма %d\n\n", max->Info, min->Info, Maximinus);
	//проходим второй раз - меняем элементы
	tmp = list->Next;
	char OnOff = 0;
	while (tmp->Next != NULL) {
		if (tmp == min->Next || tmp == max->Next) {
			OnOff = abs(OnOff - 1);
		}
		else if (OnOff == 1)
			tmp->Info = Maximinus;
		tmp = tmp->Next;
	}
}



#pragma endregion


int main() 
{
	char* locale = setlocale(LC_ALL, ""); //локализация
	SettingsRead();//чтение конфигурации
	printf("Генерация списка случайных чисел\n");
	struct MyList* lst = PopulateRandom(ListDefaultLength);
	Print(lst);
	//MaximineListByValue(lst);
	MaximineListByPosition(lst);
	printf("Выполняем задание 1\nВ списке целых чисел все элементы между Max и Min заменить полусуммой Max и Min\n");
	//printf("Вариант 1: \"Между\" - это по значению\n");
	printf("Вариант 2: \"Между\" - это согласно положению\n");
	Print(lst);
	printf("AVECAESARGAIUSIULIUSVERUSMAXIMINUS!");
	getchar();//даём пользователю прочитать
}