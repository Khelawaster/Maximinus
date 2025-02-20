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
	fp = fopen(filename, "w"); //��������� ���� ����������� ��� ������. 
	//���� ���� ����� �����������, �� �� ������������� � ������������ ������
	if (fp == NULL)
	{
		printf("�� �� ���������!!!\n�� ���� ������� ���� ������������.");
		_fcloseall();
		_exit(1);
	}
	fputs(";���� ������������ ��������� Maximinus\n", fp);
	fputs(";��������! ����� ������ ��������� �� ������ ��������� 128 ��������!\n", fp);
	fputs(";intSetting\nintSetting=1\n", fp);
	fputs(";dblSetting ����������� ����� � ������� ������ - �������.\ndblSetting=0,1\n", fp);
	fputs(";strSetting\nstrSetting=setting value is 0.1\n", fp);
	fclose(fp);
}


void SettingsRead()
{
	FILE* fp;
	for (;;)
	{
		fp = fopen("options.dat", "r"); //��������� ���� ����������� ��� ������
		if (fp == NULL)
		{
			printf("���� ������������ �� ������ � ����� ������������ �� ���������� �� ���������.\n");
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
	char* locale = setlocale(LC_ALL, ""); //�����������
	SettingsCreate();
	SettingsRead();
	printf("AVE CAESAR");
	getchar();
}