#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
using namespace std;
#define MAX 100
#define INF 0x3f3f3f3f3f3f3f3f

struct Node {//���Ľڵ�
	float weight;//Ȩֵ
	char ch;//�ַ�����Ҷ�ӽڵ�ֵΪ'\0'
	Node* lchild;
	Node* rchild;
	Node* parent;
};

struct Code {
	char ch;
	vector<int>bin;//�����ַ��Ķ����Ʊ���
};

vector<Node*> forest,forest1;//ɭ��,�������ͷָ��,1Ϊ���ݵ�ԭʼɭ��

void Select(Node** s1, Node** s2)
{
	int a = 0, b = 0;//��¼��С������Ȩֵ��λ��
	for (int i = 0; i < forest.size(); i++) 
		if (forest[i]->weight < forest[a]->weight) a = i;
	if (a == 0) b = 1;//��ֹab��ͬ
	for (int i = 0; i < forest.size(); i++)
	{
		if (i == a) continue;
		if (forest[i]->weight < forest[b]->weight) b = i;
	}
	//cout << forest[a]->weight << " " << forest[b]->weight << endl;
	*s1 = forest[a];
	*s2 = forest[b];
	return;
}

Node* HuffmanCreat(string s, float *w,int n)
{//����շ�����
	//1 ������ʼ��������vector
	forest.clear();
	for (int i = 1; i <= n; i++)
	{
		Node* tree = new(Node);
		tree->weight = w[i];
		tree->ch = s[i - 1];
		tree->lchild = NULL;
		tree->rchild = NULL;
		tree->parent = NULL;
		forest.push_back(tree);
	}
	//2 ����ԭʼɭ�֣�����֮�����
	forest1.clear();
	for (int i = 0; i < forest.size(); i++) forest1.push_back(forest[i]);
	//3 ������
	while (forest.size() != 1)//ֱ��ɭ����ֻ��һ����
	{
		//cout << forest.size() << endl;
		Node* s1=NULL, *s2=NULL, *New_Tree;
		Select(&s1, &s2);//��ɭ����ѡ������Ȩֵ��С��������Ϊs1,s2

		//ȷ���������ڵ�Ĺ�ϵ���ϳ�һ������New_Tree
		New_Tree = new(Node);
		New_Tree->ch = '\0';
		New_Tree->weight = s1->weight + s2->weight;
		New_Tree->lchild = s1;
		New_Tree->rchild = s2;
		New_Tree->parent = NULL;
		s1->parent = New_Tree;
		s2->parent = New_Tree;
		//��ɭ����ɾ����������������
		forest.push_back(New_Tree);
		for (int i = 0; i < forest.size(); i++)
		{
			if (forest[i] == s1 || forest[i] == s2) forest.erase(forest.begin() + i), i--;
		}
	}
	return forest[0];//����ɭ���е�Ψһһ���������շ�����
}

void HuffmanCoding(Node* HFm,Code* HC,int n,int* deep)//���ɺշ����������HC[]
{//forest1ɭ���е�ָ��ָ��շ�������Ҷ�ӽڵ�
	int de;
	for (int i = 0; i < forest1.size(); i++)
	{//��ջ���� 
		stack<bool>st;
		de = 1;//ÿ��Ҷ�ӽڵ�����
		Node* p = forest1[i];
		//HC[i] = new(Code);
		HC[i].ch = p->ch;
		//��Ҷ�ӽڵ㵽�����
		while (p->parent)//�Ǹ����
		{//��0��1
			if (p->parent->lchild == p) st.push(0);
			else st.push(1);
			p = p->parent;
			de++;
		}
		while (!st.empty())//��ջ�еĶ����Ʊ������HC[i]
		{
			HC[i].bin.push_back(st.top());
			st.pop();
		}
		*deep = max(*deep, de);//���ֵ���������
	}
	FILE * fid = fopen("Coding rule.txt", "w");
	for (int i = 0; i < n; i++)
	{
		fprintf(fid, "%c\t", HC[i].ch); cout << HC[i].ch << "\t";
		for (int j = 0; j < HC[i].bin.size(); j++) fprintf(fid, "%d", HC[i].bin[j]), cout << HC[i].bin[j];
		fprintf(fid, "\n");
		cout << endl;
	}
	fclose(fid);
	cout << "���������������д��Coding rule��" << endl;
}

