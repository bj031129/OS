#include "Disk.h"
#include <iostream>

// Directory 类的构造函数，初始化目录名称
Directory::Directory(std::string name) : name(name) {}

// Directory 类的析构函数，清理目录中的文件和子目录
Directory::~Directory() {
    // 清理目录中的文件
    for (auto& file : files) {
        delete file.second;
    }
    // 清理目录中的子目录
    for (auto& dir : directories) {
        delete dir.second;
    }
}

// Disk 类的构造函数，创建文件系统的根目录和当前目录
Disk::Disk() {
    root = new Directory("/");  // 根目录
    currentDirectory = root;    // 当前目录初始为根目录
}

// Disk 类的析构函数，清理文件系统的根目录及其内容
Disk::~Disk() {
    deleteDirectory(root);  // 递归清理根目录及其内容
}

// 显示当前目录下的文件和子目录
void Disk::ls() {
    // 显示子目录
    for (auto& dir : currentDirectory->directories) {
        if (dir.first != "..") {  // 排除父目录
            std::cout << dir.first << "/ ";
        }
    }
    // 显示文件
    for (auto& file : currentDirectory->files) {
        std::cout << file.first << " ";
    }
    std::cout << std::endl;
}

// 切换到指定的目录
void Disk::cd(std::string dirName) {
    if (dirName == "..") {  // 返回上级目录
        if (currentDirectory != root) {
            currentDirectory = currentDirectory->directories[".."];
        }
    } else if (currentDirectory->directories.find(dirName) != currentDirectory->directories.end()) {
        currentDirectory = currentDirectory->directories[dirName];  // 进入子目录
    } else {
        std::cout << "Directory not found!" << std::endl;  // 目录不存在
    }
}

// 移动文件或目录到指定路径
void Disk::mv(std::string oldName, std::string newDirPath) {
    // 导航到目标目录
    Directory* targetDir = navigateToDirectory(newDirPath);
    if (targetDir == nullptr) {
        std::cout << "Target directory not found!" << std::endl;
        return;
    }

    // 检查是否为文件
    if (currentDirectory->files.find(oldName) != currentDirectory->files.end()) {
        File* file = currentDirectory->files[oldName];
        currentDirectory->files.erase(oldName);
        targetDir->files[oldName] = file;  // 移动文件
    }
    // 检查是否为目录
    else if (currentDirectory->directories.find(oldName) != currentDirectory->directories.end()) {
        Directory* dir = currentDirectory->directories[oldName];
        currentDirectory->directories.erase(oldName);
        dir->directories[".."] = targetDir;  // 更新父目录
        targetDir->directories[oldName] = dir;  // 移动目录
    } else {
        std::cout << "File or Directory not found!" << std::endl;  // 文件或目录不存在
    }
}

// 创建一个新的文件
void Disk::touch(std::string fileName) {
    if (currentDirectory->files.find(fileName) == currentDirectory->files.end()) {
        currentDirectory->files[fileName] = new File(fileName);  // 创建新文件
    } else {
        std::cout << "File already exists!" << std::endl;  // 文件已存在
    }
}

// 创建一个新的目录
void Disk::mkdir(std::string dirName) {
    if (currentDirectory->directories.find(dirName) == currentDirectory->directories.end()) {
        Directory* newDir = new Directory(dirName);  // 创建新目录
        newDir->directories[".."] = currentDirectory;  // 设置父目录
        currentDirectory->directories[dirName] = newDir;
    } else {
        std::cout << "Directory already exists!" << std::endl;  // 目录已存在
    }
}

// 删除指定的文件
void Disk::rm(std::string fileName) {
    if (currentDirectory->files.find(fileName) != currentDirectory->files.end()) {
        delete currentDirectory->files[fileName];  // 删除文件
        currentDirectory->files.erase(fileName);
    } else {
        std::cout << "File not found!" << std::endl;  // 文件不存在
    }
}

// 删除指定的目录
void Disk::rmdir(std::string dirName) {
    if (currentDirectory->directories.find(dirName) != currentDirectory->directories.end()) {
        Directory* dir = currentDirectory->directories[dirName];
        currentDirectory->directories.erase(dirName);
        deleteDirectory(dir);  // 递归删除目录
    } else {
        std::cout << "Directory not found!" << std::endl;  // 目录不存在
    }
}

// 读取指定文件的内容并显示
void Disk::read(std::string fileName) {
    if (currentDirectory->files.find(fileName) != currentDirectory->files.end()) {
        std::cout << currentDirectory->files[fileName]->content << std::endl;  // 显示文件内容
    } else {
        std::cout << "File not found!" << std::endl;  // 文件不存在
    }
}

// 向指定文件写入内容
void Disk::write(std::string fileName, std::string content) {
    if (currentDirectory->files.find(fileName) != currentDirectory->files.end()) {
        currentDirectory->files[fileName]->content = content;  // 写入文件内容
    } else {
        std::cout << "File not found!" << std::endl;  // 文件不存在
    }
}

// 退出文件系统
void Disk::exit() {
    std::cout << "Exiting file system." << std::endl;  // 退出文件系统
}

// 递归删除目录及其内容
void Disk::deleteDirectory(Directory* dir) {
   
    // 清理目录中的文件
    for (auto& file : dir->files) {
        delete file.second;
    }
    dir->files.clear();  // 清空文件列表

    // 递归清理子目录
    for (auto& subdir : dir->directories) {
        if (subdir.first != "..") {  // 排除父目录
            deleteDirectory(subdir.second);
        }
    }
    dir->directories.clear();  // 清空子目录列表

    delete dir;  // 清理目录对象
}

// 导航到指定路径的目录
Directory* Disk::navigateToDirectory(const std::string& path) {
    Directory* dir = currentDirectory;
    size_t start = 0;
    size_t end = path.find('/');
    while (end != std::string::npos) {
        std::string dirName = path.substr(start, end - start);
        if (dir->directories.find(dirName) != dir->directories.end()) {
            dir = dir->directories[dirName];  // 导航到子目录
        } else {
            return nullptr;  // 路径不存在
        }
        start = end + 1;  // 更新起始位置
        end = path.find('/', start);  // 查找下一个斜杠
    }
    std::string dirName = path.substr(start);  // 获取路径中的目录名称
    if (dirName == "..") {
        return dir->directories[".."];  // 返回上级目录
    }
    if (dir->directories.find(dirName) != dir->directories.end()) {
        return dir->directories[dirName];  // 返回目标目录
    } else {
        return nullptr;  // 路径不存在
    }
}

// 删除指定的文件
void Disk::deleteFile(File* file) {
    delete file;  // 清理文件对象
}
