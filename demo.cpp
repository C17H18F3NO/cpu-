#include<iostream>
using namespace std;
#define MAX_PEOC_ID  65536

//进程控制块
typedef struct PCB
{
    char name[10];//进程名 id
    char state;  //进程状态 W/R
    int ArriveTime; //进程到达时间
    int StartTime;  //进程开始时间
    int FinishTime;//进程结束时间
    int ServiceTime;//进程服务时间
    float WaitTime; // 等待时间    （=周转时间-服务时间）（但是实际上这里计算的是开始时间-到达时间）
    float WholeTime; //周转时间   （=进程结束时刻-进程到达时刻；平均就再/线程数）
    float Weight_WholeTime;//带权周转时间   （=周转时间/服务时间）
    double Average_WaitTime; // 平均等待时间
    double Average_WholeTime;//平均周转时间
    double Average_Weight_WholeTime;//带权平均周转时间    （=周转时间/服务时间）
//    int RunTime;//已经占用CPU时间
//    int NeedTime;//还要占用CPU时间
    int Prio;//优先级
    struct PCB *next;
}pcb;


double Sum_WaitTime=0,Sum_WholeTime=0,Sum_Weight_WholeTime=0;
int time=0;
int Proc_Num=0;
pcb *head = NULL;
pcb *tail=NULL;

void Print()
{
    pcb* cur_proc=head;
    pcb*new_proc=NULL;
    while(cur_proc)
    {
        if(cur_proc->state == 'W')
        {
            new_proc=cur_proc;
            //FCFS_RunProccess(new_proc);
            new_proc->StartTime=time;
            if(new_proc->StartTime<new_proc->ArriveTime){
                new_proc->StartTime=new_proc->ArriveTime;
            }
            cout<<"时刻 "<<time<<" 开始执行当前作业  "<<new_proc->name<<endl;
            time+=new_proc->ServiceTime;
            new_proc->state='R';
            new_proc->FinishTime=time;
            new_proc->WholeTime=new_proc->FinishTime-new_proc->ArriveTime;
            new_proc->Weight_WholeTime=new_proc->WholeTime/new_proc->ServiceTime;
            new_proc->WaitTime=new_proc->StartTime-new_proc->ArriveTime;
            //new_proc->WaitTime=new_proc->WholeTime-new_proc->ServiceTime;

//            if(new_proc->WholeTime>new_proc->ServiceTime){
//                new_proc->WaitTime=new_proc->WholeTime-new_proc->ServiceTime;
//            }
//            else{
//                new_proc->WaitTime=new_proc->ServiceTime-new_proc->WholeTime;
//            }

            Sum_WaitTime+=new_proc->WaitTime;
            Sum_WholeTime+=new_proc->WholeTime;
            Sum_Weight_WholeTime+=new_proc->Weight_WholeTime;

            new_proc->Average_WaitTime=Sum_WaitTime/Proc_Num;
            new_proc->Average_WholeTime=Sum_WholeTime/Proc_Num;
            new_proc->Average_Weight_WholeTime=Sum_Weight_WholeTime/Proc_Num;
            printf("  到达时间  开始时间  服务时间  完成时间  等待时间  周转时间  带权周转时间\n");
            printf("  %6d   %6d   %6d   %6d  %8.1f  %8.1f  %8.2f\n",
                   new_proc->ArriveTime,new_proc->StartTime,new_proc->ServiceTime,
                   new_proc->FinishTime,new_proc->WaitTime,new_proc->WholeTime,new_proc->Weight_WholeTime);
            if(new_proc->next==NULL){
                printf("  平均等待时间  平均周转时间  平均带权周转时间  \n");
                printf("  %10.2f     %10.2f    %10.2f\n",new_proc->Average_WaitTime,
                        new_proc->Average_WholeTime,new_proc->Average_Weight_WholeTime);
            }
        }
        cur_proc=cur_proc->next;
        head=cur_proc;
        free (new_proc);
        new_proc=NULL;
    }
}