void CheckCoding(Code* HC,int n)//�Ե��ı���
{
	vector<char> a;
	//�ļ���
	FILE * p = fopen("ToBeTra.txt", "r");
	if(p==NULL) {
		cout<<"���ļ�ʧ�ܣ�����Ŀ¼�´���ToBeTra.txt����д����Ҫ������ļ�"<<endl;
		return ;
	}
	while (!feof(p)) a.push_back(fgetc(p));
	//cout << "��������Ҫ����ĵ��ģ�";
	//getchar();
	//getline(cin, a);
	vector<int>bin;//���ڴ洢���Ķ����Ʊ���
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (HC[j].ch == a[i]) {
				for (int k = 0; k < HC[j].bin.size(); k++) bin.push_back(HC[j].bin[k]);//�����ַ��ı������bin��
				break;
			}
		}
	}
	cout << "���ı���Ϊ��";
	for (int i = 0; i < bin.size(); i++) 
	{
		if(i%40==0) cout << endl;
		cout << bin[i];
	}
	cout << endl;
	FILE * fid = fopen("CodeFil.txt", "w");
	for (int i = 0; i < bin.size(); i++)
		fprintf(fid, "%d", bin[i]);
	cout << "���ı�����д��CodeFil.txt��" << endl;
	fclose(fid);
}

void HuffmanTranslate(Code* HC,int n)//����������
{
	vector<int>bin;//�洢���Ķ����Ʊ���
	bin.clear();
	/*cout << "��������ƴ��룺" << endl;
	string t;
	cin >> t;
	for (int i = 0; i < t.size(); i++) {
		if(t[i]=='0') bin.push_back(0);//����string,�ٵ���bin��
		else bin.push_back(1);
	}*/
	//�ļ���
	FILE * p = fopen("CodeFile.txt", "r");
	if(p==NULL) {
		cout<<"���ļ�ʧ�ܣ�����Ŀ¼�´���CodeFile.txt����д����Ҫ������ļ�"<<endl;
		return ;
	}
	while (!feof(p)) {
		char y = fgetc(p);
		if(y=='0') bin.push_back(0);
		else bin.push_back(1);
	}
	//bin�ж����һλ������'\0'
	//for (int i = 0; i < bin.size(); i++) cout << bin[i];

	vector<int>T;//��ʱ
	vector<char> C;//����������
	for (int i = 0; i < bin.size()-1; i++)
	{
		T.push_back(bin[i]);
		//�ж�T��HC���Ƿ���ƥ��
		for (int j = 0; j < n; j++){
			if (HC[j].bin == T) {
				C.push_back(HC[j].ch);
				T.clear();
				break;
			}
		}
	}
	//cout << T.size() << endl;
	if (!T.empty()) cout << "ERROR" << endl;
	else {
		FILE * fid1 = fopen("Textfile.txt", "w");
		for (int i = 0; i < C.size(); i++) cout << C[i], fprintf(fid1, "%c", C[i]);
		cout << endl;
		cout << "������д��Textfile.txt��" << endl;
		fclose(fid1);
	}
}

void Treeprinting(Node* HFm,int deep)//ӡ��������
{//�Ȳ������,ÿ��һ�㣬�������,��ӡ�����ܿ��Ϊ 2^deep
	FILE * fid3 = fopen("TreePrint.txt", "w");//�ļ���
	queue<Node*>Q;
	Q.push(HFm);
	int m = (1 << deep);//�ܿ��
	for (int i = 1; i <= m; i++) cout << "*", fprintf(fid3, "*"); 
	cout << endl; fprintf(fid3, "\n");
	for (int h = 1; h <= deep; h++)//hΪ��ǰ���
	{
		for (int i = 1; i <= (1 << (h - 1)); i++)
		{
			Node* p=Q.front();
			Q.pop();
			if (p) {
				if (p->ch != '\0') cout <<"("<< p->ch<<","<<p->weight<<")", fprintf(fid3, "%c", p->ch);
				else cout<<"[" << p->lchild->weight<<"+"<<p->rchild->weight<<"]", fprintf(fid3, "0");
			}
			else cout << " ", fprintf(fid3, " ");
			//��h�м��Ϊ2^(deep-h+1)-1
			for (int j = 1; j < (1 << (deep + 1 - h)); j++) cout << " ", fprintf(fid3, " ");
			//������һ�еĽڵ�
			if (p){
				Q.push(p->lchild);
				Q.push(p->rchild);
			}
			else {
				Q.push(NULL);
				Q.push(NULL);
			}
		}
		cout << endl; fprintf(fid3, "\n");
	}
	for (int i = 1; i <= m; i++) cout << "*", fprintf(fid3, "*");
	cout << endl; fprintf(fid3, "\n");
	fclose(fid3);
	cout << "�ַ���ʽ�Ĺ���������д���ļ�TreePrint.txt��" << endl;
}

