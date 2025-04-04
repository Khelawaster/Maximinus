struct MyList
{
	int Info;
	struct MyList *Next;
};

struct MyList *Create(void)
{
	struct MyList *tmp; // переменна тмп с типом данных структура майлист 
	tmp = (struct MyList *)malloc(sizeof(struct MyList)); // приводим к типу стракт майлист указатель на выделенную память для одного элемента структуры 
	tmp->Info = 0;
	tmp->Next = NULL;
	return tmp;// возврат указателя на пусую структуру   
} 

struct MyList *Get_Address(struct MyList *Start, int Num)
{
	int Count;
	if (Num < 1)
		return Start;
	if (Num > Start->Info)
		Num = Start->Info;
	struct MyList *tmp = Start;
	for (Count = 1; Count <= Num; Count++)
		tmp = tmp->Next;
	return tmp;
}

int Get_Value(struct MyList *Start, int Num)
{
	struct MyList *tmp;
	tmp = Get_Address(Start, Num);
	return tmp->Info;
}

void Set_Value(struct MyList *Start, int Num, int Data)
{
	struct MyList *tmp;
	tmp = Get_Address(Start, Num);
	tmp->Info = Data;
	return;
}

int Empty(struct MyList *Start)
{
	if (Start->Info)
		return 0;
	return 1;
}

int Amount(struct MyList *Start)
{
	return Start->Info;
}

void Remove(struct MyList *Start, int Num)
{
	struct MyList *Before, *Out;
	Before = Get_Address(Start, Num - 1);
	Out = Get_Address(Start, Num);
	Before->Next = Out->Next;
	free(Out);
	Start->Info--;
	return;
}
void Purge(struct MyList *Start, int Test)
{
	while (!Empty(Start))
		Remove(Start, 1);
	if (Test)
		puts("Мой список УНИЧТОЖЕН!!! Используйте функцию Создания! ");
	free(Start);
	Start = NULL;
}

void Append(struct MyList *Start, int Data)
{
	struct MyList *tmp, *Last;
	tmp = (struct MyList *)malloc(sizeof(struct MyList));// указатель на выд память 
	tmp->Info = Data; //запись значений в нее
	tmp->Next = NULL;// типа тмп последний элемент спика
	Last = Start;//новая переменная ласт примает значения первого элемента списка
	while (1)
	{
		if (Last->Next == NULL) //если первый элемнет списка последний выход  
			break;
		Last = Last->Next;// иначе чекаем следующий элмент на то является ли он последним
	}
	Last->Next = tmp; //ссылка последнего элемента направлена на созданный нами выше узел   
	Start->Info++; //количесво элементов в списке +1
}

void Insert(struct MyList *Start, int Num, int Data)
{
	if (Empty(Start))
	{
		Append(Start, Data);
		return;
	}

	struct MyList *Fresh, *Before;
	Before = Get_Address(Start, Num - 1);
	Fresh = (struct MyList *)malloc(sizeof(struct MyList));
	Fresh->Info = Data;
	Fresh->Next = Before->Next;
	Before->Next = Fresh;
	Start->Info++;
}
void Print(struct MyList *Start)
{
	struct MyList *tmp = Start; //в переменную тмп складываем указатель на первый элемент структуры нашей
	if (Start->Info == 0) //проверяем на наличие элементов в списке 
	{
		puts("список ПУСТ!");
		return;
	}
	printf("всего элементов [%d]->%p \n", Start->Info, Start->Next);
	while (1)
	{
		tmp = tmp->Next;//адресс элемента следующего за текущим начиная с первого в тмп
		printf("%d  -> %p ", tmp->Info, tmp->Next);//%d int %p адресс указателя
		if (tmp->Next == NULL) //если последний выход
			break;
	}
	printf("\n");
	return;
}

void Exchange(struct MyList *Start, int N1, int N2)
{
	struct MyList *Before1, *Before2, *Sam1, *Sam2, *tmp;
	Before1 = Get_Address(Start, N1 - 1);
	Before2 = Get_Address(Start, N2 - 1);
	Sam1 = Get_Address(Start, N1);
	Sam2 = Get_Address(Start, N2);
	tmp = Before1->Next;
	Before1->Next = Before2->Next;
	Before2->Next = tmp;
	tmp = Sam1->Next;
	Sam1->Next = Sam2->Next;
	Sam2->Next = tmp;
	return;
}
