#include "../../common.h"
#include <string.h>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 50

int main(int argc, char* argv[])
{
	int retval;

	//	����� �μ��� ������ IP�ּҷ� ���
	//	get IP address from users
	if (argc > 1)
		SERVERIP = argv[1];

	//	���� �ʱ�ȭ
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	//	���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock)
		err_quit("socket()");

	// connet()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));)
	if (SOCKET_ERROR == retval)
		err_quit("connect()");

	//	buf


}