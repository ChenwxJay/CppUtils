#include <iostream>
#include <map>

using namespace std;
template <class T>
class My_auto_ptr{
public:
	explicit My_auto_ptr(T* ptr = nullptr):ptr_(ptr)
	{   //构造函数，使用成员初始化列表
	  	cout << "Construct a object managed by auto_ptr!" << endl;
	}
	//拷贝构造函数
	My_auto_ptr(My_auto_ptr& temp);
    //赋值运算符重载
    My_auto_ptr& operator=(My_auto_ptr& temp);
	~My_auto_ptr(){
        cout << "destroy the object hold by auto_ptr!" << endl;
        //在析构auto_ptr的时候需要调用内部指针的析构函数
        delete ptr_;
	}	
	T& operator*(){
		//返回内部指针指向的对象
		return *ptr_;
	}
	//重载->运算符，返回内部原生指针
    T* operator->(){
    	//返回内部指针
    	return ptr_;
    }
	//释放函数，返回内部持有的指针
	T* release();
	//重置函数
	void reset(T* ptr);
private:
	T* ptr_;
};
//返回auto_ptr内部含有的原生指针，并将内部指针置空
template<class T>
T* My_auto_ptr<T>::release(){
   T* temp = ptr_;
   ptr_ = nullptr;
   return temp;
}
//重载赋值运算符
template<typename T>
My_auto_ptr& My_auto_ptr<T>::operator=(My_auto_ptr& temp){
	//先判断是否为指针本身，不是再进行拷贝
	if(temp!=*this){
      //需要析构原来的资源
		delete this->ptr_;
		this->ptr_ = temp.release();
	}
	//最后返回拷贝后的原对象
	return *this;
}
//reset函数，重置内部指针，需要先
template<typename T>
My_auto_ptr<T>& My_auto_ptr<T>::reset(T* ptr){
    if(ptr_ != nullptr)
    {
    	delete ptr_;//释放原先管理的资源
    	ptr_ = ptr;//接收传进来的指针，管理新资源
    }
    return *this;
}
//拷贝构造函数定义
template<typename T>
My_auto_ptr<T>::My_auto_ptr(My_auto_ptr& temp){
	//拷贝构造函数，需要调用release函数，即
	ptr_ = temp.release();
}
