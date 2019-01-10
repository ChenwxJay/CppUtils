#include "unique_ptr.h"
using namespace std;

template<typename T,typename Delete>
void unique_ptr<T,Delete>::swap(unique_ptr<T> &rhs){
	//指定使用std命名空间的swap函数
	using std::swap;
	swap(this->pointer,rhs.pointer);//交换底层指针
	//需要交换删除器
	swap(this->deleter,rhs.deleter);
}
//重置函数，将智能指针内部的指针设置为参数指针，注意释放原先管理的资源
template<typename T,typename Delete>
void unique_ptr<T,Delete>::reset(T* ptr) noexcept
{
	unique_ptr<T> temp(ptr);//构造一个临时的智能指针对象
	//交换temp对象管理的指针和当前指针管理的资源
	swap(temp);
}
