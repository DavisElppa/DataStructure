#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<unordered_map>
using namespace std;

//节点类的定义
class node
{
public:
	//node的构造函数
	node(string s, int f)
	{
		this->word = s;
		this->frequency = f;
	}
	string word;
	int frequency;
	node* next;
};

//链表类的定义
class List
{
private:
	node* head;
	int count;
public:
	//构造函数，进行初始化
	List()
	{
		head = nullptr;
		count = 0;
	}

	//析构函数
	~List()
	{
		node* tempptr;
		while (head->next)
		{
			tempptr = head->next;
			delete head;
			head = tempptr;
		}
		delete head;
	}

	//按照字典序，进行插入的函数
	void insert(const string& s)
	{
		if (this->isempty())
		{
			node* ele = new node(s,1);
			head = ele;
			count++;
		}
		else
		{
			if (s < head->word)
			{
				node* ele = new node(s, 1);
				ele->next = head;
				head = ele;
				count++;
			}
			else
			{
				node* p = head;
				while (p->next)
				{
					if (p->word == s)
					{
						p->frequency++;
						break;
					}
					if (s > p->word && s < p->next->word)
					{
						node* ele = new node(s, 1);
						ele->next = p->next;
						p->next = ele;
						count++;
						break;
					}
					else
					{
						p = p->next;
					}
				}
				if (!p->next)//说明已经遍历到了最后一个节点
				{
					node* ele = new node(s, 1);
					p->next = ele;
					count++;
				}
			}
		}
	}

	//交换两指针的数据域
	//可以采用交换指针，但太过于麻烦；因此采用只交换数据
	void swap(node* p, node* q)
	{
		string stemp = p->word;
		p->word = q->word;
		q->word = stemp;
		int ftemp = p->frequency;
		p->frequency = q->frequency;
		q->frequency = ftemp;
	}

	//按照频率从大到小进行排序
	//采用的是插入排序
	void sortByfrequency()
	{
		if (!head) return;
		node* p = head;
		node* q = head->next;
		while (q)
		{
			int temp = q->frequency;
			while (p != q)
			{
				if (p->frequency >= temp)
				{
					p = p->next;
				}
				else
				{
					swap(p, q);
					p = p->next;
				}
			}
			q = q->next;
			p = head;
		}
	}

	//判断是否为空
	bool isempty()
	{
		if(head) return false;
		else return true;
	}

	//返回节点个数
	int size()
	{
		return count;
	}

	//输出所有节点数据
	void print()
	{
		node* p = head;
		while (p)
		{
			cout << p->word << ":" << p->frequency <<"  ";
			p = p->next;
		}
	}

	//将所有节点数据写入result.txt中
	void writeIntotxt()
	{
		ofstream fout("D:\\result.txt",ios::out|ios::app|ios::binary);
		if(head) fout <<"\n" << "字母" << (char)(head->word[0]) << "为首单词及频率" << "\n";
		node* p = head;
		while (p)
		{
			fout << p->word <<":" << p->frequency <<"  ";
			p = p->next;
		}
		fout.close();
	}
};
int main()
{
	//用于统计时间消耗
	clock_t start, end;
	start = clock();

	vector<List>l(26);
	unordered_map<string, int>umap;//用于存储停词表中的单词

	//从停词表中读出数据，并把单词存入哈希表中
	ifstream ifs("C:\\Users\\gaosensen\\Desktop\\stopwords.txt");
	char c;
	string temp = "";
	while (!ifs.eof())
	{
		ifs.get(c);
		if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
		if (c >= 'a' && c <= 'z')
		{
			temp += c;
		}
		else
		{
			if (temp != "")
			{
				umap[temp] = 1;
			}
			temp = "";
		}
	}
	ifs.close();

	//从littleprice.txt中读出数据，并把单词存储到对应的链表上(但是去除停词表上的单词)
	ifstream fin("C:\\Users\\gaosensen\\Desktop\\littleprince.txt");
	while (!fin.eof())
	{
		fin.get(c);
		if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
		if (c >= 'a' && c <= 'z')
		{
			temp += c;
		}
		else
		{
			if (temp != "")
			{
				if(umap.find(temp)==umap.end())//在存储停词表的哈希表中找不到该单词，才能存入链表中
					l[temp[0]-'a'].insert(temp);
			}
			temp = "";
		}
	}
	fin.close();

	//将数据写入到result.txt中
	ofstream fout("D:\\result.txt",ios::out|ios::app|ios::binary);
	fout <<"\n"<< "仅按字典序排列" << "\n";
	fout.close();
	for (int i=0;i<26;i++)
	{
		l[i].writeIntotxt();
	}

	//按照频率排序后，再次把数据写入到result.txt中
	fout.open("D:\\result.txt", ios::out|ios::app|ios::binary);
	fout <<"\n"<<"\n" << "按照频率排序后" << "\n";
	fout.close();
	for (int i = 0; i < 26; i++)
	{
		l[i].sortByfrequency();
		l[i].writeIntotxt();
	}

	//输出消耗时间
	end = clock();
	cout << "耗时为:" << (end - start) / CLK_TCK << endl;
	system("pause");
	return 0;
}