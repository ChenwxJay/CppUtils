#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <iostream>
#include <algorithm>

template<typename T>
class Deleter{
public:
	//重载括号运算符
	void operator()(T* ptr){
         delete ptr;
	}
}
template <typename T,typename Delete = Deleter>
class unique_ptr{
public:
	//默认构造函数
	unique_ptr(){}
	//带参构造，参数是某个类型的指针
	unique_ptr(T* pointer = nullptr):un_ptr(pointer)
	{  
       cout << "Constructor one!" << endl;
	}
	//带参构造函数，第二个参数是自定义删除器，有默认参数值
	unique_ptr(T* pointer = nullptr,const Delete& del = Delete()):un_ptr(pointer),deleter(del)
	{
       cout << "Constructor two!" << endl;
	}
	unique_ptr(unique_ptr&& temp):un_ptr(temp.un_ptr),del(std::move(temp.deleter))
    {
       //移动构造函数，因为可能利用右值，因此资源转移在成员初始化列表完成
       temp.un_ptr = nullptr;//原指针置空，避免野指针
    }
    //重载移动赋值函数
    unique_ptr& operator=(unique_ptr&& right_value) noexcept{
       
    }
    //析构函数定义
    ~unique_ptr(){
       //调用指定的析构函数对其内部指针进行析构
       deleter(p);
       p = nullptr;//避免野指针
    }
    //release方法
    T* release() noexcept{
    	T* temp = pointer;
    	pointer = nullptr;
    	return temp;
    }
    //重载比较运算符
    bool operator bool() const{
        return this->pointer != nullptr;
    }
    //重载指针运算符
    T& operator*(){
       return *pointer;//返回底层对象的引用
    }
    
private:
	//禁止拷贝构造函数和赋值运算符重载函数
	unique_ptr(const unique_ptr&) = delete ;
    unique_ptr& operator=(const unique_ptr& ) = delete ;
    //允许移动构造
    
}
