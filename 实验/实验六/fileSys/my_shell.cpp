#include <iostream>
#include <sstream>
#include "Disk.h"

// 解析用户输入的命令并调用相应的磁盘操作函数
void parseCommand(Disk &disk, const std::string &command) {
    std::istringstream iss(command);  // 使用字符串流解析命令
    std::string cmd, arg1, arg2;
    iss >> cmd >> arg1;  // 提取命令及参数
    std::getline(iss, arg2);  // 获取可能存在的第二个参数
    if (!arg2.empty() && arg2[0] == ' ') {
        arg2 = arg2.substr(1);  // 去除参数前的空格
    }

    // 根据命令调用对应的磁盘操作函数
    if (cmd == "ls") {
        disk.ls();
    } else if (cmd == "cd") {
        disk.cd(arg1);
    } else if (cmd == "mv") {
        disk.mv(arg1, arg2);
    } else if (cmd == "touch") {
        disk.touch(arg1);
    } else if (cmd == "mkdir") {
        disk.mkdir(arg1);
    } else if (cmd == "rm") {
        disk.rm(arg1);
    } else if (cmd == "rmdir") {
        disk.rmdir(arg1);
    } else if (cmd == "read") {
        disk.read(arg1);
    } else if (cmd == "write") {
        disk.write(arg1, arg2);
    } else if (cmd == "exit") {
        disk.exit();
    } else {
        std::cout << "Unknown command: " << cmd << std::endl;  // 未知命令
    }
}

int main() {
    Disk disk;  // 创建磁盘对象
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);  // 获取用户输入的命令
        if (command == "exit") {  // 如果输入为 exit，则退出循环
            break;
        }
        parseCommand(disk, command);  // 解析并执行用户命令
    }
    return 0;
}
