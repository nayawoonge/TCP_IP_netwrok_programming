#include "../../common.h"
#include <string.h>

#define SERVERPORT 9000
#define BUFSIZE	50

int main(int argc, char* argv[])
{
	int retval = -1;

	//	initialize winsock
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	//	create socket
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == listen_sock)
		err_quit("socket()");

	//	bind socket
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	// listen
	retval = listen(listen_sock, SOMAXCONN);
	if (SOCKET_ERROR == retval)
		err_quit("listen()");

	//	data communication
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1] = { 0, };

	while (1)
	{
		//	accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (INVALID_SOCKET == client_sock)
		{
			err_display("accept()");
			break;
		}

		//	print the client information
		char addr[INET_ADDRSTRLEN] = { 0, };
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP Server] Client IP = %s, port = %d\n", addr, ntohs(clientaddr.sin_port));

		//	send/recv data
		while (1)
		{
			//	recv
			retval = recv(client_sock, buf, BUFSIZE, MSG_WAITALL); //recv flag를 MSG_WAITALL로 줘서 버퍼크기 50까지 계속받음

			//	recv error
			if (SOCKET_ERROR == retval)
			{
				err_display("recv()");
				break;
			}

			//	no data received
			else if (0 == retval)
				break;

			//	print the received data
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

			//	closing condition
			retval = strncmp(buf, "99", sizeof(buf));
			if (0 == retval)
				break;

		}

		// 소켓 닫기
		closesocket(client_sock);
		printf("[TCP server] client close: IP = %s, port =%d\n", addr, ntohs(clientaddr.sin_port));
	}

	//	close socket
	closesocket(listen_sock);

	//	close winsock
	WSACleanup();

	return 0;
}
