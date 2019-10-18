#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <algorithm>
#include <iostream>

typedef  std::vector<struct epoll_event>EventList;

#define   ERR_EXIT(m) \
		  do \
		  {\
			perror(m); \
			exit(EXIT_FAILURE);\
		  }while (0)

int main(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	int idlefd = open("dev/null", O_RDONLY | O_CLOEXEC);
	int listenfd;
	if ((listenfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 

	int on;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) // 允许重用本地地址和端口　
	{
		ERR_EXIT("setsockopt");
	}
	if (bind(listenfd, (struct  sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		ERR_EXIT("bind");
	}
	if (listen(listenfd, SOMAXCONN)<0)
	{
		ERR_EXIT("listen");
	}
	std::vector<int>clients;
	int epollfd;
	epollfd = epoll_create1(EPOLL_CLOEXEC);  //创建epollfd
	struct epoll_event event;
	event.data.fd = listenfd;
	event.events = EPOLLIN;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event); //加入监听套接字

	EventList events(16);
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int connfd;

	int nready;

	while (1)
	{
		nready = epoll_wait(epollfd, &*events.begin(), static_cast<int>(events.size()), -1); //等待套接字进入就绪队列
		if (nready == -1)
		{
			if (errno ==  EINTR)//如果错误码为EINTR则重新调用系统调用
			{
				continue;
			}
			ERR_EXIT("epoll_wait");
		}
		if (nready == 0) //没有事件发生，继续
		{
			continue;
		}
		if ((size_t)nready == events.size())
		{
			events.resize(events.size() * 2);
		}

		for (int i = 0; i < nready; i++)
		{
			if (events[i].data.fd ==  listenfd)
			{
				peerlen = sizeof(peeraddr);
				connfd = ::accept4(listenfd, (struct sockaddr*)&servaddr, &peerlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
				if (connfd == -1)
				{
					if (errno == EMFILE)
					{
						close(idlefd);
						idlefd = accept(listenfd, NULL, NULL);
						close(idlefd);
						idlefd = open("/dev/null", O_RDONLY | O_CLOEXEC);
						continue;
					}
					else {
						ERR_EXIT("accept4");
					}
				}
				std::cout << "ip=" << inet_ntoa(peeraddr.sin_addr) << "port=" << ntohs(peeraddr.sin_port) << std::endl;
				clients.push_back(connfd);
				event.data.fd = connfd;
				event.events = EPOLLIN;
				epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &event);
			}
			else if (events[i].events&EPOLLIN) //可读事件发生
			{
				connfd = events[i].data.fd;
				if (connfd < 0)
				{
					continue;
				}
				char buf[1024] = { 0 };
				int ret = read(connfd, buf, 1024);
				if (ret == -1)
				{
					ERR_EXIT("read");
				}
				if (ret == 0)  //读取数据为0
				{
					std::cout << "client close" << std::endl;
					close(connfd); //关闭链接
					event = events[i];
					epoll_ctl(epollfd, EPOLL_CTL_DEL, connfd, &event); //剔除掉已经关闭连接的connfd
					continue;
				}
				std::cout << buf;
				write(connfd, buf, strlen(buf));//回写
			}

		}
	}
	return 0;
}
