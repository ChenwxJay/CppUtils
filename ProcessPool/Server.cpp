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
};

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
		}
	}
}