#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>

#define ITER	1000000
#ifndef MY_CS
//#define MY_CS
#endif

int num = 0;

#ifdef MY_CS
CRITICAL_SECTION cs;
#endif

DWORD WINAPI add_thread(LPVOID arg)
{
	for (int i = 0; i < ITER; i++)
	{
#ifdef MY_CS
		EnterCriticalSection(&cs);
#endif
		num += 2;
#ifdef MY_CS
		LeaveCriticalSection(&cs);
#endif
	}

	return 0;
}

DWORD WINAPI substract_thread(LPVOID arg)
{
	for (int i = 0; i < ITER; i++)
	{
#ifdef MY_CS
		EnterCriticalSection(&cs);
#endif
		num -= 2;
#ifdef MY_CS
		LeaveCriticalSection(&cs);
#endif
	}

	return 0;
}

int main(int argc, char* argv[])
{
	printf("before: num = %d\n", num);

#ifdef MY_CS
	InitializeCriticalSection(&cs);
#endif
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, add_thread, NULL, CREATE_SUSPENDED, NULL);
	hThread[1] = CreateThread(NULL, 0, substract_thread, NULL, CREATE_SUSPENDED, NULL);

	ResumeThread(hThread[0]);
	ResumeThread(hThread[1]);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

#ifdef MY_CS
	DeleteCriticalSection(&cs);
#endif

	printf("after: num = %d\n", num);

	return 0;
}