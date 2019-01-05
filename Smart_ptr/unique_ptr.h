#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

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
	unique_ptr(T* pointer = nullptr,const Delete& del =Delete()):un_ptr(pointer),deleter(del)
	{
       cout << "Constructor two!" << endl;
	}
	//禁止拷贝构造函数和赋值运算符重载函数
	unique_ptr(const unique_ptr&) = delete ;
    unique_ptr& operator=(const unique_ptr& ) = delete ;
    //允许移动构造
    unique_ptr(unique_ptr&& temp):un_ptr(temp.un_ptr),del(std::move(temp.deleter))
    {
       //移动构造函数，因为可能利用右值，因此资源转移在成员初始化列表完成
       temp.un_ptr = nullptr;//原指针置空，避免野指针
    }
    //重载移动赋值函数
    unique_ptr& operator=(unique_ptr&& right_value) noexcept{
       
    }
}
