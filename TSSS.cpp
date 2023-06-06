#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <fstream>
#include <io.h>
#include <thread>
#include <windows.h>
#include <shellapi.h>
#include <direct.h>
#include "FunctionState.h"
#include "VarState.h"
using namespace std;

// ѡ��
void Choice() {
	printf("��ϵͳ���ܣ�\n");
	printf("1������ؼ��ʣ����ļ��н��м���\n");
	printf("2���鿴�ı�\n");
	printf("3��������ı�\n");
	printf("4��ɾ���ı�\n");
	printf("5�����˵�\n");
	printf("9������ͣ�ô�\n");
	printf("0���˳�ϵͳ\n\n");
	printf("��ѡ��");
	string choice;
	bool flag;
	do {
		flag = true;
		cin >> choice;
		getchar();
		if (!check(choice)) {
			flag = false;
			printf("ѡ��Ϸ������������룺");
		}
	} while (!flag);
	switch (choice[0] - '0') {
	case 1: {
		CalculateTF_IDF();
		break;
	} case 2: {
		ShowFile();
		break;
	} case 3: {
		ImportFile();
		break;
	} case 4: {
		DeleteFile();
		break;
	} case 5: {
		system("cls");
		Choice();
		break;
	} case 9: {
		StopWords();
		break;
	} case 0: {
		printf("\n��ӭ�ٴ�ʹ�ã�\n");
		exit(0);
	}
	}
}

// Ԥ����
void Pretreat() {
	runFlag = false;
	readStopword();                            // ��ȡͣ�ô�
	ifstream InFile("PreTreatFile.txt");
	string FolderName;
	while (getline(InFile, FolderName)) {      // �����ı����е�ÿ���ļ���
		FOLDERSET.insert(FolderName);
		struct _finddata_t FileInfo;
		string cur = FolderName + "\\\\*.txt";
		long long handle;
		if ((handle = _findfirst(cur.c_str(), &FileInfo)) == -1L) {
			printf(" %s ��û���ı��ļ��������ļ���·���Ƿ���ȷ��\n", FolderName.c_str());
		}
		else {
			FolderName += "\\\\";
			handle = _findfirst(cur.c_str(), &FileInfo);  // �����ļ����е�ÿ���ı��ļ�
			do {
				string FileName = FolderName + FileInfo.name;
				Index(FileName);                          // ��������
			} while (!(_findnext(handle, &FileInfo)));
			_findclose(handle);
		}
	}
	runFlag = true;                                       // Ԥ�������
}
bool checkStopWord(string str) {
	for (auto it : Stopword) {
		if (upper(it) == upper(str)) return true;
	}
	return false;
}
void readStopword() {
	ifstream InFile("StopWordList.txt");
	string str;
	while (getline(InFile, str)) {
		string word;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == ' ' || i == str.size() - 1) {
				if (str[i] != ' ') word += str[i];
				Stopword.insert(upper(word));
				StopwordForShow.insert(word);
				word = "";
			}
			else word += str[i];
		}
	}
}
// ��������
void BuildWordIndex(string str, int& allwordsNum, string FileName) {
	int strlen = str.length();
	string word;
	for (int i = 0; i < strlen; i++) {
		if (str[i] == ' ' || i == strlen - 1 || wordEnd(str[i])) {
			if (i == strlen - 1 && !wordEnd(str[i]) && str[i] != ' ') {
				if (str[i] >= 97) word += str[i] - 32;
				else word += str[i];
			}
			if (word != "") {
				allwordsNum++;
				if (Stopword.count(word)) {
					word = "";
					continue;
				}
				if (!IS_WORD_IN_THIS_FILE.count({ word, FileName })) { // ����õ��ʵ�������û�и��ļ�
					IS_WORD_IN_THIS_FILE.insert({ word, FileName });
					WordInTheseFiles[word].push_back(FileName); // ��������
				}
				ThisWordNumInThisFile[{FileName, word}]++;
			}
			word = "";
		}
		else {
			if (str[i] >= 97) word += str[i] - 32;
			else word += str[i];
		}
	}
}
void Index(string FileName) {
	ifstream inFile;
	inFile.open(FileName);
	string str;
	int allwordsNum = 0;
	while (getline(inFile, str)) {
		BuildWordIndex(str, allwordsNum, FileName);
	}
	NumOfWordsInThisFile[FileName] = allwordsNum;
	inFile.close();
	FileCount++;
	FILESET.insert(FileName);
}

