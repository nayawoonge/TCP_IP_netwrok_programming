#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThread(LPVOID arg)
{
	while (1);

	return 0;
}

int main(int argc, char* argv[])
{

	//	우선순위 값의 범위를 알려준다.
	printf("priority: %d ~ %d\n", THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);

	//	CPU 개수
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	int num = (int)si.dwNumberOfProcessors;
	printf("number of CPUs = %d\n", num);

	//	CPU 개수만큼 스레드를 생성
	for (int i = 0; i < num; i++)
	{
		HANDLE hThread = CreateThread(NULL, 0, MyThread, NULL, 0, NULL);
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		CloseHandle(hThread);
	}

	//	우선순위를 낮게 설정
	HANDLE hPriority = GetCurrentThread();
	SetThreadPriority(hPriority, THREAD_PRIORITY_BELOW_NORMAL);
	Sleep(1000);
	printf("executing the main thread (%d)\n", GetThreadPriority(hPriority));

	return 0;

}
