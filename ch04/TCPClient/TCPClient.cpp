#include "../../common.h"

char* SERVERIP = (char*)"127.0.0.1";

#define SERVERPORT 9000
#define BUFSIZE 512

int main(int argc, char* argv[])
{
	int retval;

	//����� �μ��� ������ IP �ּҷ� ���
	if (argc > 1)
		SERVERIP = argv[1];

	//���� �ʱ�ȭ
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	//���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");

	//connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("connect()");

	//������ ��ſ� ����� ����
	char buf[BUFSIZE + 1] = { 0, };
	int len = 0;

	//������ ������ ���
	while (1) 
	{
		//������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		//'\n' ���� ����
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		
		//no data to send
		if (0 == strlen(buf))
			break;

		//������ ������
		retval = send(sock, buf, (int)strlen(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		// no data received
		else if (0 == retval)
			break;
		
		//���� ������ ���
		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);

		//closing condition
		retval = strncmp(buf, "99", sizeof(buf));
		if (0 == retval)
			break;
	}

	//���� �ݱ�
	closesocket(sock);

	//��������
	WSACleanup();
	return 0;
}