// ����ؼ��ʵ�����ͼ���
void showInputMesg() {
	printf("����������ؼ��ʣ����ո��������м��������룬δ���������򷵻����˵���\n");
	bool flag;
	do {
		string str, word;
		flag = false;
		getline(cin, str);
		for (int i = 0; i < str.size(); i++) {
			if (str[i] < 0 || str[i] > 127 || wordEnd(str[i])) {
				flag = true;
				printf("�������ݿ��ܺ������Ļ��㣬���������롣\n");
				break;
			}
			if (str[i] != ' ') word += str[i];
			if (str[i] == ' ' || i == str.size() - 1) {
				if (Stopword.count(upper(word))) {
					printf("�������ݺ���ͣ�ôʣ����������롣\n");
					flag = true;
					break;
				}
				if (word != "") {
					KeywordsForShow.push_back(word);
					Keywords.push_back(upper(word));
					word = "";
				}
			}
		}
	} while (flag);
	if (!Keywords.size()) {
		system("cls");
		printf("δ����ؼ��ʣ����ڷ������˵�...\n\n");
		Choice();
	}
}
void CalculateTF_IDF() {
	clear();
	system("cls");
	showInputMesg();         // ����ؼ���

	if (!runFlag) printf("���״����У��ٶȿ��ܽ��������Ե�...��\n");
	if (!runFlag) {
		printf("���ڼ�����...\n");
		while (!runFlag);
	}
	else {
		printf("���ڼ�����...\n");
	}
	for (int i = 0; i < Keywords.size(); i++) {  // ����ؼ���IDF
		Keyword_IDF[Keywords[i]] = log10(1.0 * FileCount / (WordInTheseFiles[Keywords[i]].size()));
	}
	for (int i = 0; i < Keywords.size(); i++) { // ����ÿ���ؼ��ʵĵ����б�
		for (auto it : WordInTheseFiles[Keywords[i]]) {
			Keyword_TF_InThisFile[{it, Keywords[i]}] = 1.0 * ThisWordNumInThisFile[{it, Keywords[i]}] / NumOfWordsInThisFile[it];
			TheFileToKeywordTF_IDF[{it, Keywords[i]}] = Keyword_TF_InThisFile[{it, Keywords[i]}] * Keyword_IDF[Keywords[i]];
			FileTF_IDF[it] += TheFileToKeywordTF_IDF[{it, Keywords[i]}];
		}
	}
	Output();                                   // ����������
}
// ���������Ϣ
void Output() {
	vector<pair<string, double>> FileTF_IDFVector(FileTF_IDF.begin(), FileTF_IDF.end());
	if (!FileTF_IDFVector.size()) {
		printf("û�������Ϲؼ���ƥ����ı��ļ���\n");
		Choice();
		return;
	}
	printf("\n����������£�\n");
	sort(FileTF_IDFVector.begin(), FileTF_IDFVector.end(), TF_IDFCMP);
	printf("Top-------------Name--------------Frequency---------TF*IDF-------\n");
	for (int i = 0; i < FileTF_IDFVector.size() && i < k; i++) {
		printf(" %d\t%s\t\t\t   %-8f\n", i + 1, FileTF_IDFVector[i].first.c_str(), FileTF_IDFVector[i].second);
		for (int j = 0; j < Keywords.size(); j++) {
			printf("\t\t%s\t\t    %d\t\t   %-8f\n",
				KeywordsForShow[j].c_str(), ThisWordNumInThisFile[{FileTF_IDFVector[i].first.c_str(), Keywords[j]}],
				TheFileToKeywordTF_IDF[{FileTF_IDFVector[i].first, Keywords[j]}]);
		}
		printf("-----------------------------------------------------------------\n");
	}
	printf("\n");
	Choice();
}
// ��������
void clear() {
	Keywords.clear();
	KeywordsForShow.clear();
	TheFileToKeywordTF_IDF.clear();
	FileTF_IDF.clear();
}

