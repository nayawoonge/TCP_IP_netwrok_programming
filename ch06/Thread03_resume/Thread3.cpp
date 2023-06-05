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
	//	CREATE_SUSPENDED : ������� �Ͻ� �ߴܵ� ���·� ��������� 
	//	ResumeThread �Լ��� ȣ��� ������ ������� �ʽ��ϴ�.
	//	0	: ������� ���� ���Ŀ� ����˴ϴ�.
	
	
	//WaitForSingleObject(hThread, INFINITE);
	//Sleep(1);
	//SuspendThread(hThread);

	printf("������ ���� ��. ��� ��� = %d\n", sum);
	ResumeThread(hThread);
	WaitForSingleObject(hThread, INFINITE);
	printf("������ ���� ��. ��� ��� = %d\n", sum);
	CloseHandle(hThread);
	return 0;
}
