/*
**@author:Lijunjie
**@time:2018-4-30 22:04:55
**题目：进程模拟调度算法
**assume:优先数大的，优先级高
*/

#include<iostream>
using std::cout;
using std::cin;
using std::endl;

#define jinchengshu 5
//char STATE[3] = { 'R','W','F' };

struct PCB
{
	char NAME[10];  //进程标识符
	int PRIO;  //进程优先数
	int ROUND; //进程每次轮转的时间片数
	int CPUTIME; //进程累计占用CPU的时间片数
	int NEEDTIME; //进程到完成还需要的时间片数
	char STATE; //进程状态
	PCB *next; //链指针,指向结构体类型的指针，连接下一个PCB的指针
};


PCB  *run, *ready,*tail,*finish; //*ready,*tail,*finish为队列指针，*run是当前运行指针
int n; //用户选择的算法

//将尚未完成的 PCB 按优先数顺序插入到就绪队列中；   
void INSERT1(PCB *p)
{
	/*冒泡排序方法
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
	PCB *temp;      //定义的临时指针变量，否则ready前面的值将会丢掉
	PCB *pre;       //当前插入位置的前一个位置的指针
	pre = temp = ready;  //让temp指向就绪队列的队首
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
				   //退出循环的重要性-_-
				//return;   //终止语句
			}
			pre = temp;  //让temp指针后移(即让ready指针后移)
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
	
	//遍历完还没有找到，就插到最后
	if (!temp)
	{
		tail->next = p;
		tail = tail->next; //tail指针后移
		tail->next = NULL;

	}
    
}

//在轮转法中，将执行了一个时间片单位（为 2），但尚未完成的进程的PCB，插到就绪队列的队尾
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


//创建新进程，并将它的PCB插入就绪队列
void CREATE()
{
	PCB *p;
	ready = NULL;
	tail = NULL;
	run = NULL;
	finish = NULL;
	cout << "请输入" << jinchengshu << "个进程的进程号和所需运行时间：" << endl;

	for (int i = 0; i < jinchengshu; i++)
	{
		//对PCB的初始化
		p = new PCB;     //申请的空间，把其地址给了p

		cin >> p->NAME >> p->NEEDTIME;
		p->CPUTIME = 0;
		p->ROUND = 2;
		p->PRIO = 50 - p->NEEDTIME;
		p->STATE = 'W';

		if (n == 1)
		{		
			//将新进入按优先级数加入就绪队列
			INSERT1(p);
		}
		else if (n == 2)
		{
			INSERT2(p);
		}
		//delete p;
	}
}



//调度就绪队列的链首进程投入运行
void FIRSTIN()
{
	run = ready; //运行头指针指向就绪队列头指针
	run->STATE = 'R';
	ready = ready->next; //指针后移
}

//显示每执行一次后所有进程的状态及有关信息
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

//按优先数算法调度进程
void PRISCH()
{
	FIRSTIN();  //将就绪队列链首投入运行
	if (run != NULL)
	{
		run->PRIO--;
		run->CPUTIME++;
		run->NEEDTIME--;

		if (run->NEEDTIME == 0)         //执行完毕
		{
			//run = run->next;  //指针后移
			if (finish == NULL)
			{
				finish = run;     //将完成的run放到finish中
			}
			else
			{
				run->next = finish;
				finish = run;  //会不会将前面已完成的进程丢掉？？这样写就不会啦T_T
			}
			run->STATE = 'F'; //置状态为完成态
			PRINT(run);
			run = NULL;
		}
		else          //run->needtime不为0
		{
			run->STATE = 'W';
			PRINT(run);
			INSERT1(run);
			PRINT(run);;
		}

	}

			/*
				if (ready != NULL)    //就绪队列不为空
			{
				FIRSTIN();

			}
			else                  //若就绪队列为空，说明已经执行完毕,退出循环              
			{
				break;
			}
			
			
		
		
		else                   //run->needtime不为0
		{
			if (run->PRIO >= ready->PRIO)
			{
				PRINT(run);
				PRISCH();     //递归
			}
			else
			{
				run->STATE = 'W';
				PRINT(run);
				INSERT1(run);  //将其按优先级重新插入就绪队列
				FIRSTIN();     //重新将就绪队列的链首进程调入

			}
			
		}
		*/
	

}

//按时间片轮转法调度进程
void ROUNDSCH()
{
	FIRSTIN();
	if (run != NULL)
	{
		run->CPUTIME += 2;
		run->NEEDTIME -= 2;
		if (run->NEEDTIME == 0)
		{
			//run = run->next;  //指针后移
			if (finish == NULL)
			{
				finish = run;     //将完成的run放到finish中
			}
			else
			{
				run = finish->next;
				finish = run;
			}
			run->STATE = 'F'; //置状态为完成态
			PRINT(run);
			run = NULL;
		}
		else
		{
			run->STATE = 'W';
			PRINT(run);
			INSERT2(run);    //将其重新插入就绪队列
			//FIRSTIN();     //重新将就绪队列的链首进程调入
		}
	}
}



int main()
{
	cout << "请选择调度算法：" << endl;
	cout << "             ***1.优先数调度算法***" << endl;
	cout << "             ***2.循环轮转调度算法***" << endl;
	cout << "输入（1 or 2）：";
	
	cin >> n;
	while (n)
	{
		if (n == 1)
		{
			cout << "优先算法调度后的结果：" << endl;
			CREATE();
			while (ready)
			{
				PRISCH();	
			}
			
			break;
		}
		else if (n == 2)
		{
			cout << "循环轮转调度后的结果：" << endl;
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