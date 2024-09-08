#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class PCB
{
public:
    string name;  // 进程名
    int cputime;  // CPU已用时间
    int needtime; // 需要的总时间
    char state;   // 进程状态，Ready、Run、Finish
    PCB *next;    // 指向下一个PCB的指针

    PCB(string name, int needtime)
        : name(name), cputime(0), needtime(needtime), state('W'), next(nullptr) {}
};

PCB *readyQueue = nullptr;
PCB *runningProcess = nullptr;
PCB *finishedProcesses = nullptr; // 定义就绪队列、运行队列和完成队列

int numOfProcesses;
int timeSlice;

void getFirstPcb();                 // 从就绪队列中获取第一个进程
void outputStatus();                // 输出所有队列中的进程信息
void insertByPriority(PCB *pcb);    // 按优先级将进程插入就绪队列
void moveToFinished(PCB *pcb);      // 将进程移入完成队列
void createProcessesWithPriority(); // 创建进程并初始化
void priorityScheduling();          // 执行优先级调度算法

int main()
{
    cout << "优先级调度算法" << endl;
    cout << "请输入进程的数量：";
    cin >> numOfProcesses;
    cout << "请输入固定的时间片大小：";
    cin >> timeSlice;
    createProcessesWithPriority();
    priorityScheduling();
    outputStatus(); // 最后输出所有进程的状态
    return 0;
}

void createProcessesWithPriority()
{
    string name;
    int needtime;
    cout << "请输入进程名和需要的时间（每行一个进程）：" << endl;
    for (int i = 0; i < numOfProcesses; i++)
    {
        cin >> name >> needtime;
        PCB *newPcb = new PCB(name, needtime);
        insertByPriority(newPcb);
    }
}

void getFirstPcb()
{
    runningProcess = readyQueue;
    if (readyQueue != nullptr)
    {
        runningProcess->state = 'R';
        readyQueue = readyQueue->next;
        runningProcess->next = nullptr;
    }
}

void outputStatus()
{
    PCB *currentPcb;
    cout << left << setw(10) << "pcbName" << setw(10) << "cpuTime" << setw(10) << "needTime" << setw(10) << "proState" << endl;
    cout << "就绪队列" << endl;
    for (currentPcb = readyQueue; currentPcb != nullptr; currentPcb = currentPcb->next)
    {
        cout << left << setw(10) << currentPcb->name << setw(10) << currentPcb->cputime << setw(10) << currentPcb->needtime << setw(10) << currentPcb->state << endl;
    }

    cout << "完成队列" << endl;
    for (currentPcb = finishedProcesses; currentPcb != nullptr; currentPcb = currentPcb->next)
    {
        cout << left << setw(10) << currentPcb->name << setw(10) << currentPcb->cputime << setw(10) << currentPcb->needtime << setw(10) << currentPcb->state << endl;
    }

    cout << "正在运行" << endl;
    if (runningProcess != nullptr)
    {
        cout << left << setw(10) << runningProcess->name << setw(10) << runningProcess->cputime << setw(10) << runningProcess->needtime << setw(10) << runningProcess->state << endl;
    }
    cout << "-----------------------------------------------------" << endl;
}

void insertByPriority(PCB *pcb)
{
    pcb->state = 'W';
    PCB *firstPcb;
    PCB *nextPcb;
    if (readyQueue == nullptr)
    {
        pcb->next = readyQueue;
        readyQueue = pcb;
    }
    else
    {
        firstPcb = nextPcb = readyQueue;
        if (pcb->needtime < firstPcb->needtime)
        {
            pcb->next = readyQueue;
            readyQueue = pcb;
        }
        else
        {
            while (firstPcb->next != nullptr && pcb->needtime >= firstPcb->needtime)
            {
                nextPcb = firstPcb;
                firstPcb = firstPcb->next;
            }
            if (firstPcb->next == nullptr)
            {
                pcb->next = nullptr;
                nextPcb->next = pcb;
            }
            else
            {
                nextPcb->next = pcb;
                pcb->next = firstPcb;
            }
        }
    }
}

void moveToFinished(PCB *pcb)
{
    PCB *firstPcb = finishedProcesses;
    if (finishedProcesses == nullptr)
    {
        pcb->next = finishedProcesses;
        finishedProcesses = pcb;
    }
    else
    {
        while (firstPcb->next != nullptr)
        {
            firstPcb = firstPcb->next;
        }
        pcb->next = nullptr;
        firstPcb->next = pcb;
    }
}

void priorityScheduling()
{
    int flag = 1;
    getFirstPcb();
    while (runningProcess != nullptr)
    {
        outputStatus();
        while (flag)
        {
            if (runningProcess->needtime <= timeSlice)
            {
                runningProcess->state = 'F';
                runningProcess->cputime += runningProcess->needtime;
                runningProcess->needtime = 0;
                moveToFinished(runningProcess);
                flag = 0;
            }
            else
            {
                runningProcess->state = 'W';
                runningProcess->cputime += timeSlice;
                runningProcess->needtime -= timeSlice;
                insertByPriority(runningProcess);
                flag = 0;
            }
        }
        flag = 1;
        getFirstPcb();
    }
}
