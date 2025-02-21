#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

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
*/


#define BUFFER_SIZE 257 //������ ������ ��� ������ ����� ������������
// 257=128(��� ���������)+128(��������)+1(���� ���������)

//static const int bufferSize = 257; 
// char buffer[bufferSize]
// - ����� ������� �� ��������. 
//����� ������� �� ����� ���������� ���������� (�� ���� �����, �� ����� malloc).

#define COMMENT_SYMBOL ';' //� ����� ������� ���������� ����������� � ����� ������������
#define COMMENT_SEPARATOR '=' //���� �������� ����������� ������������ � �������� ���������

int intSetting;
double dblSetting;
char * strSetting;

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

int ParseSettingString(char* string, char * name, char * value) {
	if (string[0] == COMMENT_SYMBOL) return 0;//����������� ����������
	char* ch = name; //������� ������ � ���������� name
	int j = 0; //���������� ����� ������������ �������
	for (int i=0 ;i< strlen(string);i++, j++)
	{
		if(string[i]== COMMENT_SEPARATOR)	
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
	WriteSetting("������������� ��������", "intSetting", "1", fp);
	WriteSetting("������� ��������. ����������� ����� � ������� ������ - �������.", "dblSetting", "0,1", fp);
	WriteSetting("��������� ��������.", "strSetting", "setting value is 0.1", fp);
	fclose(fp);
}

void SettingsRead()
{
	FILE* fp;
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
			if (strcmp(settingName, "intSetting") == 0) //������ ��� ������ �� ��������
			{
				intSetting = strtol(settingValue, &end, 10);//10 - ������ ���������� �������
				if (end == settingValue) {//�� ���� ������ ���������� � ������������. �� ����� ����� �� ������� ���������
					printf("�� �������� �������� ��������� intSetting %s\n", settingValue);
					SettingsCreate();
					goto TRYAGAIN; //��� goto ����� ����� - ��� ����� (�������� ������� ����� � �������� ��� ����������)
				}
			}
			else if (strcmp(settingName, "dblSetting") == 0)
			{ //DRY (don`t repeat yourself)? -���, �� ������... 
				dblSetting = strtod(settingValue, &end);
				if (end == settingValue) {
					printf("�� �������� �������� ��������� dblSetting %s\n", settingValue);
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
	char* locale = setlocale(LC_ALL, ""); //�����������
	SettingsRead();//������ ������������
	printf("AVECAESARGAIUSIULIUSVERUSMAXIMINUS!");
	getchar();//��� ������������ ���������
}