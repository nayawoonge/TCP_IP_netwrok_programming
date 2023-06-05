#include "../../common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char* argv[])
{
	int retval = -1;

	//	get IP address from users
	if (argc > 1)
		SERVERIP = argv[1];

	//	initialize winsock
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	//	create socket
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sock)
		err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	//	variables declaration
	//	변수 초기화
	struct sockaddr_in peeraddr;
	int addrlen = 0;
	char buf[BUFSIZE + 1] = { 0, };
	int len = 0;

	//	data communication
	while (1)
	{
		//	get data to send
		printf("\n[data to send] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		//	remove null terminate character
		len = (int)strlen(buf);
		if ('\n' == buf[len - 1])
			buf[len - 1] = '\0';
		//	마지막 엔터 \n 문자를 \0 null로 대체

		//	no data to send
		if (0 == strlen(buf))
			break;

		//	send data
		retval = sendto(sock, buf, (int)strlen(buf), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		//	       sock, 보낼버퍼, 버퍼크기, 옵션,           보낼 서버 주소 정보,    서버 주소 크기
		if (SOCKET_ERROR == retval)
		{
			err_display("sendto()");
			break;
		}
		printf("[UDP client] %d bytes sent.\n", retval);

		//	receive data
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr*)&peeraddr, &addrlen);
		if (SOCKET_ERROR == retval)
		{
			err_display("recvfrom()");
			break;
		}

		//	print the received data
		if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr)))
		{
			printf("[ERROR] invalide data\n");
			break;
		}

		buf[retval] = '\0';
		printf("[UDP client] receive %d bytes.\n", retval);
		printf("[received data] %s\n", buf);
	}

	//	close socket
	closesocket(sock);

	//	close socket
	WSACleanup();

	return 0;
}

