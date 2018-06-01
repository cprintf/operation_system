/*
**author:Michael
**时间：2018年6月1日 09:46:13
**题目：存储管理动态分区分配及回收算法
*/

#include<iostream>
#include<Windows.h>
#define Max_Size 32767
using std::cin;
using std::cout;
using std::endl;

struct node
{
	int id;
	int adr;
	int size;
	node *next;

};

node *head1; //head1为first fit算法空闲区队列头节点的指针
node *head2; //head2为best fit算法空闲区队列头节点的指针
node *back1; //back1是first fit算法指向释放区 node 结构的指针
node *back2; //back2是best fit算法指向释放区 node 结构的指针
node *assign; //assign是指向申请的内存分区 node 结构的指针
int request;

//检查回收地址和大小时是否有效
int check(int add, int siz, char c)
{
	node *p, *head;
	int check = 1;
	if (add < 0 || siz < 0)  //地址和大小为负数
	{
		check = 0;
	}
	if (c == 'f' || c == 'F')
	{
		head = head1;
	}
	else
	{
		head = head2;
	}
	p = head->next;    
	while ((p != NULL) && check)   //链表不为空，且输入地址和大小有效
	{
		if (((add < p->adr) && (add + siz > p->adr)) || ((add >= p->adr) && (add < p->adr + p->size)))
		{
			check = 0;
		}
		else
		{
			p = p->next;
		}

		if (check == 0)
		{
			cout << "输入释放区地址或大小有错误！\n";
		}
	}
	return check;
}

//初始化操作函数
void init()
{
	//p是第一个有用的节点
	node *p;
	head1 = new node();
	head2 = new node();
	p = (node*)malloc(sizeof(node));

	head1->next = p;
	head2->next = p;
	p->size = Max_Size;
	p->adr = 0;
	p->next = NULL;
}

//first fit算法进行分配
node* assignment1(int num, int req) //num指的分配的节点号(id)
{
	node *before, *after, *ass;
	ass = new node();  //ass为assigment，即是分配的节点
	before = head1;
	after = head1->next;
	ass->id = num;
	ass->size = req;
	while (after->size < req)   //当前指向的空闲区空间不足,遍历
	{
		before = before->next;
		after = after->next;
	}
	if (after == NULL)
	{
		ass->adr = -1;
	}
	else
	{
		if (after->size == req)  //空闲区空间等于需要分配的空间
		{
			before->next = after->next;
			ass->adr = after->adr;
		}
		else               //空闲区空间大于需要分配的空间,分割
		{
			after->size -= req;
			ass->adr = after->adr;
			after->adr += req;
		}
	}
	after->id = ass->id;
	return ass;
}

//first fit Algorithm 回收
void acceptment1(int address, int siz, int id)
{
	node *before, *after;
	int insert = 0;   //做标记位
	back1 = new node();
	before = head1;
	after = head1->next;
	back1->adr = address;
	back1->size = siz;
	back1->id = id;
	back1->next = NULL;

	while (!insert && after)  //插入到空闲区(按首址大小从小到大插入)
	{
		//将back1插入到before和after的中间
		if ((after == NULL) || ((back1->adr <= after->adr) && (back1->adr >= before->adr)))
		{
			before->next = back1;
			back1->next = after;
			insert = 1;
		}
		else
		{
			before = before->next;
			after = after->next;
		}
	}

	if (insert)  //对上面能插入的情况再进行分类
	{
		//和前边分区合并,然后删除掉back1指针指向的空间
		if (back1->adr == before->adr + before->size)
		{
			before->size += back1->size;
			before->next = back1->next;
			delete back1;                //等同于free(back1)
		}
		//和后边分区合并
		else if (after && back1->adr + back1->size == after->adr)
		{
			back1->size += after->size;
			back1->next = after->next;
			back1->id = after->id;

			delete after;
			after = back1;
		}

		cout << "First Fit Algorithm 回收内存成功！\n";
	}
	else
	{
		cout << "First Fit Algorithm 回收内存失败！\n";
	}
}

//Best Fit Algorithm进行分配
node* assignment2(int num, int req) //num指的分配的节点号(id)
{
	node *before, *after, *ass, *q;
	ass = new node();  //分配的节点
	q = new node();
	before = head2;
	after = head2->next;
	ass->id = num;
	ass->size = req;

	while (after->size < req)   //空闲空间不够大
	{
		before = before->next;
		after = after->next;
	}
	if (after == NULL)
	{
		ass->adr = -1;
	}
	else
	{
		if (after->size == req)     //需求的正好和空闲区一样大
		{
			before->next = after->next;
			ass->adr = after->adr;
		}
		else                       //空闲区大于分配的要求，将空闲区进行分割
		{
			q = after;       //q是指向被分割后的空间的指针
			before->next = after->next;
			ass->adr = q->adr;
			q->size -= req;
			q->adr += req;

			before = head2;
			after = head2->next;
			if (after == NULL)    //说明该链表为空,直接插在后面
			{
				before->next = q;
				q->next = NULL;
			}
			else                //按分区大小将q插回去
			{
				while ((after->size) < (q->size))  
				{
					before = before->next;
					after = after->next;
				}
				before->next = q;
				q->next = after;
			}
		}
	}
	after->id = ass->id;   //赋值一下id(编号)
	return ass;
}

