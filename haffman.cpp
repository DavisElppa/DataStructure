#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include<queue>
#include<string>
#include<functional>
#include<stdio.h>
using namespace std;

//将char类型转换为八位01的string
inline string change(char c)
{
	string data;
	for (int i = 0; i < 8; i++)
	{
		//  data+=c&(0x01<<i);
		if ((c >> (i - 1)) & 0x01 == 1)
		{
			data += "1";
		}
		else
		{
			data += "0";
		}
	}
	for (int a = 1; a < 5; a++)
	{
		char x = data[a];
		data[a] = data[8 - a];
		data[8 - a] = x;
	}
	return data;
}

//哈夫曼树节点
class node
{
public:
	char letter;
	int frequency;
	string code;
	node* left;
	node* right;

	node(char c, int f)
	{
		this->frequency = f;
		this->letter = c;
	}
};
struct cmp
{
	bool operator()(node* a, node* b)
	{
		return a->frequency > b->frequency;
	}
};

//哈夫曼树的构造
class haffmanTree
{
public:
	//存储字母对应的编码
	vector<string>ans;

	//构建哈夫曼树，并返回树的根节点
	node* createhaffmanTree(priority_queue<node*,vector<node*>,cmp> q)
	{
		while (!q.empty())
		{
			node* first = q.top();
			q.pop();
			node* second = q.top();
			q.pop();
			node* father=new node(NULL, first->frequency + second->frequency);
			if (first->frequency <= second->frequency)
			{
				father->left = first;
				father->right = second;
			}
			else
			{
				father->left = second;
				father->right = first;
			}
			if (q.empty())
			{
				return father;
				break;
			}
			q.push(father);
		}
	}

	//根据构建的哈夫曼树求出每个字母对应的二进制编码
	void letterCode(priority_queue<node*,vector<node*>,cmp> pq)
	{
		ans.resize(128);
		node* root = this->createhaffmanTree(pq);
		root->code = "";
		queue<node*>q;
		q.push(root);
		while (!q.empty())
		{
			int n = q.size();
			for (int i = 0; i < n; i++)
			{
				node* temp = q.front();
				q.pop();
				if (temp->left)
				{
					temp->left->code = temp->code + "0";
					q.push(temp->left);
					if ((int)temp->left->letter>=0&&(int)temp->left->letter<128)
						ans[(int)temp->left->letter] = temp->left->code;
				}
				if (temp->right)
				{
					temp->right->code = temp->code + "1";
					q.push(temp->right);
					if ((int)temp->right->letter>=0&&(int)temp->right->letter<128)
						ans[(int)temp->right->letter] = temp->right->code;
				}
			}
		}
	}
};

//将inputfile1中的字符读出并统计频率
void letterFrequency(const char*s,vector<int>& farr)
{
	char ch;
	ifstream fin(s);
	while (!fin.eof())
	{
		fin.get(ch);
		if (int(ch) >= 0 && int(ch) <= 127)
			farr[int(ch)]++;
	}
	fin.close();
}

void tangFrequency(const char*s, vector<int>& farr)
{
	char ch;
	int frequency=0;
	char line[100];
	ifstream fin(s);
	while (!fin.eof())
	{
		fin.getline(line, 100);
		if (line[0] >= '0' && line[0] <= '9' && line[1] >= '0' && line[1] <= '9')
		{
			int cnt = (line[0] - '0') * 10 + line[1] - '0';
			frequency = line[3] - '0';
			for (int i = 4; i < 100 && line[i] != ' '; i++)
			{
				frequency = frequency * 10 + (line[i] - '0');
			}
			farr[cnt] = frequency;
			frequency = 0;
		}
		else
		{
			ch = line[0];
			frequency = line[2] - '0';
			for (int i = 3; i < 100 && line[i] != ' '; i++)
			{
				frequency = frequency * 10 + (line[i] - '0');
			}
			farr[(int)ch] = frequency;
			frequency = 0;
		}
	}
}

