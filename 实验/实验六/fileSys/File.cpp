#include "File.h"

/**
 * @brief 使用指定的名称构造一个新的 File 对象。
 * 
 * 文件内容初始为空。
 *
 * @param name 文件的名称。
 */
File::File(std::string name) : name(name) {
    // 构造函数实现
}

/**
 * @brief 销毁 File 对象。
 * 
 * 这个析构函数清理 File 对象使用的任何资源。
 */
File::~File() {
    // 析构函数实现
}
