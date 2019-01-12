#include "shared_ptr.h"

using namespace std;

template<typename T>
class shared_ptr{
public:
	//拷贝构造函数
	shared_ptr(const shared_ptr& ptr);
	//移动构造函数
	shared_ptr(shared_ptr&& ptr) noexcept;
	//移动赋值运算符重载
	shared_ptr& operator=();
	//引用计数函数
	size_t use_count(){
		//返回内部计数器的值
		return *count;
	}
	//unique函数实现
	bool unique()const {
		//判断内部引用计数是否为1
        return *count == 1;
	}
	//重载比较运算符
	bool operator bool(){
	   //只要内部指针不为nullptr,该智能指针对象就不会指向nullptr
       return ptr_ != nullptr;
	}
	//reset函数
	void reset(T* ptr);
    //swap函数
    void swap(shared_ptr& temp);
    //release函数
    T* release()const{
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
    //重载*运算符，->运算符，实现指针运算
private:
    std::size_t* count;
    T* ptr;
    //内部含有的function对象，实现自定义删除器
    std::function<void(T*)> deleter;
}