// �����ڲ鿴�ı���Ϣ
void ShowFile() {
	printf("������Ҫ�鿴���ļ�·�����磺Data\\\\123.txt��-1��ʾ���أ���\n");
	string filename;
	cin >> filename;
	if (filename == "-1") {
		system("cls");
		Choice();
	}
	if (!runFlag) printf("���״����У��ٶȿ��ܽ��������Ե�...��\n");
	if (!runFlag) {
		printf("���ڲ���...\n");
		while (!runFlag);
	}
	bool flag;
	do {
		if (filename == "-1") break;
		flag = true;
		if (!FILESET.count(filename)) {
			flag = false;
			printf("û���ҵ����ļ������������룺");
			cin >> filename;
		}
	} while (!flag);
	if (filename == "-1") {
		system("cls");
		Choice();
	}
	DeleteData(filename);
	string path = "notepad.exe " + filename;
	WinExec(path.c_str(), SW_SHOW);              // ���ü��±����ļ�
	Index(filename);
	printf("\n");
	Choice();
}
// ����ļ�
void ImportFile() {
	system("cls");
	printf("������Ҫ��ӵ��ļ����У�·����-1��ʾ���أ���\n");

	string filename;
	cin >> filename;
	if (filename == "-1") {
		system("cls");
		Choice();
	}
	if (!runFlag) printf("���״����У��ٶȿ��ܽ��������Ե�...��\n");
	if (!runFlag) {
		printf("���ڴ���...\n");
		while (!runFlag);
	}
	bool flag;
	do {
		if (filename == "-1") break;
		flag = true;
		if (FOLDERSET.count(filename) || FILESET.count(filename)) {
			flag = false;
			printf("�ļ��Ѵ��ڣ����������룺");
			cin >> filename;
		}
	} while (!flag);
	if (filename == "-1") {
		system("cls");
		Choice();
	}

	if (filename.substr(filename.length() - 4, 4) == ".txt") { // �ж��Ƿ����ı��ļ�
		ifstream in(filename.c_str());
		if (in.good()) {
			Index(filename);
			printf("����ɹ���\n\n");
		}
		else {
			printf("�ļ������ڣ�����·����\n\n");
		}
	}
	else {                                                      // �����ļ���
		FOLDERSET.insert(filename);
		struct _finddata_t FileInfo;
		string cur = filename + "\\\\*.txt";
		long long handle;
		if ((handle = _findfirst(cur.c_str(), &FileInfo)) == -1L) {
			printf(" %s ��û���ı��ļ��������ļ���·���Ƿ���ȷ��\n\n", filename.c_str());
		}
		else {
			filename += "\\\\";
			handle = _findfirst(cur.c_str(), &FileInfo);        // �����ļ�����ÿ���ı��ļ�
			do {
				string FileName = filename + FileInfo.name;
				Index(FileName);
			} while (!(_findnext(handle, &FileInfo)));
			_findclose(handle);
			ofstream out("PreTreatFile.txt");
			for (auto it : FOLDERSET) {                         // �����ı����е��ļ���
				out << it << endl;
			}
			printf("\n�ļ�����ɹ���\n\n");
		}
	}
	Choice();
}
// ɾ���ļ�
void removeDir(const char* dirPath)
{
	struct _finddata_t fb;
	char  path[250];
	long long  handle = 0;
	strcpy_s(path, dirPath);
	strcat_s(path, "/*");

	handle = _findfirst(path, &fb);
	if (handle != 0) {
		while (0 == _findnext(handle, &fb)) {
			memset(path, 0, sizeof(path));
			strcpy_s(path, dirPath);
			strcat_s(path, "/");
			strcat_s(path, fb.name);
			remove(path);
		}
		_findclose(handle);
	}
}
void DeleteData_ByLine(string str, string FileName) {
	int strlen = str.length();
	string word;
	for (int i = 0; i < strlen; i++) {
		if (str[i] == ' ' || i == strlen - 1 || wordEnd(str[i])) {
			if (i == strlen - 1 && !wordEnd(str[i]) && str[i] != ' ') {
				if (str[i] >= 97) word += str[i] - 32;
				else word += str[i];
			}
			if (word != "") {
				if (IS_WORD_IN_THIS_FILE.count({ word, FileName })) { // ����õ��ʵ��������и��ļ�
					IS_WORD_IN_THIS_FILE.erase({ word, FileName });   // ��ɾ���õ��ʶԸ��ļ�������
					vector<string>::iterator it = find(WordInTheseFiles[word].begin(), WordInTheseFiles[word].end(), FileName);
					if (it != WordInTheseFiles[word].end()) WordInTheseFiles[word].erase(it);
				}
				if (ThisWordNumInThisFile[{FileName, word}]) {
					ThisWordNumInThisFile.erase({ FileName, word });
				}
				if (Keyword_TF_InThisFile[{FileName, word}]) {
					Keyword_TF_InThisFile.erase({ FileName, word });
				}
				if (Keyword_IDF[word]) {
					Keyword_IDF.erase(word);
				}
			}
			word = "";
		}
		else {
			if (str[i] >= 97) word += str[i] - 32;
			else word += str[i];
		}
	}
}
void DeleteData(string FileName) {
	ifstream inFile(FileName);
	string str;
	while (getline(inFile, str)) {
		DeleteData_ByLine(str, FileName);
	}
	NumOfWordsInThisFile.erase(FileName);
	inFile.close();
	FileCount--;
	FILESET.erase(FileName);                // �ı������Ƴ����ļ�
}
void DeleteFile() {
	system("cls");
	printf("������Ҫɾ�����ļ����У�·����-1��ʾ���أ���\n");

	string filename;
	cin >> filename;
	if (filename == "-1") {
		system("cls");
		Choice();
	}
	if (!runFlag) printf("���״����У��ٶȿ��ܽ��������Ե�...��\n");
	if (!runFlag) {
		printf("���ڲ���Ҫɾ�����ļ�...\n");
		while (!runFlag);
	}
	bool flag;
	do {
		if (filename == "-1") break;
		flag = true;
		if (!FILESET.count(filename) && !FOLDERSET.count(filename)) {
			flag = false;
			printf("û���ҵ����ļ������������룺");
			cin >> filename;
		}
	} while (!flag);
	if (filename == "-1") {
		system("cls");
		Choice();
	}
	printf("������ɾ����ʽ��1.��ɾ������  2.ɾ���������ļ�\n");
	int c;
	scanf_s("%d", &c);
	if (filename.substr(filename.length() - 4, 4) == ".txt") {        // �ж��Ƿ����ı��ļ�
		DeleteData(filename);
		FILESET.erase(filename);
		FileCount--;
		if (c == 2) {
			remove((filename).c_str());
			printf("\nɾ���������ļ��ɹ���\n");
		}
		else printf("\nɾ�������ɹ���\n");
	}
	else {
		struct _finddata_t FileInfo;
		string cur = filename + "\\\\*.txt";
		long long handle;
		if ((handle = _findfirst(cur.c_str(), &FileInfo)) == -1L) {
			printf(" %s ��û���ı��ļ��������ļ���·���Ƿ���ȷ��\n\n", filename.c_str());
		}
		else {
			handle = _findfirst(cur.c_str(), &FileInfo);               // �����ļ�����ÿ���ļ�
			do {
				string FileName = filename + "\\\\" + FileInfo.name;
				DeleteData(FileName);                                  // ɾ��ÿ���ļ�������
			} while (!(_findnext(handle, &FileInfo)));
			_findclose(handle);
			FOLDERSET.erase(filename);
			ofstream out("PreTreatFile.txt");
			for (auto it : FOLDERSET) {                                // �����ı���
				out << it << endl;
			}
			if (c == 2) {                                              // �ж��Ƿ���Ӳɾ��
				removeDir(filename.c_str());
				_rmdir(filename.c_str());
				printf("\nɾ���������ļ��гɹ���\n");
			}
			else printf("\nɾ�������ɹ���\n");
		}
	}
	printf("\n");
	Choice();
}

