#include "../../common.h"
#include <string.h>


#define SERVERPORT 9000
#define BUFSIZE    512

//	thread
DWORD WINAPI ProcessClient(LPVOID arg)
{
	//	인자로 전달된 소켓을 SOCKET 타입으로 저장
	SOCKET client_sock = (SOCKET)arg;
	int retval = 0;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN] = { 0, };
	int addrlen = 0;
	char buf[BUFSIZE + 1] = { 0, };

	//	getpeername()으로 클라이언트 주소 정보를 획득
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1)
	{
		//	recv
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (SOCKET_ERROR == retval)
		{
			err_display("recv()");
			break;
		}
		else if (0 == retval)
			break;

		buf[retval] = '\0';
		printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		//	send
		retval = send(client_sock, buf, retval, 0);
		if (SOCKET_ERROR == retval)
		{
			err_display("send()");
			break;
		}
	}

	closesocket(client_sock);
	printf("[TCP server] client shutdown: IP=%s, Port=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;
}



int main(int argc, char* argv[])
{
	int retval = -1;

	//	initialize winsock
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	printf("[TCP Server] WSAStartup Success.\n");

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
	printf("[TCP Server] bind() Success.\n");

	// listen
	retval = listen(listen_sock, SOMAXCONN);
	if (SOCKET_ERROR == retval)
		err_quit("listen()");
	printf("[TCP Server] listen() Success.\n");

	//	data communication
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread = NULL;
	//	char buf[BUFSIZE + 1];


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

		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (NULL == hThread)
			closesocket(client_sock);
		else
			CloseHandle(hThread);
		
		
		/*	멀티스레드가 아닌 그냥 TCP 서버일 때
				//	send/recv data
				while (1)
				{
					//	recv
					retval = recv(client_sock, buf, BUFSIZE, 0);

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

					//	send data
					retval = send(client_sock, buf, retval, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}


					//	closing condition
					retval = strncmp(buf, "99", sizeof(buf));
					if (0 == retval)
						break;

				}

				// 소켓 닫기
				closesocket(client_sock);
				printf("[TCP server] client close: IP = %s, port =%d\n", addr, ntohs(clientaddr.sin_port));
		*/
	}

	//	close socket
	closesocket(listen_sock);

	//	close winsock
	WSACleanup();

	return 0;
}
