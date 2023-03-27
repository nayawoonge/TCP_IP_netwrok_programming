#include "../../common.h"

int main(int argc, char* argv[])
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;
	printf("[�˸�] ���� �ʱ�ȭ ����\n");

	//���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");
	printf("[�˸�] ���� ���� ����\n");

	//���� �ݱ�
	closesocket(sock);

	//���� ����
	WSACleanup();
	return 0;
}