void Delete(Node* p)//�ͷŹ�������
{
	if (p) {
		Delete(p->lchild);
		Delete(p->rchild);
		delete(p);
	}
}

int main()
{
	printf("---------------------------------------------\n");
	printf("       �շ������������        \n");
	printf("        1.��ʼ��               \n");
	printf("        2.�����Ӧ����������   \n");
	printf("        3.���кշ�������       \n");
	printf("        4.���кշ�������       \n");
	printf("        5.ӡ��������           \n");
	printf("        6.�˳��շ�������       \n");
	printf(" ---------------------------------------------\n");
	int deep = 0;//�������������
	int n;//�ַ���
	float w[MAX];//Ȩֵ
	string s;//�ַ���
	Node* HFm=NULL;
	Code* HC=NULL;
	int opt;
	int f=0;
	while (1)
	{
		cout<<"�������Ӧ��������ţ�"; 
		cin >> opt;
		switch (opt)
		{
		case 1://��ʼ�� 
			if(HFm) Delete(HFm);
			if(HC) delete[] HC;
			HFm = NULL;
			HC = NULL;
			cout<<"��ʼ���ɹ�"<<endl; 
			break;
		case 2://�����Ӧ����������
			if(HFm && HC){
				cout<<"���ȳ�ʼ��"<<endl;
				break; 
			}
			while (1)
			{ 
				cout << "�������ַ�������" << endl;
				cin>>n;
				if (n > 0) break;
				else cout << "ERROR" << endl;
			}
			getchar();
			while (1)
			{
				map<char,int>mp;
				cout << "�������ַ���" << endl;
				
				getline(cin, s);
				if(s.length()!=n) { cout << "�ַ���������" << endl;continue;}
				//����
				int f=0;
				for(int i=0;i<s.length();i++)
				{
					if(mp[s[i]]==0) mp[s[i]]=1;
					else{
						cout<<"�ַ��ظ��������¶��롣"<<endl;
						f=1;
						break; 
					}
				} 
				if(f==0) break;//����ɹ� 
			}
			cout << "�������ӦȨֵ(Ȩֵ��ո�)��" << endl;
			for (int i = 1; i <= n; i++) cin >> w[i];
			HFm = HuffmanCreat(s, w, n);//�����շ�����
			HC = new Code[n + 2];//���ɺշ����������
			HuffmanCoding(HFm, HC, n, &deep);
			cout<<"���ڿ��Խ��б��������"<<endl; 
			break;
		case 3://3.���кշ������� 
			if(HFm==NULL || HC==NULL){
				cout<<"���������������ݣ��������������Լ��������"<<endl;
				break; 
			}
			CheckCoding(HC, n);
			break;
		case 4://���кշ������� 
			if(HFm==NULL || HC==NULL){
				cout<<"���������������ݣ��������������Լ��������"<<endl;
				break; 
			}
			HuffmanTranslate(HC, n);
			break;
		case 5://ӡ��������
			if(HFm==NULL || HC==NULL){
				cout<<"���������������ݣ��������������Լ��������"<<endl;
				break; 
			}
			Treeprinting(HFm, deep);
			break;
		case 6://�˳��շ�������
			Delete(HFm);
			delete[] HC;
			HFm = NULL;
			HC = NULL;
			f=1;
			cout << "�˳��ɹ���" << endl;
			break;
		default:
			cout<<"ERROR!!!"<<endl;			
		}
		if(f) break;
	}
	system("PAUSE");
	return 0;
}
/*
5
abcde
1 2 7 8 13

8
abcdefgh
0.05 0.29 0.07 0.08 0.14 0.23 0.03 0.11

27
 ABCDEFGHIJKLMNOPQRSTUVWXYZ
186 64 13 22 32 103 21 15 47 57 1 5 32 20 57 63 15 1 48 51 80 23 8 18 1 16 1
THIS PROGRAME IS MY FAVORITE
*/
