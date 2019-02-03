#include <iostream>
#include <memory>
using namespace std;

class UseCount{
public:
	int s;//shared_ptr引用计数
	int w;//weak_ptr引用计数
	UseCount():s(0),w(0){
        cout << "construct a counter!" << endl;
	}
};
//weak_ptr完整定义
template<class T>
class weak_ptr{
public:
	weak_ptr(shared_ptr<T>& s):ptr(s.ptr),count(s.count){
		cout << "construct a weak_ptr" << endl;
		count->w++;
	}
	//参数是weak_ptr对象，对应构造函数的一个版本，拷贝构造
	weak_ptr(weak_ptr<T>& w):ptr(w.ptr),count(w.count){
		count->w++;
	}
	~weak_ptr(){
		release();//直接释放资源
	}
	//赋值运算符重载函数，参数是weak_ptr对象
	weak_ptr<T>& operator=(const weak_ptr<T>& temp);
	//赋值运算符重载函数，参数是shared_ptr对象
	weak_ptr<T>& operator=(const shared_ptr<T>& s);
	
	//lock函数，返回一个shared_ptr<T>指针
	shared_ptr<T> lock(){
		return shared_ptr<T> (*this);
	}
    //expired函数，判断引用计数是否为0
    friend class shared_ptr<T>; //设置友元类，方便操作引用计数
private:
	//用于智能指针的析构
	void release();

	T* ptr;//底层封装的指针
	UseCount* count;
}
template<typename T>
bool weak_ptr<T>::expired(){
    if(count != nullptr){ //引用计数不为，不是独占性指针
    		if(count->s > 0){
    			cout << "empty" << endl;
    			return false;
    		}
    }
    //其他情况认为是独占性指针
    return true;
}
template<typename T>
void weak_ptr<T>::swap(weak_ptr<T>& other){
	std::swap(ptr,other.ptr);
	count.swap(other.count);//调用
}

template<class T>
void weak_ptr<T>::release(){
    
}

template<typename T>
