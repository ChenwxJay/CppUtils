#include <iostream>
#include <>

class CgiConnecton{
public:
	CgiConnecton();
	~CgiConnecton();
	//初始化客户端连接
	void Init();
	//处理客户端连接
	void Process();
private:
	static const int BUFFER_SIZE = 1024;
	//epoll内核事件表描述符
	static int m_epollfd;

	//套接字描述符，绑定在连接上
	int m_sockfd;

	//客户端地址
	sockaddr_in m_address;
	//读缓冲区，大小由常量指定
	char m_buf[BUFFER_SIZE];

	//标记缓冲区中已经存放客户数据的最后一个字节的下一个位置
	int m_read_index;
};
//epoll描述符，静态变量，需要在外面初始化
int CgiConnecton::m_epollfd = -1;

void CgiConnecton::Init(){
    m_epollfd = epollfd;
    m_sockfd = sockfd;
    //客户端地址
    m_address = client_addr;
    //需要清除缓冲区
    memset(m_buf,'\0',BUFFER_SIZE);
    //索引清零
    m_read_index = 0;
}
void CgiConnecton::Process(){
	int index = 0;
	int ret = -1;
	//循环读取和分析客户端发送过来的数据
	while(true){
		index = m_read_index;
		//使用recv函数进行接收
		ret = recv(m_sockfd,m_buf + index,BUFFER_SIZE-1-index,0);
		//通过返回值来判断
		if(ret < 0){
			if(errno != EAGAIN)
			{   
				//连接，读错误，直接从epoll内核事件表中删除socket fd
				removefd(m_epollfd,m_sockfd);
			}
			break;
		}
		else if(ret == 0)
		{
			m_read_index += ret;
			//打印接收到的信息
			printf("use content: %s\n",m_buf);
			//循环处理接收到的请求
			for(;index < m_read_index;index++){
				if((index >= 1)&& (m_buf[index-1] == '\r')&&(m_buf[index] == '\n'))
				{   
					//如果接收到的客户数据包含"\r\n"就可以开始处理请求
					break;
				}
			}
			//校验是否检查到换行符
			if(index == m_read_index){
				//
				printf("Please input more data!");
				continue;
			}
			m_buf[index-1] = '\0';//在换行符处填充空字符，将请求行分割出来

			//获取文件名，即请求的资源
			char* FileName = m_buf;

			//判断用户要执行的cgi程序是否存在
			if(access(FileName,F_OK) == -1){
				//从epoll内核事件表移除sokcet描述符
                RemoveFd(m_epollfd,m_sockfd);
                break;
			}
			//创建子进程来运行
            ret = fork();
            if(ret == -1){
            	//创建子进程失败，删除套接字描述符
            	RemoveFd(m_epollfd,m_sockfd);
            	break;
            }
            else if(ret > 0){
            	//父进程不需要监听套接字
            	RemoveFd(m_epollfd,m_sockfd);
            	break;
            }
            else{
            	//子进程将标准输出描述符重定向到socket，并执行cgi程序
            	close(STDOUT_FILENO);//先关闭标准输出
            	//重定向
            	dup(m_sockfd);
            	//执行客户端请求的程序
            	excel(m_buf,m_buf,0);
            	//执行完毕就退出子进程
            	exit(0);
            }
		}
	}
}
