#include "../../common.h"

#define BUFFER_SIZE 1024
#define SERVER_PORT 9000

DWORD WINAPI MyThread(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    struct sockaddr_in clientaddr;
    char addr[INET_ADDRSTRLEN];
    int addrlen;
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

    char buffer[BUFFER_SIZE];
    int retval = -1;
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        // Ŭ���̾�Ʈ�κ��� �޽��� ����
        retval = recv(client_sock, buffer, sizeof(buffer), 0);
        if (retval == SOCKET_ERROR)
        {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
        {
            break;
        }
        // Ŭ���̾�Ʈ�� "00"�� �Է��ϸ� ä�� ����
        if (strcmp(buffer, "00") == 0)
        {
            printf("[ä�� ����]\n");
            break;
        }

        // ������ �޽��� ���
        printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buffer);

        // Ŭ���̾�Ʈ�κ��� ���� ���� ��û�� �޾��� �� ó��
        if (strcmp(buffer, "FILE") == 0)
        {
            // �����κ��� ���� ��� ����
            memset(buffer, 0, sizeof(buffer));
            retval = recv(client_sock, buffer, sizeof(buffer), 0);
            if (retval == SOCKET_ERROR)
            {
                err_display("recv()");
                break;
            }

            FILE* file = fopen(buffer, "rb");
            if (file == NULL)
            {
                printf("fopen() error: %s\n", buffer);
                break;
            }

            // ���� ũ�� ���
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            // ���� ũ�� ����
            sprintf(buffer, "%ld", file_size);
            retval = send(client_sock, buffer, sizeof(buffer), 0);
            if (retval == SOCKET_ERROR)
            {
                err_display("send()");
                fclose(file);
                break;
            }

            // ���� ������ ����
            long bytes_send = 0;
            int progress = 0;
            while (bytes_send < file_size)
            {
                memset(buffer, 0, sizeof(buffer));

                // ���Ͽ��� ���� ũ�⸸ŭ �б�
                int bytes_read = fread(buffer, sizeof(char), sizeof(buffer), file);
                if (bytes_read <= 0)
                {
                    printf("fread() error\n");
                    fclose(file);
                    break;
                }

                // ���� ������ ����
                int bytes_send_now = send(client_sock, buffer, bytes_read, 0);
                if (bytes_send_now == SOCKET_ERROR)
                {
                    err_display("send()");
                    fclose(file);
                    break;
                }

                // ���� ���� ��Ȳ ���
                bytes_send += bytes_send_now;
                int new_progress = (int)((bytes_send * 100) / file_size);
                while (new_progress <= 100)
                {
                    if (new_progress == 100)
                        break;
                    printf("File ���� ���� ��Ȳ : %d%%\n", new_progress);
                }
            }

            fclose(file);
            printf("File transmission completed.\n");
        }
    }

    // Ŭ���̾�Ʈ ���� ����
    closesocket(client_sock);
    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
    return 0;
}

int main()
{
    int retval = -1;
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup() error.\n");
        return 1;
    }

    SOCKET listen_sock;
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock == INVALID_SOCKET)
    {
        err_quit("socket()");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVER_PORT);

    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        err_quit("bind()");
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
    {
        err_quit("listen()");
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    printf("Server running! \n");

    while (1)
    {
        SOCKET client_sock;
        struct sockaddr_in clientaddr;
        int addrlen;
        addrlen = sizeof(clientaddr);

        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET)
        {
            err_display("accept()");
            closesocket(listen_sock);
            WSACleanup();
            return 1;
        }

        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));

        // Ŭ���̾�Ʈ ������ ����
        HANDLE hThread1 = CreateThread(NULL, 0, MyThread, (LPVOID)client_sock, 0, NULL);
        if (hThread1 == NULL)
        {
            closesocket(client_sock);
        }
        else
        {
            // ������ �ڵ��� ���� (������� �ڵ����� ����� ������ �����)
            CloseHandle(hThread1);
        }
    }

    //  ���� �ݱ�
    closesocket(listen_sock);
    //  ���� ����
    WSACleanup();

    return 0;
}



