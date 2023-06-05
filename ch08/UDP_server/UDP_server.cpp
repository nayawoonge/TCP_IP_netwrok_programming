#include "../../common.h"


#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char* argv[])
{
	int retval = -1;

	//	initialize winsock
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	//	create socket
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sock)
		err_quit("socket()");

	//	bind socket
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	printf("I'm running!!!\n");

	//	data communication
	struct sockaddr_in clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1)
	{
		//	accept()
		addrlen = sizeof(clientaddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr*)&clientaddr, &addrlen);
		//	      소켓, 받을 데이터 버퍼, 버퍼크기, 옵션, 클라이언트 주소 정보, 주소길이
		//retval = 수신한 데이터 길이

		//	recv error
		if (SOCKET_ERROR == retval)
		{
			err_display("recvfrom()");
			break;
		}

		//	print the received data
		buf[retval] = '\0';
		char addr[INET_ADDRSTRLEN] = { 0, };
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("[UDP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		//	send data
		retval = sendto(sock, buf, retval, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
		if (SOCKET_ERROR == retval)
		{
			err_display("sendto()");
			break;
		}
	}

	//	close socket
	closesocket(sock);

	//	close winsock
	WSACleanup();

	return 0;
}