//best fit Algorithm进行空间的回收
void acceptment2(int address, int siz, int id)
{
	node *before, *after;
	int insert = 0;
	back2 = new node();
	before = head2;
	after = head2->next;

	back2->adr = address;
	back2->size = siz;
	back2->id = id;
	back2->next = NULL;

	//空闲队列为空
	if (head2->next == NULL)
	{
		head2->next = back2;
		head2->next->size = back2->size;
	}
	else
	{
		while (after)   //空闲队列不为空的时候
		{
			//和前边分区合并
			if (back2->adr == after->adr + after->size)
			{
				before->next = after->next;
				after->size += back2->size;
				back2 = after;
			}
			else
			{
				before = before->next;
				after = after->next;
			}
		}

		before = head2;
		after = head2->next;
		while (after)
		{
			//和后边分区合并
			if (after->adr == back2->adr + back2->size)
			{
				before->next = after->next;
				back2->size += after->size;
			}
			else
			{
				before = before->next;
				after = after->next;
			}
		}

		before = head2;
		after = head2->next;
		while (!insert)
		{
			//将被回收的块插入到合适的位置(按分区大小从小到大）
			if (after == NULL || ((after->size > back2->size) && (before->size < back2->size)))
			{
				before->next = back2;
				back2->next = after;
				insert = 1;
				break;
			}
			else
			{
				before = before->next;
				after = after->next;
			}
		}
	}
	if (insert)
	{
		cout << "最佳适应算法回收内存成功!\n";
	}
	else
	{
		cout << "最佳适应算法回收内存失败!\n";
	}
}

void print(char choice)
{
	node *p;
	if (choice == 'f' || choice == 'F')
	{
		p = head1->next;
	}
	else
	{
		p = head2->next;
	}

	if (p)
	{
		cout << "空闲区队列的情况:\n";
		cout << "\t编号\t首址\t终址\t大小\n";
		while (p)
		{
			cout << "\t" << p->id << "\t" << p->adr << "\t" << (p->adr + p->size - 1) << "\t" << p->size << "\n";
			p = p->next;
		}
	}
}

void menu()
{
	cout << "请选择动态分区分配算法和回收算法：" << endl;
	cout << "F.First Fit Algorithm" << endl;
	cout << "B.Best Fit Algorithm" << endl;
	cout << "E.exit" << endl;
	cout << "please choose(F、B or E):";
	char ch;
	int m,num = 0,backNum = 0;
	int req; //用户申请空间的大小
	int adr_release, size_release;  //要释放的首地址和大小
	cin >> ch;
	while (1)
	{

		if (ch == 'F' || ch == 'f')
		{
			cout << "First Fit Algorithm模拟：" << endl;
			cout << "1.分配内存  " << "2.回收内存  " << "3.查看内存  " << "4.返回" << endl;
			cout << endl;
			cout << "请输入你的选择：";
			cout << endl;
			cin >> m;
			switch (m)
			{
			case 1:
				cout << "请输入你所想要分配的空间编号和大小：";
				cin >> num >> req;

				assign = assignment1(num,req);
				if (assign->adr == -1)
				{
					cout << "分配内存失败!\n";
				}
				else
				{
					cout << "分配内存成功!\n";
				}
				break;
			case 2:
				cout << "请输入要释放的空间编号、首地址和大小：";
				cin >> backNum >> adr_release >> size_release;
				if (check(adr_release, size_release,ch))
				{
					acceptment1(adr_release, size_release,backNum);
				}
				break;
			case 3:
				print(ch);
				break;
			case 4:
				menu();
				break;

			}
			//break; //这个break用于跳出First Fit Algorithm
		}
		else if (ch == 'B' || ch == 'b')
		{
			cout << "Best Fit Algorithm模拟：" << endl;
			cout << "1.分配内存  " << "2.回收内存  " << "3.查看内存  " << "4.返回" << endl;
			cout << endl;
			cout << "请输入你的选择：";
			cin >> m;
			switch (m)
			{
			case 1:
				cout << "请输入你所想要分配的空间编号和大小：";
				cin >> num >> req;
				
				assign = assignment2(num,req);
				if (assign->adr == -1)
				{
					cout << "分配内存失败!\n";
				}
				else
				{
					cout << "分配内存成功!\n";
				}
				break;
			case 2:
				cout << "请输入要释放的空间编号、首地址和大小：";
				cin >> backNum >> adr_release >> size_release;
				if (check(adr_release, size_release,ch))
				{
					acceptment2(adr_release, size_release,backNum);
				}
				break;
			case 3:print(ch); break;
			case 4:menu(); break;
			default:
				break;
			}
		}
		else if (ch == 'E' || ch == 'e')
		{
			cout << "退出系统T_T" << endl;
			Sleep(2000);     //毫秒级别
							 //break不需要了，因为不用退出循环了，直接退出程序
			exit(0);		//是系统级别的，是main函数最后默认调用的函数体
							//return; 是语言级别的，返回上一层调用，释放堆栈 
		}
		else
		{
			cout << "input error!!" << endl;
			cout << "请重新输入：" << endl;
			cin >> ch;
		}
	}

}

int main()
{
	init();
	menu();
	return 0;
}
