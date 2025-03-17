#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
13/01/2025
������� ... ����������� � ��������� ���������������� ������
�� ������ ���� ������� � �������� ���������������

������� 1 ?
� ������ ����� ����� ��� �������� ����� Max � Min �������� ���������� Max � Min

������� 2 
����������� �� ������ ������� ����� ������������ ����������� ������ ����� ����� � ��������� ������� 1
�������� ����������� ����� �� ������ ��� ����� � ��������� ������� ��� ������������ ������ (����� 3)
���������� � ���������� � ��������: � ������ ������ �� ����� ���� ������������� 
� ����������, ������� ������ ������������ ������. � � � ������� �� ����� ��������� � ����� ������

������� 3
����������� ������: ����, ������� ��� ���� �������� �������. 
��� ������� ������������ ��������, ������� �������� ���������. 
����: push, pop, �������: ����, ������������: �����, �������� ������ � �����
������ ���� ���������� ������, �� ������� � �� ����������
�� ������ ���� ��������������

�������:
1) � ������� 2 - ��� ������ ������������ ������

1-13 �����������, Create
14-26 Append *
29-41 Print
43-53 GetAddress *
58-63 Remove *
64-69 Purge *
74-85 Insert *
*/
const int InfoMaxRandomValue = 99;// ������������ �������� Info � ��������� ������

#pragma region Settings
#define BUFFER_SIZE 257 //������ ������ ��� ������ ����� ������������
// 257=128(��� ���������)+128(��������)+1(���� ���������)

//static const int bufferSize = 257; 
// char buffer[bufferSize]
// - ����� ������� �� ��������. 
//����� ������� �� ����� ���������� ���������� (�� ���� �����, �� ����� malloc).

#define COMMENT_SYMBOL ';' //� ����� ������� ���������� ����������� � ����� ������������
#define COMMENT_SEPARATOR '=' //���� �������� ����������� ������������ � �������� ���������

int ListDefaultLength;
int ListElementDefaultInfo;
double dblSetting;
char * strSetting;
const int SettingsCount = 4;//��� �������� ����� ����������� ����������

void WriteSetting(char* settingComment, char* settingName, char* settingValue, FILE* fp)
{
	//�� ��� ������ - ����� ����� ���� ������ ������� ����������� 
	// � ����������� ����� � �������� ���������
	fputc(COMMENT_SYMBOL, fp);
	fputs(settingComment, fp);
	fputc('\n', fp);
	fputs(settingName, fp);
	fputc(COMMENT_SEPARATOR, fp);
	fputs(settingValue, fp);
	fputc('\n', fp);
}

int ParseSettingString(char* string, char* name, char* value) {
	if (string[0] == COMMENT_SYMBOL) return 0;//����������� ����������
	char* ch = name; //������� ������ � ���������� name
	int j = 0; //���������� ����� ������������ �������
	for (int i = 0; i < strlen(string); i++, j++)
	{
		if (string[i] == COMMENT_SEPARATOR)
		{ //���� �������� �� �����������
			ch[j] = '\0'; //��������� ���������� name
			ch = value; //������ ����� ������ � ���������� value
			j = -1; //�������� ����� �������
			continue;
		}
		else if (string[i] == '\n')
		{ //���� �������� �� �����
			ch[j] = '\0'; //��������� ���������� value
			break;//������� �� �����
		}
		else
			ch[j] = string[i];	//����� ������ � ������� ����������	 
	}
	return 1;
}

void SettingsCreate()
{
	printf("���� ������������ ����� ������������ �� ���������� �� ���������.\n������� ������ (���� ��� ����) - ��. options.bak\n");
	const char* filename = "options.dat";
	FILE* fp = fopen(filename, "r");
	if (fp)
	{
		fclose(fp);
		rename(filename, "options.bak");
	}
	fp = fopen(filename, "w"); //��������� ���� ����������� ��� ������. 
	//���� ���� ����� �����������, �� �� ������������� � ������������ ������
	if (fp == NULL)
	{
		printf("�� �� ���������!!!\n�� ���� ������� ���� ������������.");
		_fcloseall();
		_exit(1);
	}
	fputc(COMMENT_SYMBOL, fp);
	fputs("���� ������������ ��������� Maximinus\n", fp);
	fputc(COMMENT_SYMBOL, fp);
	fputs("��������! ����� ������ ��������� �� ������ ��������� 128 ��������!\n", fp);
	WriteSetting("����� ������ �� ���������", "ListDefaultLength", "9", fp);
	WriteSetting("�������� �������� ������ �� ���������", "ListElementDefaultInfo", "0", fp);
	WriteSetting("������� ��������. ����������� ����� � ������� ������ - �������.", "dblSetting", "0,1", fp);
	WriteSetting("��������� ��������.", "strSetting", "setting value is 0.1", fp);
	fclose(fp);
}

