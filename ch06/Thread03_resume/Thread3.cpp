#include <windows.h>
#include <stdio.h>

int sum = 0;

DWORD WINAPI MyThread(LPVOID arg)
{
	int num = (int)(long long)arg;
	for (int i = 1; i <= num; i++)
		sum += i;
	return 0;
}

int main(int argc, char* argv[])
{
	int num = 100;
	HANDLE hThread = CreateThread(NULL, 0, MyThread, (LPVOID)(long long)num, CREATE_SUSPENDED, NULL);
	//	CREATE_SUSPENDED : 스레드는 일시 중단된 상태로 만들어지고 
	//	ResumeThread 함수가 호출될 때까지 실행되지 않습니다.
	//	0	: 스레드는 만든 직후에 실행됩니다.
	
	
	//WaitForSingleObject(hThread, INFINITE);
	//Sleep(1);
	//SuspendThread(hThread);

	printf("스레드 실행 전. 계산 결과 = %d\n", sum);
	ResumeThread(hThread);
	WaitForSingleObject(hThread, INFINITE);
	printf("스레드 실행 후. 계산 결과 = %d\n", sum);
	CloseHandle(hThread);
	return 0;
}
