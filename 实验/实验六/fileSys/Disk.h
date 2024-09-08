#ifndef DISK_H
#define DISK_H

#include <vector>
#include <string>
#include <unordered_map>
#include "File.h"

/**
 * @class Directory
 * @brief 表示文件系统中的一个目录。
 *
 * Directory 类包含了目录的名称以及目录中包含的文件和子目录。
 */
class Directory {
public:
    std::string name;    ///< 目录的名称。
    std::unordered_map<std::string, File*> files; ///< 目录中的文件。
    std::unordered_map<std::string, Directory*> directories; ///< 目录中的子目录。

    Directory(std::string name);  ///< 构造一个新的 Directory 对象。
    ~Directory();  ///< 销毁 Directory 对象。
};

/**
 * @class Disk
 * @brief 表示一个简单的内存文件系统。
 *
 * Disk 类包含了文件系统的根目录和当前目录，并提供了基本的文件和目录操作。
 */
class Disk {
public:
    Directory* root; ///< 文件系统的根目录。
    Directory* currentDirectory; ///< 当前工作目录。

    Disk();  ///< 构造一个新的 Disk 对象。
    ~Disk();  ///< 销毁 Disk 对象。

    void ls();  ///< 显示当前目录下的文件和子目录。
    void cd(std::string dirName);  ///< 切换到指定的目录。
    void mv(std::string oldName, std::string newDirPath);  ///< 移动文件或目录到指定路径。
    void touch(std::string fileName);  ///< 创建一个新的文件。
    void mkdir(std::string dirName);  ///< 创建一个新的目录。
    void rm(std::string fileName);  ///< 删除指定的文件。
    void rmdir(std::string dirName);  ///< 删除指定的目录。
    void read(std::string fileName);  ///< 读取指定文件的内容并显示。
    void write(std::string fileName, std::string content);  ///< 向指定文件写入内容。
    void exit();  ///< 退出文件系统。

private:
    void deleteDirectory(Directory* dir);  ///< 删除指定的目录及其内容。
    void deleteFile(File* file);  ///< 删除指定的文件。
    Directory* navigateToDirectory(const std::string& path);  ///< 导航到指定路径的目录。
};

#endif // DISK_H
