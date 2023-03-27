#include "../../common.h"

int main(int argc, char* argv[])
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsa) )
		return 1;

	u_short x1 = 0x1234;		//short 2byte
	u_long y1 = 0x1234567;		//long 4byte
	u_short x2;			//�ٲ� ���� ���� ����
	u_long y2;			//�ٲ� ���� ���� ����

	//ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ
	printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");
	printf("%#x -> %#x\n", x1, x2 = htons(x1));
	printf("%#x -> %#x\n", y1, y2 = htonl(y1));

	// ��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ
	printf("\n[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
	printf("%#x -> %#x\n", x2, ntohs(x2));	//hton �Լ��� short�� s�� ���� htons �Լ�
	printf("%#x -> %#x\n", y2, ntohl(y2));	//hton �Լ��� long�� l�� ���� htonl �Լ�

	// �߸��� ��� ��
	printf("\n[�߸��� ��� ��]\n");
	printf("%#x -> %#x\n", x1, htonl(x1)); //short�� htonl�� ������ �� byte�� 0���� �ڸ��� ä����
	printf("%#x -> %#x\n", y1, htons(y1));	//long�� htons�� ������ byte�� ©��

	// ���� ����
	WSACleanup();
	return 0;
}