//可能是下面循环创造对象的原因，使用指针后就解决了
//也可能是上面的那个错误，把root->right->letter错写成了root->right->right,可能root->right就已经为空，因此出错
//由Inputfile1中的数据，构造出哈夫曼树
//并把字母对应的频率和编码存入outputfile1.txt中
node* createTree(vector<int>& farr, haffmanTree& Haffman)
{
	priority_queue<node*, vector<node*>, cmp>q;
	for (int i = 0; i < 128; i++)
	{
		if (farr[i] != 0)
		{
			node* element = new node((char)(i), farr[i]);
			q.push(element);
		}
	}
	node* head = Haffman.createhaffmanTree(q);
	Haffman.letterCode(q);

	ofstream fout3("C:\\Users\\gaosensen\\Desktop\\test\\outputfile1.txt", ios::out);
	for (int i = 0; i < 128; i++)
	{
		if (farr[i] != 0)
		{
			if(i<32)
				fout3 <<i<< " " << farr[i] << " " << Haffman.ans[i] << endl;
			else
			{
				fout3<<(char)i<<" "<<farr[i]<<" "<< Haffman.ans[i] << endl;
			}
		}
	}

	return head;
}

//对inputfile2进行编码，并把结果存入outputfile.dat
void code(const char*sout,haffmanTree& Haffman,const char*sin)
{
	char ch;
	int count = 0;
	char cnt = NULL;
	int cur = 0;
	ofstream fout1(sout, ios::out | ios::app | ios::binary);
	ifstream fin1(sin);

	while (!fin1.eof())
	{
		fin1.get(ch);
		
		if (int(ch) >= 0 && int(ch) < 128)
		{
			for (char x : Haffman.ans[int(ch)])
			{
				count++;
				cur = cur * 2 + (x - '0');
				if (count == 8)
				{
					cnt = (char)cur;
					fout1.write((char*)&cnt, sizeof(char));
					count = 0;
					cur = 0;
				}
			}
		}
	}

	cnt = (char)cur;
	fout1.write((char*)&cnt, sizeof(char));
	int lastEndPos = count == 0 ? 8 : count;
	cnt = (char)lastEndPos;
	fout1.write((char*)&cnt, sizeof(char));
	fout1.close();
	fin1.close();
}

//对outputfile.dat进行译码，并把结果存入outputfile2.txt
void decode(const char* sout,node*head,const char* sin)
{
	node* temp = head;
	int count = 0;
	char ch;
	int lastEndPos;
	string data = "";
	ifstream fin2( sout,ios::binary);//一定要以二进制的形式打开，否则会崩溃
	ofstream fout2(sin, ios::out | ios::app);
	fin2.seekg(-1, ios::end);//把最后一个字符读出来
	fin2.read((char*)&ch, sizeof(ch));
	lastEndPos = int(ch);
	fin2.seekg(-3, ios::end);//倒数第二个字符
	streampos ensPosition = fin2.tellg();//标记最后一位的位置
	fin2.seekg(ios::beg);

	while (fin2.tellg() != ensPosition)
	{
		fin2.read((char*)&ch, sizeof(ch));
		data = change(ch);
		while (count < 8)
		{
			if (data[count] == '0')
				temp = temp->left;
			else
				temp = temp->right;
			if (temp->letter)
			{
				fout2 << temp->letter;
				temp = head;
			}
			count++;
		}
		count = 0;
	}

	fin2.read((char*)&ch, sizeof(char));
	data = change(ch);
	for (int i = 8 - lastEndPos; i < 8; i++)
	{
		if (data[i] == '0')
			temp = temp->left;
		else
			temp = temp->right;
		if (temp->letter)
		{
			fout2 << temp->letter;
			temp = head;
		}
	}
	fin2.close();
	fout2.close();
}

int main()
{
	vector<int>farr(128, 0);
	letterFrequency("c:\\users\\gaosensen\\desktop\\test\\inputfile1.txt",farr);
	haffmanTree haffman;
	node*head=createTree(farr, haffman);
	code("C:\\Users\\gaosensen\\Desktop\\test\\outputfile.dat",haffman, "C:\\Users\\gaosensen\\Desktop\\test\\inputfile2.txt");
	decode("C:\\Users\\gaosensen\\Desktop\\test\\outputfile.dat",head, "C:\\Users\\gaosensen\\Desktop\\test\\outputfile2.txt");

	//解tang的码
	vector<int>farr1(128, 0);
	tangFrequency("C:\\Users\\gaosensen\\Desktop\\test\\tang.txt", farr1);
	haffmanTree haffman1;
	node* head1 = createTree(farr1, haffman1);
	decode("C:\\Users\\gaosensen\\Desktop\\test\\tang.dat", head1, "C:\\Users\\gaosensen\\Desktop\\test\\tangoutput.txt");
	
	return 0;
}