void SettingsRead()
{
	FILE* fp;
	int cnt = 0; //������� ����������� ����������
TRYAGAIN:
	//�������� ������� ���� ������������
	fp = fopen("options.dat", "r");
	if (fp == NULL)
	{
		//���� ��� ��� - ������ � �������� �������
		printf("���� ������������ �� ������\n");
		SettingsCreate();
		goto TRYAGAIN;
	}
	/* ������ ������ ���������
	;intSetting - �����������
	intSetting=1 */
	char buffer[BUFFER_SIZE]; //���� ������ ������ �� �����
	char settingName[BUFFER_SIZE / 2 - 1]; // ���� �������� ��� ����������
	char settingValue[BUFFER_SIZE / 2 - 1];// ���� ��������
	while ((fgets(buffer, 257, fp)) != NULL)//������ ������ � �����
	{
		//������� ����� � ������� ParseSettingString. ��� ������ ��� � 
		//�������� ��� � �������� ��������� � ���������� settingName � settingValue ��������������
		if (ParseSettingString(buffer, settingName, settingValue) == 1)
		{ //ParseSettingString ������� 1 - ������ �� �����������
			//������������ ���������� ������ settingValue
			/*
				���� ������� ���������� atoi � atof,
				� ����������� ����� ������������ strtol � strtod
				� ������������ ��������� ������
			*/
			char* end;/* ����� �������� strtol � strtod
			���� ��������� ����� ������� ������������ �������
			*/
			if (strcmp(settingName, "ListDefaultLength") == 0) //������ ��� ������ �� ��������
			{
				ListDefaultLength = strtol(settingValue, &end, 10);//10 - ������ ���������� �������
				if (end == settingValue) {//�� ���� ������ ���������� � ������������. �� ����� ����� �� ������� ���������
					printf("�� �������� �������� ��������� intSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN; //��� goto ����� ����� - ��� ����� (�������� ������� ����� � �������� ��� ����������)
				}
				cnt++;
			}
			if (strcmp(settingName, "ListElementDefaultInfo") == 0) //������ ��� ������ �� ��������
			{
				ListElementDefaultInfo = strtol(settingValue, &end, 10);//10 - ������ ���������� �������
				if (end == settingValue) {//�� ���� ������ ���������� � ������������. �� ����� ����� �� ������� ���������
					printf("�� �������� �������� ��������� intSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN; //��� goto ����� ����� - ��� ����� (�������� ������� ����� � �������� ��� ����������)
				}
				cnt++;
			}
			else if (strcmp(settingName, "dblSetting") == 0)
			{ //DRY (don`t repeat yourself)? -���, �� ������... 
				dblSetting = strtod(settingValue, &end);
				if (end == settingValue) {
					printf("�� �������� �������� ��������� dblSetting %s\n", settingValue);
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
		printf("��������� �� ��� ���������\n");
		SettingsCreate();
		goto TRYAGAIN;
	}
	//printf("%d\t%f\t%s\n", ListDefaultLength, dblSetting, strSetting);
	fclose(fp);
}
#pragma endregion

#pragma region List
struct MyList {
	int Info; //� ������� ����� ��� �������� ����� ����� ���� ��������� ���������
	struct MyList* Next;
};

void Append(struct MyList* start, struct MyList* link);
void Insert(struct MyList* start, int num, int data);

#pragma region Constructors

struct MyList* Create(void) {
	struct MyList* ret;
	ret = (struct MyList*)malloc(sizeof(struct MyList));
	ret->Info = ListElementDefaultInfo;
	ret->Next = NULL;
	printf("�� ������ %p ����������� ����� �����. �������� �� ���������.\n",ret);
	return ret;
}

struct MyList* CreateWithValue(int value) {
	struct MyList* ret;
	ret = Create();
	ret->Info = value;
	printf("����������� �������� ����� �� ������ %p : %d\n", ret , value);
	return ret;
}

//��� � ��� �������. ��������� ������ �������� ����� �� ��������� ����������.
struct MyList* CreateWithLengthRandom(int length) {
	printf("��������� ������ ��������� ����� ������ %d\n", length);
	srand(time(NULL));
	int max = InfoMaxRandomValue + 1;
	struct MyList* ret = CreateWithValue(0);
	for (int i = 1; i <= length; i++)
		Insert(ret,i, rand() % max);
	printf("��������� ������ ���������\n\n");
	return ret;
}


//��� � ��� �������. ��������� ������ �������� ����� �� ��������� �� ���������.
struct MyList* CreateWithLengthDefault(int length) {
	printf("��������� ������ ����� ������ %d �� ��������� %�\n", length, ListElementDefaultInfo);
	struct MyList* ret = CreateWithValue(length);
	for (int i = 1; i <= length; i++)
		Insert(ret,ret->Info, ListElementDefaultInfo);
	printf("��������� ������ ���������\n\n");
	return ret;
}
#pragma endregion

#pragma region Methods


struct MyList* GetAddress(struct MyList* start, int num) {
	if (num < 1) return start;
	if (num > start->Info) num = start->Info; //���� ����� ������� �� ������� ������ - ������������ ���������
	struct MyList* tmp = start;
	for (int i = 0; i < num; i++) tmp = tmp->Next;
	return tmp;
}

void Append(struct MyList* start, struct MyList* link) {
	//�� ����� ����������, ���� Next!=NULL - ��� ������� ���������� ������ ����� ����� �� ����...
	struct MyList* last;
	last = GetAddress(start, start->Info);
	last->Next = link;
	start->Info++;
}

//���������� �������� ��������� ��������. ����� ������������ ��� Pop � Dequeue
int Remove(struct MyList* start, int num) {
	struct MyList* before, * out;
	before = GetAddress(start, num - 1);
	out = before->Next;
	before->Next = out->Next;
	int ret = out->Info;
	free(out);
	start->Info--;
	return ret;
}

void Insert(struct MyList* start, int num, int data) {
	struct MyList* before, * new;
	before = GetAddress(start, num - 1);
	new = CreateWithValue(data);
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



/*
��� ������ ��� ������ �� ������� ������� ��� �� ������
*/

void Push  (struct MyList* start, int data) {
//	�� �� Enqueue 
	Insert(start, start->Info+1, data);
}


//���� ��� ������� ����������. ������ ��� ��� ���������� ������ Remove
int Pop(struct MyList* list) {
	return Remove (list, list->Info);
}
int Dequeue(struct MyList* list) {
	return Remove (list, 1);
}
//���������� � ������ �������. ���� ������ ��� Insert
void EnqueueHead(struct MyList* list, int data) {
	Insert(list, 1, data);
}

#pragma region Sort

void Concat (struct MyList* head, struct MyList* tail) {
	//������� ������ ��� ������
	struct MyList* last = GetAddress(head, head->Info);
	last->Next = tail->Next;
	head->Info += tail->Info;
	last= GetAddress(head, head->Info);
	last->Next = NULL;
	free(tail);
}

struct MyList* QuickSort(struct MyList* list) {
	//���������� ����������� �������� ������� ����������
	if (list->Info < 2)
		return list; //������� ������
	else {
		struct MyList* pivot = list->Next; // ������ ������� - �������
		struct MyList* head = Create(); // ���� ��, ��� ������ ��������
		struct MyList* tail = Create(); // ���� ��, ��� ������
		struct MyList* tmp = pivot->Next; // ���������� ��� ��������, ����������� �� �� �������
		for (int i = 0; i < list->Info-1; i++) {
			if (tmp->Info <= pivot->Info)
				Append(head, tmp);
			else
				Append(tail, tmp);
			tmp = tmp->Next;
		}
		Append(head, pivot);//������� - � "������"
		Concat(QuickSort(head), QuickSort(tail)); //�������� - ������� �������� ���� ���� ��� "������" � "������"
		//��� ����� ������������, ���� � ��� �� ��������� �� ������ �������� (����� ��������� ������� ������)
		list->Next = head->Next;//�� ���������� �������� ������
		free(head);//"�����" ��� ��������� �������� Concat
		return list;
	}

}

#pragma endregion


#pragma endregion

// ����������: ������������ ������ ������
void freeList(struct MyList* start) {
	printf("�������� ������ %p\n", &start);
	struct MyList* tmp = start;
	struct MyList* next;
	int i=0;
	while (tmp != NULL) {
		printf("�������� ����� %d �� ������ %p\n",i, tmp );
		next = tmp->Next;
		free(tmp);
		tmp = next;
		i++;
	}
	start = NULL;
	printf("%p = %p",&start, start);
	printf("\n������� ������ ���������\n\n");
}

#pragma endregion

#pragma region Stack
/*��� ��������� � ��������� �������. ������������ ����� � ��� ���� ������ ���� ������ � ������ ����������� ������,
�� ���� � ������ ������ ��� ���. ���� ����� ������������ - �������� ��� ������ "�����������" �������*/
struct MyStack{
	struct MyList * base;
	void (*Push) (struct Stack*, int);
	int (*Pop) (struct Stack*);
} ;

// �������� ������� push
void push(struct MyStack* stack, int data);
int pop(struct MyStack* stack);

struct MyStack* CreateStack() {
	struct MyList* lst = Create();
	struct MyStack* ret;
	ret= (struct MyStack*)malloc(sizeof(struct MyStack));
	ret->base = lst;
	ret->Push = push;
	ret->Pop = pop;
	return ret;
}

void push(struct MyStack* stack, int data)
{
	Insert(stack->base, stack->base->Info, data);
}
int pop(struct MyStack* stack)
{
	int lastIndex = (stack->base)->Info;
	struct MyList* lst = GetAddress(stack->base, lastIndex);
	int ret = lst->Info;
	Remove(stack->base, lastIndex);
	return ret;
}


#pragma endregion



#pragma region Tasks
#pragma region Task 1
void MaximineListByValue(struct MyList* list) {
	int min = INT_MAX;	int max = INT_MIN;
	struct MyList* tmp = list->Next; //����, ������� � ������� ��������
	while (1) {
		if (tmp->Info <= min)
			min = tmp->Info;
		if (tmp->Info >= max)
			max = tmp->Info;
		if (tmp->Next == NULL)
			break;
		tmp = tmp->Next;
	}
	int Maximinus = (min + max) / 2;
	printf("�������� %d, ������� %d, ��������� %d\n\n", max, min, Maximinus);
	//������ �������� �� ������� ���� � ������ �������� ���������
	tmp = list->Next;
	while (tmp->Next != NULL) {
		if (tmp->Info != min && tmp->Info != max)
			tmp->Info = Maximinus;
		tmp = tmp->Next;
	}
}

void MaximineListByPosition(struct MyList* list) {
	/*min->Info - ����������� ��������
	min->Next - ����� �������� � ���� ���������*/
	printf("������ ��������� ����������\n");
	struct MyList* min = CreateWithValue(INT_MAX);
	struct MyList* max = CreateWithValue(INT_MIN);
	//�������� ������ ��� - ���� ���� � ���
	struct MyList* tmp = list->Next;
	while (1) {
		if (tmp->Info <= min->Info) {
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
	printf("�������� %d, ������� %d, ��������� %d\n\n", max->Info, min->Info, Maximinus);
	//�������� ������ ��� - ������ ��������
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
void Task1(void) {
	struct MyList* lst = CreateWithLengthRandom(ListDefaultLength);
	Print(lst);
	//MaximineListByValue(lst);
	MaximineListByPosition(lst);
	printf("��������� ������� 1\n� ������ ����� ����� ��� �������� ����� Max � Min �������� ���������� Max � Min\n");
	//printf("������� 1: \"�����\" - ��� �� ��������\n");
	printf("������� 2: \"�����\" - ��� �������� ���������\n");
	Print(lst);
	freeList(lst);
}

#pragma endregion

#pragma region StackAndQueue
void StackAndQueue () {
	printf("�������� �����\n");
	struct MyStack* st = CreateStack();
	st->Push(st, 13);
	Push(st->base, 26);
	Push(st->base, 39);
	EnqueueHead(st->base, 7);
	Print(st->base);
	Append(st->base, CreateWithValue(666));


	int i = Dequeue(st->base);
	printf("\n%d\n",i);
	i = st->Pop(st);
	printf("\n%d\n", i);
	i = Pop(st->base);
	printf("\n%d\n",i);
	Print(st->base);
}


#pragma endregion

void Sort() {
	struct MyList* lst = CreateWithLengthRandom(13);
	Print(lst);
	QuickSort(lst);
	Print(lst);
}
#pragma endregion

int main() 
{
	char* locale = setlocale(LC_ALL, ""); //�����������
	SettingsRead();//������ ������������
	//Task1();
	//StackAndQueue();
	Sort();
	printf("AVECAESARGAIUSIULIUSVERUSMAXIMINUS!");
	getchar();//��� ������������ ���������
}