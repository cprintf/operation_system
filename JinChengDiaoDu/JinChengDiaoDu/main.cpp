/*
**@author:Lijunjie
**@time:2018-4-30 22:04:55
**��Ŀ������ģ������㷨
**assume:��������ģ����ȼ���
*/

#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#define jinchengshu 5
//char STATE[3] = { 'R','W','F' };

struct PCB
{
	char NAME[10];  //���̱�ʶ��
	int PRIO;  //����������
	int ROUND; //����ÿ����ת��ʱ��Ƭ��
	int CPUTIME; //�����ۼ�ռ��CPU��ʱ��Ƭ��
	int NEEDTIME; //���̵���ɻ���Ҫ��ʱ��Ƭ��
	char STATE; //����״̬
	PCB *next; //��ָ��,ָ��ṹ�����͵�ָ�룬������һ��PCB��ָ��
};


PCB  *run, *ready,*tail,*finish; //*ready,*tail,*finishΪ����ָ�룬*run�ǵ�ǰ����ָ��
int n; //�û�ѡ����㷨

//����δ��ɵ� PCB ��������˳����뵽���������У�   
void INSERT1(PCB *p)
{
	/*ð�����򷽷�
	int prio = 0;
	for (int i = 0; i < jinchengshu - 1; i++)
	{
	for (int j = 0; j < jinchengshu - 1 - i; j++)
	{
	if (pcb[j].PRIO < pcb[j + 1].PRIO)
	{

	}
	}

	}
	*/
	PCB *temp;      //�������ʱָ�����������readyǰ���ֵ���ᶪ��
	PCB *pre;       //��ǰ����λ�õ�ǰһ��λ�õ�ָ��
	pre = temp = ready;  //��tempָ��������еĶ���
	//pre->next = ready;
	if (ready == NULL && tail == NULL)
	{
		ready = tail = p;
		tail->next = NULL;
	}
	else
	{
		while (temp != NULL)
		{
			if (p->PRIO > temp->PRIO)
			{
				break;
				//ready = temp;
				   //�˳�ѭ������Ҫ��-_-
				//return;   //��ֹ���
			}
			pre = temp;  //��tempָ�����(����readyָ�����)
			temp = temp->next;

		}
		if (pre == temp)
		{
			p->next = temp;
			ready = p;
		}
		else
		{
			pre->next = p;
			p->next = temp;
		}
		
	}
	
	//�����껹û���ҵ����Ͳ嵽���
	if (!temp)
	{
		tail->next = p;
		tail = tail->next; //tailָ�����
		tail->next = NULL;

	}
    
}

//����ת���У���ִ����һ��ʱ��Ƭ��λ��Ϊ 2��������δ��ɵĽ��̵�PCB���嵽�������еĶ�β
void INSERT2(PCB *p)
{
	if (ready == NULL && tail == NULL)
	{
		ready = tail = p;
		tail->next = NULL;
	}
	else
	{
		tail->next = p;
		tail = tail->next;
		tail->next = NULL;
	}
}


//�����½��̣���������PCB�����������
void CREATE()
{
	PCB *p;
	ready = NULL;
	tail = NULL;
	run = NULL;
	finish = NULL;
	cout << "������" << jinchengshu << "�����̵Ľ��̺ź���������ʱ�䣺" << endl;

	for (int i = 0; i < jinchengshu; i++)
	{
		//��PCB�ĳ�ʼ��
		p = new PCB;     //����Ŀռ䣬�����ַ����p

		cin >> p->NAME >> p->NEEDTIME;
		p->CPUTIME = 0;
		p->ROUND = 2;
		p->PRIO = 50 - p->NEEDTIME;
		p->STATE = 'W';

		if (n == 1)
		{		
			//���½��밴���ȼ��������������
			INSERT1(p);
		}
		else if (n == 2)
		{
			INSERT2(p);
		}
		//delete p;
	}
}



//���Ⱦ������е����׽���Ͷ������
void FIRSTIN()
{
	run = ready; //����ͷָ��ָ���������ͷָ��
	run->STATE = 'R';
	ready = ready->next; //ָ�����
}

