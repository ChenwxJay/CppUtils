#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

//进程描述类，封装了目标进程的pid和与其他进程通信使用的管道
class Process{
public:
	Process():m_pid(-1){
		cout << "Create a process!" << endl;
	}
	~Process(){}
private:
	pid_t m_pid;
	int m_pipefd[2];//进程通信的管道
};
//进程池模板类
template<class T>
class ProcessPool{
public:
	void StartMainProcess();
private:
    void SetupSigPipe();
    void StartMainProcess();
    void StartChildProcess();
//数据成员定义
private:
	//最大进程数
	static const int MAX_PROCESS_NUMBER = 16;
	//每个子进程最多能处理的客户端连接数量
	static const int USER_PER_PROCESS = 65536;
	//每个进程能够处理的epoll事件数
	static const int MAX_EVENT_NUMBER = 10000;

	//进程池中的进程总数
	int m_process_number;

	//子进程在进程池中的序号，从0开始
	int m_idx;
	//进程池中的每一个进程都有一个epoll内核事件表
	int m_epollfd;
	//监听套接字
	int m_listenfd;
	//保存所有子进程的描述信息，管理子进程
	Process* m_sub_process;
	//停止标志位
	int m_stop;
    //进程池静态实例
    static ProcessPool<T>* m_instance;
};