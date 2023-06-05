#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThread(LPVOID arg)
{
	while (1);

	return 0;
}

int main(int argc, char* argv[])
{

	//	�켱���� ���� ������ �˷��ش�.
	printf("priority: %d ~ %d\n", THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);

	//	CPU ����
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	int num = (int)si.dwNumberOfProcessors;
	printf("number of CPUs = %d\n", num);

	//	CPU ������ŭ �����带 ����
	for (int i = 0; i < num; i++)
	{
		HANDLE hThread = CreateThread(NULL, 0, MyThread, NULL, 0, NULL);
		SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		CloseHandle(hThread);
	}

	//	�켱������ ���� ����
	HANDLE hPriority = GetCurrentThread();
	SetThreadPriority(hPriority, THREAD_PRIORITY_BELOW_NORMAL);
	Sleep(1000);
	printf("executing the main thread (%d)\n", GetThreadPriority(hPriority));

	return 0;

}
