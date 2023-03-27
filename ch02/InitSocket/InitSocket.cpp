#include "../../common.h"

int main(int argc, char* argv[])
{
	//원속 초기화
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;
	printf("[알림] 원속 초기화 성공\n");

	//소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");
	printf("[알림] 소켓 생성 성공\n");

	//소켓 닫기
	closesocket(sock);

	//원속 종료
	WSACleanup();
	return 0;
}