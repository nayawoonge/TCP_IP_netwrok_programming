#include "../../common.h"
#include <string.h>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 50

int main(int argc, char* argv[])
{
	int retval;

	//	명령행 인수가 있으면 IP주소로 사용
	//	get IP address from users
	if (argc > 1)
		SERVERIP = argv[1];

	//	윈속 초기화
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	//	소켓 생성
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