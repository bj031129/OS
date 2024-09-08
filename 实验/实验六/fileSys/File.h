#ifndef FILE_H
#define FILE_H

#include <string>

/**
 * @class File
 * @brief 表示文件系统中的一个文件。
 * File 类封装了文件的属性和操作，包括文件名和内容。
 */
class File {
public:
    std::string name;    ///< 文件的名称。
    std::string content; ///< 文件的内容。

    /**
     * @brief 构造一个新的 File 对象。
     * 这个构造函数使用指定的名称初始化一个 File 对象。
     * 文件内容初始为空。
     * @param name 文件的名称。
     */
    File(std::string name);

    /**
     * @brief 销毁 File 对象。
     * 这个析构函数清理 File 对象使用的任何资源。
     */
    ~File();
};

#endif // FILE_H
