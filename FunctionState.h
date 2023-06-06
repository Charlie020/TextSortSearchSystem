#pragma once
#include <iostream>
using namespace std;

void Choice();                           // ѡ��
void Pretreat();                         // Ԥ����
bool checkStopWord(string str);
void readStopword();                     // ��ȡͣ�ôʱ�
void Index(string FileName);         // ��������
void BuildWordIndex(string str, int& allwordsNum, string FileInfoName);

// ����ؼ��ʵ�����ͼ���
void showInputMesg();
void CalculateTF_IDF();
void Output();                           // ���������Ϣ
void clear();                            // ��������

void ShowFile();                         // �����ڲ鿴�ı���Ϣ
void ImportFile();                          // �½��ļ�
void DeleteFile();                       // ɾ���ļ�
void DeleteData(string FileInfoName);
void DeleteData_ByLine(string str, string FileInfoName);
void removeDir(const char* dirPath);

void StopWords();                        //����ͣ�ô�

bool check(string str) {
	if (str.length() != 1) return false;
	if (str[0] < 48 || (str[0] > 53 && str[0] != '9')) return false;
	return true;
}

string upper(string str) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] >= 97) str[i] -= 32;
	}
	return str;
}
// �ж��Ƿ񵽴��˵��ʵ�ĩβ
bool wordEnd(char c) {
	if (c == '.' || c == ',' || c == '!' || c == '?' || c == '|'
		|| c == '\\' || c == '-' || c == '(' || c == ')' || c == '\'')
		return true;
	return false;
}
bool TF_IDFCMP(pair<string, double>& a, pair<string, double>& b) {
	return a.second > b.second;
}