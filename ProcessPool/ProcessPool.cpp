#include "ProcessPool.h"

template<class T>
class ProcessPool{
public:
	void StartMainProcess();

}
//设置非阻塞描述符
int SetNonBlocking(int fd){
   int old_options = fcntl(fd,F_GETFL);
   int new_options = old_options |  O_NONBLOCK;
   fcntl(fd,F_SETFL,new_options);
   return old_options;//返回原来的描述符属性
}

template<typename T>
void ProcessPool<T>::SetupSigPipe(){
	//创建epollfd监听描述符
	EpollFd = epoll_create(EPOLL_EVENT_LIMIT);
	if(EpollFd == -1){
       printf("Failed to create epollfd!\n");
       return;
	}
	//创建信号管道，双向
	int ret = socketpair(PF_UNIX,SOCK_STREAM,0,SigPipeFd);
	if(ret == -1){
		printf("Failed to create pipe!\n");
		return;
	}
	//设置非阻塞描述符
	SetNonBlocking(SigPipeFd[1]);
	Addfd(EpollFd,SigPipeFd[0]);

	//添加信号处理函数
	AddSigHandler(SIGCHLD,sig_handler);
	AddSigHandler(SIGTERM,sig_handler);
	AddSigHandler(SIGPIPE,sig_handler);
	AddSigHandler(SIGINT,sig_handler);
}
template<typename T>
void ProcessPool<T>::run(){
	if(owner != -1){
		//子进程的owner标志大于等于0，此处执行子进程
        StartChildProcess();
        return;
	}
	//主进程的owner的标志为-1，执行主进程流程
	StartMainProcess();
}
template<typename T>
void ProcessPool<T>::StartChildProcess(){
	
}