#include "../../common.h"

int main(int argc, char* argv[])
{
	//원속 초기화
	WSADATA wsa;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsa) )
		return 1;

	u_short x1 = 0x1234;		//short 2byte
	u_long y1 = 0x1234567;		//long 4byte
	u_short x2;			//바뀐 값을 넣을 변수
	u_long y2;			//바뀐 값을 넣을 변수

	//호스트 바이트 -> 네트워크 바이트
	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	printf("%#x -> %#x\n", x1, x2 = htons(x1));
	printf("%#x -> %#x\n", y1, y2 = htonl(y1));

	// 네트워크 바이트 -> 호스트 바이트
	printf("\n[네트워크 바이트 -> 호스트 바이트]\n");
	printf("%#x -> %#x\n", x2, ntohs(x2));	//hton 함수에 short인 s를 붙인 htons 함수
	printf("%#x -> %#x\n", y2, ntohl(y2));	//hton 함수에 long인 l을 붙인 htonl 함수

	// 잘못된 사용 예
	printf("\n[잘못된 사용 예]\n");
	printf("%#x -> %#x\n", x1, htonl(x1)); //short를 htonl로 찍으면 빈 byte가 0으로 자릿수 채워짐
	printf("%#x -> %#x\n", y1, htons(y1));	//long을 htons로 찍으면 byte가 짤림

	// 윈속 종료
	WSACleanup();
	return 0;
}