#include "../../common.h"

#define MULTICASTIP	"235.7.8.9"
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

	DWORD ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl));
	if (SOCKET_ERROR == retval)
		err_quit("setsockopt(IPPROTO_IP, IP_MULTICAST_TTL)");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, MULTICASTIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	//	variables declaration
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

		//	no data to send
		if (0 == strlen(buf))
			break;

		//	send data
		retval = sendto(sock, buf, (int)strlen(buf), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (SOCKET_ERROR == retval)
		{
			err_display("sendto()");
			break;
		}
		printf("[UDP client] %d bytes sent.\n", retval);
	}

	//	close socket
	closesocket(sock);

	//	close socket
	WSACleanup();

	return 0;
}