//��ʾÿִ��һ�κ����н��̵�״̬���й���Ϣ
void PRINT(PCB *p)
{
	if (n == 1)
	{
		cout << "name\t" << "cputime\t" << "needtime\t" << "priority\t" << "state" << endl;
		cout << p->NAME << "\t" << p->CPUTIME << "\t" << p->NEEDTIME << "\t\t" << p->PRIO << "\t\t" << p->STATE << endl;
		cout << endl;
	}
	else if (n == 2)
	{
		cout << "name\t" << "cputime\t" << "needtime\t" << "round\t" << "state" << endl;
		cout << p->NAME << "\t" << p->CPUTIME << "\t" << p->NEEDTIME << "\t\t" << p->ROUND << "\t" << p->STATE << endl;
		cout << endl;
	}

}

//���������㷨���Ƚ���
void PRISCH()
{
	FIRSTIN();  //��������������Ͷ������
	if (run != NULL)
	{
		run->PRIO--;
		run->CPUTIME++;
		run->NEEDTIME--;

		if (run->NEEDTIME == 0)         //ִ�����
		{
			//run = run->next;  //ָ�����
			if (finish == NULL)
			{
				finish = run;     //����ɵ�run�ŵ�finish��
			}
			else
			{
				run->next = finish;
				finish = run;  //�᲻�Ὣǰ������ɵĽ��̶�����������д�Ͳ�����T_T
			}
			run->STATE = 'F'; //��״̬Ϊ���̬
			PRINT(run);
			run = NULL;
		}
		else          //run->needtime��Ϊ0
		{
			run->STATE = 'W';
			PRINT(run);
			INSERT1(run);
			PRINT(run);;
		}

	}

			/*
				if (ready != NULL)    //�������в�Ϊ��
			{
				FIRSTIN();

			}
			else                  //����������Ϊ�գ�˵���Ѿ�ִ�����,�˳�ѭ��              
			{
				break;
			}
			
			
		
		
		else                   //run->needtime��Ϊ0
		{
			if (run->PRIO >= ready->PRIO)
			{
				PRINT(run);
				PRISCH();     //�ݹ�
			}
			else
			{
				run->STATE = 'W';
				PRINT(run);
				INSERT1(run);  //���䰴���ȼ����²����������
				FIRSTIN();     //���½��������е����׽��̵���

			}
			
		}
		*/
	

}

//��ʱ��Ƭ��ת�����Ƚ���
void ROUNDSCH()
{
	FIRSTIN();
	if (run != NULL)
	{
		run->CPUTIME += 2;
		run->NEEDTIME -= 2;
		if (run->NEEDTIME == 0)
		{
			//run = run->next;  //ָ�����
			if (finish == NULL)
			{
				finish = run;     //����ɵ�run�ŵ�finish��
			}
			else
			{
				run = finish->next;
				finish = run;
			}
			run->STATE = 'F'; //��״̬Ϊ���̬
			PRINT(run);
			run = NULL;
		}
		else
		{
			run->STATE = 'W';
			PRINT(run);
			INSERT2(run);    //�������²����������
			//FIRSTIN();     //���½��������е����׽��̵���
		}
	}
}



int main()
{
	cout << "��ѡ������㷨��" << endl;
	cout << "             ***1.�����������㷨***" << endl;
	cout << "             ***2.ѭ����ת�����㷨***" << endl;
	cout << "���루1 or 2����";
	
	cin >> n;
	while (n)
	{
		if (n == 1)
		{
			cout << "�����㷨���Ⱥ�Ľ����" << endl;
			CREATE();
			while (ready)
			{
				PRISCH();	
			}
			
			break;
		}
		else if (n == 2)
		{
			cout << "ѭ����ת���Ⱥ�Ľ����" << endl;
			CREATE();
			while (ready)
			{
				ROUNDSCH();
			}

			break;
		}
		else
		{
			cout << "input error!" << endl;
			break;

		}
	}
	cin.get();
	cin.get();
	cin.get();
	return 0;
}