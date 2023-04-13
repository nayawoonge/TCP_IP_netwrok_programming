#include "../../common.h"
#include <string.h>


char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    50

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
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock)
		err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval)
		err_quit("connect()");

	//	variables declaration
	char buf[BUFSIZE + 1] = { 0, };
	const char* testdata[] = {
		"안녕하세요",
		"반가워요",
		"오늘따라 할 이야기가 많을 것 같네요",
		"저도 그렇네요"
	};
	int len = 0;

	for (int i = 0; i < 4; i++)
	{
		len = (int)strlen(testdata[i]);
		//	서버에 사이즈를 정수로 보냄
		//	첫 데이터 10byte를 정수 10으로 보냄
		strncpy(buf, testdata[i], len);

		retval = send(sock, (char*)&len, sizeof(int), 0);
		if (SOCKET_ERROR == retval)
		{
			err_display("send()");
			break;
		}

		retval = send(sock, buf, len, 0);
		if (SOCKET_ERROR == retval)
		{
			err_display("send()");
			break;
		}
		printf("[TCP Client] %d bytes has been sent.\n", retval);
	}

	//	close socket
	closesocket(sock);

	//	close socket
	WSACleanup();

	return 0;
}

