#include "ProcessPool.h"

#define MAX_EVENT_NUMBER 64

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
	//子进程通过自身在进程池中的编号找到与父进程通信的管道
	int pipe_fd = SubProcess_[this->owner].m_pipefd[1];
	//子进程对该管道进行监听
	Addfd(pipe_fd);

	//创建epoll_event数组
	epoll_event events[MAX_EVENT_NUMBER];
	//创建用户进程数组
	T* users = new T[USER_PER_PROCESS];
	if(users == nullptr){
		printf("failed to create users!\n");
		return;
	}
	//局部变量
	int number = 0;
	int ret = -1;
	//子进程循环
	while(!m_stop)

	{
        //epoll_wait函数，监听是否有事件就绪
        number = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
        //一旦epoll_wait返回，校验返回值
        if((number < 0)|| (errno != EINTR)){
        	//发生错误
        	printf("epoll_wait error!\n");
        	break;
        }
        //循环遍历所有就绪的事件
        for(int i = 0;i < number;i++)
        {
        	int sockfd = events[i].data.fd;//获取对应的描述符
        	if((sockfd == pipe_fd) &&(events[i].events & EPOLLIN))
        	{
        	  //从父进程接收数据，存储在
        		int client = 0;//客户端数据变量，一个int类型即可
        		//向监听的管道收消息
        	    ret = recv(m_epollfd,(char*)&client,sizeof(client),0)
        	    if((ret < 0) && (!errno->EAGAIN) || (ret == 0))
        	    {	//有两种情况
        	    	//1.返回值小于零，并errno并没有设置
        	    	//2.返回值等于零，表示没有描述符就绪
                    continue;
        	    }
        	    else
        	    {
        	    	struct sockaddr_in client_address;//客户端地址结构体
        	    	socklen_t address_len = sizeof(client_address);
        	    	//进行accept,获取客户端连接描述符
        	    	int connfd = accept(ListenFd,(struct sockaddr*)&client_address,&address_len);
        	    	if(connfd < 0){
        	    		printf("failed to create socket connection!\n");
        	    		continue;
        	    	}
        	    	Addfd(m_epollfd,connfd);//将新获得的连接套接字fd注册到epoll内核事件表
        	    	//初始化连接
        	    	users[connfd].init(m_epollfd,connfd,client_address);
        	    }
        	}
        	else if((sockfd == sig_pipefd[0])&&(events[i].events & EPOLLIN))
        	{
        		int sig_value;
        		char sig_buff[1024];//信号缓冲区，使用数组实现
        		//从管道接收父进程传过来的信号
        		ret = recv(sig_pipefd[0],sig_buff,sizeof(sig_buff),0);
        		if(ret <= 0){
                   continue;
        		}
        		//循环处理收到的信号值
        		for(int i = 0;i < ret;i++)
        		{
        			switch(sig_buff[i])
        			{
        				case SIGCHLD:
        				{
        					pid_t pid;//进程描述符
        					int stat;
        					while(pid = waitpid(-1,&stat,WNOHANG) > 0){//等待进程退出
        						continue;//一旦退出就执行下一次循环
        					}
        					break;
        				}
        				case SIGTERM:
        				case SIGINT:
        				{
        					m_stop = true;//置停止标志位
        					break;
        				}
        				default:
        				{   
        					printf("can not match right signal!\n");
        					break;
        				}
        			}
        		}
        	}
        	else if(events[i].events & EPOLLIN)
        	{
               //客户端请求到来，调用process函数处理请求
        		users[sockfd].process();
        	}
        	else{
        		//匹配不到，跳过本次循环
        		continue;
        	}
        } 
	}
	printf("The child process will exit!\n");
	//子进程退出，先释放资源
	delete users;
	users = nullptr;//指针置空，避免野指针
	close(pipe_fd);//关闭管道
	close(m_epollfd);//关闭epoll描述符
    
}
//启动主进程，使用模板类的函数
template<class T>
void ProcessPool<T>::StartMainProcess(){
	SetupSigPipe();
}