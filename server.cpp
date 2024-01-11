#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>//cout
#include "Logger.hpp"
#include "thread_pool.hpp"

using namespace std;
 
Log::Logger* Global_Logger = Log::Logger::getInstance();

int main() {
	
  Global_Logger->open("ServerLog");
  Global_Logger->setMaxSize(65536);

  // mt::ThreadPool ThreadPool;
  // ThreadPool.Init();

	struct sockaddr_in s_addr;
 
	int socketfd = 0;
	socklen_t length = 0;
	int acceptfd = 0;//客户端的文件描述符
	char ser_buf[66] = { 0 };
	int pid = 0;
	//初始化网络
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if (socketfd == -1)
	{
		perror(" socket error");
	}
	else
	{
		//确定使用那个协议族 ipv4
		s_addr.sin_family = AF_INET;
		//系统自动获取本机ip地址
		s_addr.sin_addr.s_addr = INADDR_ANY;
		//端口65535，10000以下是操作系统使用,自己定义需要10000以后
		s_addr.sin_port = htons(10800);
 
		length = sizeof(s_addr);
		//绑定ip地址和端口号int             
        //bind(int socket, const struct sockaddr *address, socklen_t address_len);
		if (::bind(socketfd,(sockaddr*)&s_addr,length) == -1)
		{
			perror(" bind error");
		}
		//监听这个地址和端口有没有客户端连接
 
		if (listen(socketfd,10) == -1)
		{
			perror(" listen error");
		}
		cout << "服务器网络通道准备好了" << endl;
 
		//死循环保证服务器长时间在线
		while (true)
		{
			cout << "等待客户端上线" << endl;
			//等待客户端上线,地址和端口号已经设置过了，所以为null，如果没有客户端访问则一直被动等待
			//返回值就表示那个客户端（给客户端发消息不需要知道客户端的ip地址）
			acceptfd = accept(socketfd, NULL, NULL);//阻塞函数
 
			pid = fork();
			if (pid == 0)
			{
				read(acceptfd, ser_buf, sizeof(ser_buf));//注意这里是acceptfd不是socketfd
				cout << "客户端连接成功! client_fd = " << acceptfd << "\nMessage: " << ser_buf << endl;
				bzero(ser_buf, sizeof(ser_buf));
			}

      char str[] = "Recived...";
      char* answerMessage;
      answerMessage = str;

      if (strcmp(ser_buf, "Hello") == 0){
        char hello[] = "Hello! I am uncle don!";
        answerMessage = hello;
      }
      write(acceptfd, answerMessage, sizeof(str));

      // 等待阻塞结束后断开
      // close(acceptfd);

      // 立刻断开
      // shutdown(acceptfd, SHUT_RD);  // 断开读取
      // shutdown(acceptfd, SHUT_WR);  // 断开写入
      shutdown(acceptfd, SHUT_RDWR);  // 断开读取及写入
		}
	}
	
 
  // ThreadPool.Release();
  Global_Logger->close();
	return 0;
}
 

