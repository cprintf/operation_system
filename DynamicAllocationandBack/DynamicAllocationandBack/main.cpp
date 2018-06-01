/*
**author:Michael
**ʱ�䣺2018��6��1�� 09:46:13
**��Ŀ���洢����̬�������估�����㷨
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

node *head1; //head1Ϊfirst fit�㷨����������ͷ�ڵ��ָ��
node *head2; //head2Ϊbest fit�㷨����������ͷ�ڵ��ָ��
node *back1; //back1��first fit�㷨ָ���ͷ��� node �ṹ��ָ��
node *back2; //back2��best fit�㷨ָ���ͷ��� node �ṹ��ָ��
node *assign; //assign��ָ��������ڴ���� node �ṹ��ָ��
int request;

//�����յ�ַ�ʹ�Сʱ�Ƿ���Ч
int check(int add, int siz, char c)
{
	node *p, *head;
	int check = 1;
	if (add < 0 || siz < 0)  //��ַ�ʹ�СΪ����
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
	while ((p != NULL) && check)   //����Ϊ�գ��������ַ�ʹ�С��Ч
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
			cout << "�����ͷ�����ַ���С�д���\n";
		}
	}
	return check;
}

//��ʼ����������
void init()
{
	//p�ǵ�һ�����õĽڵ�
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

//first fit�㷨���з���
node* assignment1(int num, int req) //numָ�ķ���Ľڵ��(id)
{
	node *before, *after, *ass;
	ass = new node();  //assΪassigment�����Ƿ���Ľڵ�
	before = head1;
	after = head1->next;
	ass->id = num;
	ass->size = req;
	while (after->size < req)   //��ǰָ��Ŀ������ռ䲻��,����
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
		if (after->size == req)  //�������ռ������Ҫ����Ŀռ�
		{
			before->next = after->next;
			ass->adr = after->adr;
		}
		else               //�������ռ������Ҫ����Ŀռ�,�ָ�
		{
			after->size -= req;
			ass->adr = after->adr;
			after->adr += req;
		}
	}
	after->id = ass->id;
	return ass;
}

//first fit Algorithm ����
void acceptment1(int address, int siz, int id)
{
	node *before, *after;
	int insert = 0;   //�����λ
	back1 = new node();
	before = head1;
	after = head1->next;
	back1->adr = address;
	back1->size = siz;
	back1->id = id;
	back1->next = NULL;

	while (!insert && after)  //���뵽������(����ַ��С��С�������)
	{
		//��back1���뵽before��after���м�
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

	if (insert)  //�������ܲ��������ٽ��з���
	{
		//��ǰ�߷����ϲ�,Ȼ��ɾ����back1ָ��ָ��Ŀռ�
		if (back1->adr == before->adr + before->size)
		{
			before->size += back1->size;
			before->next = back1->next;
			delete back1;                //��ͬ��free(back1)
		}
		//�ͺ�߷����ϲ�
		else if (after && back1->adr + back1->size == after->adr)
		{
			back1->size += after->size;
			back1->next = after->next;
			back1->id = after->id;

			delete after;
			after = back1;
		}

		cout << "First Fit Algorithm �����ڴ�ɹ���\n";
	}
	else
	{
		cout << "First Fit Algorithm �����ڴ�ʧ�ܣ�\n";
	}
}

//Best Fit Algorithm���з���
node* assignment2(int num, int req) //numָ�ķ���Ľڵ��(id)
{
	node *before, *after, *ass, *q;
	ass = new node();  //����Ľڵ�
	q = new node();
	before = head2;
	after = head2->next;
	ass->id = num;
	ass->size = req;

	while (after->size < req)   //���пռ䲻����
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
		if (after->size == req)     //��������úͿ�����һ����
		{
			before->next = after->next;
			ass->adr = after->adr;
		}
		else                       //���������ڷ����Ҫ�󣬽����������зָ�
		{
			q = after;       //q��ָ�򱻷ָ��Ŀռ��ָ��
			before->next = after->next;
			ass->adr = q->adr;
			q->size -= req;
			q->adr += req;

			before = head2;
			after = head2->next;
			if (after == NULL)    //˵��������Ϊ��,ֱ�Ӳ��ں���
			{
				before->next = q;
				q->next = NULL;
			}
			else                //��������С��q���ȥ
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
	after->id = ass->id;   //��ֵһ��id(���)
	return ass;
}

//best fit Algorithm���пռ�Ļ���
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

	//���ж���Ϊ��
	if (head2->next == NULL)
	{
		head2->next = back2;
		head2->next->size = back2->size;
	}
	else
	{
		while (after)   //���ж��в�Ϊ�յ�ʱ��
		{
			//��ǰ�߷����ϲ�
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
			//�ͺ�߷����ϲ�
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
			//�������յĿ���뵽���ʵ�λ��(��������С��С����
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
		cout << "�����Ӧ�㷨�����ڴ�ɹ�!\n";
	}
	else
	{
		cout << "�����Ӧ�㷨�����ڴ�ʧ��!\n";
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
		cout << "���������е����:\n";
		cout << "\t���\t��ַ\t��ַ\t��С\n";
		while (p)
		{
			cout << "\t" << p->id << "\t" << p->adr << "\t" << (p->adr + p->size - 1) << "\t" << p->size << "\n";
			p = p->next;
		}
	}
}

void menu()
{
	cout << "��ѡ��̬���������㷨�ͻ����㷨��" << endl;
	cout << "F.First Fit Algorithm" << endl;
	cout << "B.Best Fit Algorithm" << endl;
	cout << "E.exit" << endl;
	cout << "please choose(F��B or E):";
	char ch;
	int m,num = 0,backNum = 0;
	int req; //�û�����ռ�Ĵ�С
	int adr_release, size_release;  //Ҫ�ͷŵ��׵�ַ�ʹ�С
	cin >> ch;
	while (1)
	{

		if (ch == 'F' || ch == 'f')
		{
			cout << "First Fit Algorithmģ�⣺" << endl;
			cout << "1.�����ڴ�  " << "2.�����ڴ�  " << "3.�鿴�ڴ�  " << "4.����" << endl;
			cout << endl;
			cout << "���������ѡ��";
			cout << endl;
			cin >> m;
			switch (m)
			{
			case 1:
				cout << "������������Ҫ����Ŀռ��źʹ�С��";
				cin >> num >> req;

				assign = assignment1(num,req);
				if (assign->adr == -1)
				{
					cout << "�����ڴ�ʧ��!\n";
				}
				else
				{
					cout << "�����ڴ�ɹ�!\n";
				}
				break;
			case 2:
				cout << "������Ҫ�ͷŵĿռ��š��׵�ַ�ʹ�С��";
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
			//break; //���break��������First Fit Algorithm
		}
		else if (ch == 'B' || ch == 'b')
		{
			cout << "Best Fit Algorithmģ�⣺" << endl;
			cout << "1.�����ڴ�  " << "2.�����ڴ�  " << "3.�鿴�ڴ�  " << "4.����" << endl;
			cout << endl;
			cout << "���������ѡ��";
			cin >> m;
			switch (m)
			{
			case 1:
				cout << "������������Ҫ����Ŀռ��źʹ�С��";
				cin >> num >> req;
				
				assign = assignment2(num,req);
				if (assign->adr == -1)
				{
					cout << "�����ڴ�ʧ��!\n";
				}
				else
				{
					cout << "�����ڴ�ɹ�!\n";
				}
				break;
			case 2:
				cout << "������Ҫ�ͷŵĿռ��š��׵�ַ�ʹ�С��";
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
			cout << "�˳�ϵͳT_T" << endl;
			Sleep(2000);     //���뼶��
							 //break����Ҫ�ˣ���Ϊ�����˳�ѭ���ˣ�ֱ���˳�����
			exit(0);		//��ϵͳ����ģ���main�������Ĭ�ϵ��õĺ�����
							//return; �����Լ���ģ�������һ����ã��ͷŶ�ջ 
		}
		else
		{
			cout << "input error!!" << endl;
			cout << "���������룺" << endl;
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
