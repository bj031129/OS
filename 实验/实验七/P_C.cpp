#include <iostream>  // 标准输入输出库
#include <cstdlib>   // 包含rand()函数和srand()函数
#include <ctime>     // 包含time()函数
#include <windows.h> // 包含Windows API函数和数据类型

using namespace std;

const int BUFFER_SIZE = 10; // 缓冲区大小
int buffer[BUFFER_SIZE];    // 用于存储数据的缓冲区
int front, rear;            // 分别指向缓冲区的前端和后端
bool full;                  // 指示缓冲区是否已满
HANDLE Mutex;               // 互斥量，用于保护对缓冲区的访问
HANDLE Full;                // 信号量，指示缓冲区中有可消费的项目
HANDLE Empty;               // 信号量，指示缓冲区中有可插入的新项目

// 打印缓冲区内容的函数
void showBuffer()
{
    cout << "buffer: ";
    if (front != rear || full)
    {
        int i = front;
        do
        {
            cout << buffer[i] << " "; // 打印缓冲区中的每个元素
            i = (i + 1) % BUFFER_SIZE;
        } while (i != rear);
    }
    else
        cout << "empty"; // 如果缓冲区为空，打印“empty”
    cout << "\n*******************************" << endl;
}

// 插入元素到缓冲区的函数
int insert_item(int item)
{
    int condition = -1;
    WaitForSingleObject(Empty, INFINITE); // 等待信号量Empty，表示有空位可插入
    WaitForSingleObject(Mutex, INFINITE); // 等待互斥量Mutex，进入临界区

    if (!full) // 如果缓冲区未满
    {
        buffer[rear] = item;             // 插入元素到缓冲区
        rear = (rear + 1) % BUFFER_SIZE; // 更新rear指针
        if (rear == front)               // 如果rear追上front，缓冲区已满
            full = true;
        condition = 0;                                // 表示插入成功
        cout << "producer produced " << item << endl; // 打印生产的信息
        showBuffer();                                 // 显示缓冲区内容
    }

    ReleaseMutex(Mutex);             // 释放互斥量，退出临界区
    ReleaseSemaphore(Full, 1, NULL); // 增加信号量Full的计数，表示有新的元素可消费
    return condition;
}

// 从缓冲区移除元素的函数
int remove_item(int item)
{
    int condition = -1;
    WaitForSingleObject(Full, INFINITE);  // 等待信号量Full，表示有元素可消费
    WaitForSingleObject(Mutex, INFINITE); // 等待互斥量Mutex，进入临界区

    if (front != rear || full) // 如果缓冲区不为空
    {
        item = buffer[front];              // 移除缓冲区中的元素
        front = (front + 1) % BUFFER_SIZE; // 更新front指针
        full = false;                      // 缓冲区不再满
        condition = 0;                     // 表示移除成功

        cout << "consumer comsumed " << item << endl; // 打印消费的信息
        showBuffer();                                 // 显示缓冲区内容
    }
    ReleaseMutex(Mutex);              // 释放互斥量，退出临界区
    ReleaseSemaphore(Empty, 1, NULL); // 增加信号量Empty的计数，表示有新的空位可插入
    return condition;
}

// 生产者线程函数
DWORD WINAPI producer(void *param)
{
    srand((unsigned)time(0)); // 使用当前时间作为随机数种子

    while (1) // 无限循环
    {
        Sleep((rand() % 50) + 1000);       // 随机等待1到1.05秒
        insert_item(rand() % BUFFER_SIZE); // 插入一个随机数到缓冲区
    }
};

// 消费者线程函数
DWORD WINAPI consumer(void *param)
{
    srand((unsigned)time(0)); // 使用当前时间作为随机数种子

    while (1) // 无限循环
    {
        Sleep((rand() % 100) + 1500);      // 随机等待1.5到1.6秒
        remove_item(rand() % BUFFER_SIZE); // 从缓冲区移除一个随机数
    }
};

int main(int argc, char *argv[])
{
    front = 0;    // 初始化front指针
    rear = 0;     // 初始化rear指针
    full = false; // 初始化缓冲区状态为不满

    Mutex = CreateMutex(NULL, FALSE, NULL);         // 创建互斥量
    Full = CreateSemaphore(NULL, 0, 5, NULL);       // 创建初始计数为0的信号量Full
    Empty = CreateSemaphore(NULL, 5, 5, NULL);      // 创建初始计数为5的信号量Empty
    CreateThread(NULL, 0, producer, NULL, 0, NULL); // 创建生产者线程
    CreateThread(NULL, 0, consumer, NULL, 0, NULL); // 创建消费者线程

    Sleep(400000); // 主线程睡眠400秒，给生产者和消费者线程足够的运行时间
    return 0;
}