//����ͣ�ô�
void StopWords() {
	system("cls");
	printf("ͣ�ôʣ�\n");
	for (auto it : StopwordForShow) cout << it << ' ';
	printf("\n1�����ͣ�ôʡ�\n");
	printf("2��ɾ��ͣ�ôʡ�\n");
	printf("0�����ز�����������\n");
	printf("��ѡ��");
	bool change = false;
	while (true) {
		string choice;
		bool flag;
		do {
			flag = true;
			cin >> choice;
			if (!check(choice) || choice[0] == '3' || choice[0] == '4' || choice[0] == '5' || choice[0] == '9') {
				flag = false;
				printf("ѡ��Ϸ������������룺");
			}
		} while (!flag);
		switch (choice[0] - '0') {
		case 1: {
			printf("������Ҫ��ӵ�ͣ�ôʣ�");
			string str;
			cin >> str;
			while (Stopword.count(upper(str))) {                // �ȶ��Ѵ��ڵ�ͣ�ô�
				printf("��ͣ�ô��Ѵ��ڣ����������룺");
				cin >> str;
			}
			Stopword.insert(upper(str));
			StopwordForShow.insert(str);
			printf("\n��ӳɹ���\n");
			change = true;
			break;
		} case 2: {
			if (!Stopword.size()) {
				printf("ͣ�ôʱ�Ϊ�գ������ѡ�\n");
				break;
			}
			printf("������Ҫɾ����ͣ�ôʣ�");
			string str;
			cin >> str;
			while (!Stopword.count(upper(str))) {
				printf("δ�ҵ���ͣ�ôʣ����������룺");
				cin >> str;
			}
			Stopword.erase(upper(str));
			StopwordForShow.erase(str);
			printf("\nɾ���ɹ���\n");
			change = true;
			break;
		} case 0: {
			if (change) {
				if (!runFlag) {
					printf("���ڸ��£����Ե�...\n");
					while (!runFlag);
				}
				ofstream InFile("StopWordList.txt");
				for (auto it : StopwordForShow) {             // ����ͣ�ôʱ�
					InFile << it << ' ';
				}
				printf("\n����������ϵͳ�Ը���������\n");
				exit(0);
			}
			else {
				system("cls");
				Choice();
			}
			break;
		}
		}
		printf("ͣ�ôʣ�\n");
		for (auto it : StopwordForShow) cout << it << ' ';
		printf("\n\n");
		printf("�����ѡ��");
	}
}

int main() {
	printf("��ӭʹ�û��ڹؼ��ʵ��ı��������ϵͳ��\n");

	thread treatThread(Pretreat);
	treatThread.detach();

	Choice();

	return 0;
}