void FCFS_CreateProccess()
{
    cout<<"请输入进程的个数: ";
    cin>>Proc_Num;
    if(Proc_Num > MAX_PEOC_ID)
    {
        cout<<"sorry  I can't give you PCB \n";
        return;
    }
    for(int i=1;i<=Proc_Num;++i)
    {
        pcb*new_proc=NULL;
        if((new_proc=(pcb*)malloc(sizeof(pcb))) == NULL)
        {
            perror("malloc");
            return;
        }
        cout<<"请输入第"<<i<<"个进程名：";
        cin>>new_proc->name;
        cout<<"请输入第"<<i<<"个进程到达时间：";
        cin>>new_proc->ArriveTime;
        cout<<"请输入第"<<i<<"个进程服务时间：";
        cin>>new_proc->ServiceTime;
        new_proc->next=NULL;
        if(head == NULL)
        {
            new_proc->next=head;
            head=new_proc;
            tail=head;
            time=new_proc->ArriveTime;
        }
        else
        {
            if(head->ArriveTime>new_proc->ArriveTime)
            {
                new_proc->next=head;
                head=new_proc;
            }
            else
            {
                pcb* cur_proc=head;
                while(cur_proc->next != NULL && cur_proc->next->ArriveTime<new_proc->ArriveTime)
                {
                    cur_proc=cur_proc->next;
                }
                if(cur_proc->next==NULL)
                {
                    tail=new_proc;
                }
                new_proc->next=cur_proc->next;
                cur_proc->next=new_proc;

            }
            if(new_proc->ArriveTime < time)
            {
                time=new_proc->ArriveTime;
            }
        }
        new_proc->StartTime=0;
        new_proc->FinishTime=0;
        new_proc->WholeTime=0;
        new_proc->WaitTime=0;
        new_proc->Weight_WholeTime=0;
        Sum_WaitTime=0;
        Sum_WholeTime=0;
        Sum_Weight_WholeTime=0;
        new_proc->Average_WaitTime=0;
        new_proc->Average_Weight_WholeTime=0;
        new_proc->Average_WholeTime=0;
        new_proc->state= 'W';
//        new_proc->RunTime=0;
//        new_proc->NeedTime=0;
    }
}
void ShortJobsCreateProccess()
{
    cout<<"请输入进程的个数: ";
    cin>>Proc_Num;
    if(Proc_Num > MAX_PEOC_ID)
    {
        cout<<"sorry  I can't give you PCB \n";
        return;
    }
    for(int i=1;i<=Proc_Num;++i)
    {
        pcb*new_proc=NULL;
        if((new_proc=(pcb*)malloc(sizeof(pcb))) == NULL)
        {
            perror("malloc");
            return;
        }
        cout<<"请输入第"<<i<<"个进程名：";
        cin>>new_proc->name;
        cout<<"请输入第"<<i<<"个进程到达时间：";
        cin>>new_proc->ArriveTime;
        cout<<"请输入第"<<i<<"个进程服务时间：";
        cin>>new_proc->ServiceTime;
        new_proc->next=NULL;
        if(head == NULL)
        {
            new_proc->next=head;
            head=new_proc;
            tail=head;
            time=new_proc->ArriveTime;

        }
        else
        {
            if(head->ServiceTime>new_proc->ServiceTime)
            {
                new_proc->next=head;
                head=new_proc;
            }
            else
            {
                pcb* cur_proc=head;
                while(cur_proc->next != NULL && cur_proc->next->ServiceTime<new_proc->ServiceTime)
                {
                    cur_proc=cur_proc->next;
                }
                if(cur_proc->next==NULL)
                {
                    tail=new_proc;
                }
                new_proc->next=cur_proc->next;
                cur_proc->next=new_proc;

            }
            if(new_proc->ArriveTime < time)
            {
                time=new_proc->ArriveTime;
            }
        }
        new_proc->StartTime=0;
        new_proc->WaitTime=0;
        new_proc->FinishTime=0;
        new_proc->WholeTime=0;
        new_proc->Weight_WholeTime=0;
        Sum_WaitTime=0;
        Sum_WholeTime=0;
        Sum_Weight_WholeTime=0;
        new_proc->Average_WaitTime=0;
        new_proc->Average_Weight_WholeTime=0;
        new_proc->Average_WholeTime=0;
        new_proc->state= 'W';
//        new_proc->RunTime=0;
//        new_proc->NeedTime=0;
    }
}
void PrioCreateProccess()
{
    cout<<"请输入进程的个数: ";
    cin>>Proc_Num;
    if(Proc_Num > MAX_PEOC_ID)
    {
        cout<<"sorry  I can't give you PCB \n";
        return;
    }
    for(int i=1;i<=Proc_Num;++i)
    {
        pcb*new_proc=NULL;
        if((new_proc=(pcb*)malloc(sizeof(pcb))) == NULL)
        {
            perror("malloc");
            return;
        }
        cout<<"请输入第"<<i<<"个进程名：";
        cin>>new_proc->name;
        cout<<"请输入第"<<i<<"个进程优先级(值越小优先级越高): ";
        cin>>new_proc->Prio;
        cout<<"请输入第"<<i<<"个进程到达时间：";
        cin>>new_proc->ArriveTime;
        if(new_proc->ArriveTime==0){
            new_proc->Prio=0;
        }
        cout<<"请输入第"<<i<<"个进程服务时间：";
        cin>>new_proc->ServiceTime;
//        cout<<"请输入第"<<i<<"个进程优先级(值越小优先级越高): ";
//        cin>>new_proc->Prio;
        new_proc->next=NULL;
        if(head == NULL)
        {
            new_proc->next=head;
            head=new_proc;
            tail=head;
            time=new_proc->ArriveTime;

        }
        else
        {
            if(head->Prio>new_proc->Prio)
            {
                new_proc->next=head;
                head=new_proc;
            }
            else
            {
                pcb* cur_proc=head;
                while(cur_proc->next != NULL && cur_proc->next->Prio<new_proc->Prio)
                {
                    cur_proc=cur_proc->next;
                }
                if(cur_proc->next==NULL)
                {
                    tail=new_proc;
                }
                new_proc->next=cur_proc->next;
                cur_proc->next=new_proc;

            }
            if(new_proc->ArriveTime < time)
            {
                time=new_proc->ArriveTime;
            }
        }
        new_proc->StartTime=0;
        new_proc->WaitTime=0;
        new_proc->FinishTime=0;
        new_proc->WholeTime=0;
        new_proc->Weight_WholeTime=0;
        Sum_WaitTime=0;
        Sum_WholeTime=0;
        Sum_Weight_WholeTime=0;
        new_proc->Average_WaitTime=0;
        new_proc->Average_Weight_WholeTime=0;
        new_proc->Average_WholeTime=0;
        new_proc->state= 'W';
//        new_proc->RunTime=0;
//        new_proc->NeedTime=0;
    }
}
int main()
{
    int select=1;
    while(select)
    {
        cout<<"******************************************\n";
        cout<<"*****1.******* 先来先服务算法 ************\n";
        cout<<"*****2.********  短作业优先   ************\n";
        cout<<"*****3.********  优先级调度   ************\n";
        cout<<"*****0.**********退出*********************\n";
        cout<<"请选择:> ";
        cin>>select;
        switch(select)
        {
            case 1:
                FCFS_CreateProccess();
                Print();
                break;
            case 2:
                ShortJobsCreateProccess();
                Print();
                break;
            case 3:
                PrioCreateProccess();
                Print();
                break;
            default:
                break;
        }
    }
}

