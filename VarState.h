#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
using namespace std;

long long FileCount;                                      // �ı������ı�����
long long k = 5;
bool runFlag = false;

vector<string> Keywords, KeywordsForShow;                 // �ؼ�������
map<pair<string, string>, int> ThisWordNumInThisFile;     // ͳ�Ƹ��ļ���ÿ���ؼ��ʳ��ֵĴ���
unordered_map<string, int> NumOfWordsInThisFile;          // ��¼���ļ��ĵ�������

set<pair<string, string>> IS_WORD_IN_THIS_FILE;           // �жϸ��ļ��Ƿ��ڸùؼ��ʵ���������
unordered_map<string, vector<string>> WordInTheseFiles;   // ͳ�ƺ��иùؼ��ʵ��ļ��������ں�������

map<pair<string, string>, double> Keyword_TF_InThisFile;  // �ؼ�����ĳһ�ļ���TFֵ
unordered_map<string, double> Keyword_IDF;                // �ؼ��ʵ�IDFֵ
map<pair<string, string>, double> TheFileToKeywordTF_IDF; // �ؼ�����ĳһ�ļ��е�TF-IDFֵ
unordered_map<string, double> FileTF_IDF;                 // ÿ���ļ�����TF_IDFֵ

unordered_set<string> FILESET;                            // ��¼һ������Щ�ļ�
unordered_set<string> FOLDERSET;                          // ��¼�ı�������Щ�ļ���

unordered_set<string> Stopword;                           // ��¼ͣ�ô����ڱȶ�
unordered_set<string> StopwordForShow;                    // ��¼ͣ�ô�������ʾ