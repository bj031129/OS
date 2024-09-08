#include <iostream>
#include <string>
#include <iomanip> // 包含iomanip头文件以使用setw

using namespace std;

class PCB
{
public:
    string name;  // 进程名
    int pri;      // 优先级
    int cpuTime;  // 已用CPU时间
    int needTime; // 需要的总CPU时间
    char state;   // 进程状态，Ready、Run、Finish
    PCB *next;    // 指向下一个PCB的指针

    PCB(string n, int p, int nt)
        : name(n), pri(p), cpuTime(0), needTime(nt), state('W'), next(nullptr) {}
};

PCB *readyQueue = nullptr;
PCB *runningProcess = nullptr;
PCB *finishedProcesses = nullptr;

int numOfProcesses;

void getFirstPCB();                 // 获取就绪队列中的第一个进程
void outputStatus();                // 输出所有进程信息
void insertByPriority(PCB *pcb);    // 根据优先级将进程插入到就绪队列中
void moveToFinished(PCB *pcb);      // 将完成的进程移入到完成队列
void createProcessesWithPriority(); // 创建优先级调度的进程实例
void priorityScheduling();          // 执行优先级调度算法

int main()
{
    cout << "优先级调度算法" << endl;
    cout << "请输入进程的数量：";
    cin >> numOfProcesses;
    createProcessesWithPriority();
    priorityScheduling();
    outputStatus();
    return 0;
}

void createProcessesWithPriority()
{
    PCB *newPCB;
    string name;
    int pri, needTime;
    cout << "请输入进程名、需要的时间、优先级（每行一个进程）：\n";
    for (int i = 0; i < numOfProcesses; i++)
    {
        cin >> name >> needTime >> pri;
        newPCB = new PCB(name, pri, needTime);
        insertByPriority(newPCB);
    }
}

void getFirstPCB()
{
    runningProcess = readyQueue;
    if (readyQueue != nullptr)
    {
        runningProcess->state = 'R';
        readyQueue = runningProcess->next;
        runningProcess->next = nullptr; // 断开与原就绪队列的链接
    }
}

void outputStatus()
{
    PCB *currentPCB;
    // 设置输出格式头部
    cout << left << setw(10) << "pcbName" << setw(10) << "priority" << setw(10) << "cpuTime" << setw(10) << "needTime" << setw(10) << "proState" << endl;
    cout << "就绪队列" << endl;

    // 遍历并打印就绪队列
    for (currentPCB = readyQueue; currentPCB != nullptr; currentPCB = currentPCB->next)
    {
        cout << left << setw(10) << currentPCB->name
             << setw(10) << currentPCB->pri
             << setw(10) << currentPCB->cpuTime
             << setw(10) << currentPCB->needTime
             << setw(10) << currentPCB->state
             << endl;
    }

    cout << "完成队列" << endl;
    for (currentPCB = finishedProcesses; currentPCB != nullptr; currentPCB = currentPCB->next)
    {
        cout << left << setw(10) << currentPCB->name
             << setw(10) << currentPCB->pri
             << setw(10) << currentPCB->cpuTime
             << setw(10) << currentPCB->needTime
             << setw(10) << currentPCB->state
             << endl;
    }

    cout << "当前运行" << endl;
    if (runningProcess != nullptr)
    {
        cout << left << setw(10) << runningProcess->name
             << setw(10) << runningProcess->pri
             << setw(10) << runningProcess->cpuTime
             << setw(10) << runningProcess->needTime
             << setw(10) << runningProcess->state
             << endl;
    }

    cout << "-----------------------------------------------------" << endl;
}

void insertByPriority(PCB *pcb)
{
    pcb->state = 'W'; // 初始状态为等待
    PCB *firstPCB, *nextPCB;
    if (readyQueue == nullptr)
    {
        pcb->next = readyQueue;
        readyQueue = pcb;
    }
    else
    {
        firstPCB = nextPCB = readyQueue;
        if (pcb->pri < firstPCB->pri)
        {
            pcb->next = readyQueue;
            readyQueue = pcb;
        }
        else
        {
            while (firstPCB->next != nullptr && pcb->pri >= firstPCB->pri)
            {
                nextPCB = firstPCB;
                firstPCB = firstPCB->next;
            }
            if (firstPCB->next == nullptr)
            {
                pcb->next = nullptr;
                nextPCB->next = pcb;
            }
            else
            {
                nextPCB->next = pcb;
                pcb->next = firstPCB;
            }
        }
    }
}

void moveToFinished(PCB *pcb)
{
    PCB *firstPCB = finishedProcesses;
    if (finishedProcesses == nullptr)
    {
        pcb->next = finishedProcesses;
        finishedProcesses = pcb;
    }
    else
    {
        while (firstPCB->next != nullptr)
            firstPCB = firstPCB->next;
        pcb->next = nullptr;
        firstPCB->next = pcb;
    }
}

void priorityScheduling()
{
    int flag = 1;
    getFirstPCB();
    while (runningProcess != nullptr)
    {
        outputStatus();
        while (flag)
        {
            runningProcess->pri += 2; // 每个时间片后优先级+2
            if (runningProcess->needTime <= 10)
            {
                runningProcess->state = 'F';
                runningProcess->cpuTime += runningProcess->needTime;
                runningProcess->needTime = 0;
                moveToFinished(runningProcess);
                flag = 0;
            }
            else
            {
                runningProcess->state = 'W';
                runningProcess->cpuTime += 10;
                runningProcess->needTime -= 10;
                insertByPriority(runningProcess);
                flag = 0;
            }
        }
        flag = 1;
        getFirstPCB(); // 取下一个进程继续调度
    }
}
