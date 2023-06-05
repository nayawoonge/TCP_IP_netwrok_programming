#include "../../common.h"

#define BUFFER_SIZE 1024
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000

void send_file(SOCKET client_sock, const char* file_path)
{
    char buffer[BUFFER_SIZE];

    // ���� ���� ��û ����
    sprintf(buffer, "FILE");
    if (send(client_sock, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
    {
        printf("Error in sending file request to server.\n");
        return;
    }

    // ���� ��� ����
    sprintf(buffer, "%s", file_path);
    if (send(client_sock, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
    {
        printf("Error in sending file path to server.\n");
        return;
    }

    // ���� ũ�� ����
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_sock, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
    {
        printf("Error in receiving file size from server.\n");
        return;
    }
    long file_size = atol(buffer);
    if (file_size <= 0)
    {
        printf("Invalid file size received from server.\n");
        return;
    }

    // ���� ������ ����
    FILE* file = fopen(file_path, "wb");
    if (file == NULL)
    {
        printf("Failed to create file: %s\n", file_path);
        return;
    }

    long bytes_received = 0;
    int progress = 0;
    while (bytes_received < file_size)
    {
        memset(buffer, 0, sizeof(buffer));

        // �����κ��� ������ ����
        int bytes_received_now = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received_now == SOCKET_ERROR || bytes_received_now <= 0)
        {
            printf("Error in receiving file data from server.\n");
            fclose(file);
            return;
        }

        // ������ ������ ���Ͽ� ����
        fwrite(buffer, sizeof(char), bytes_received_now, file);

        // ���� ���� ��Ȳ ���
        bytes_received += bytes_received_now;
        int new_progress = (int)((bytes_received * 100) / file_size);
        if (new_progress > progress)
        {
            progress = new_progress;
            printf("File transmission progress: %d%%\n", progress);
        }
    }

    fclose(file);
    printf("File transmission completed.\n");
}


void send_chat_message(SOCKET client_sock, const char* message)
{
    if (send(client_sock, message, strlen(message), 0) == SOCKET_ERROR)
    {
        printf("Error in sending chat message to server.\n");
    }
}

int main(int argc, char* argv[])
{
    int retval = -1;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup() error\n");
        return 1;
    }

    //  ���� ����
    SOCKET client_sock;
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == INVALID_SOCKET)
    {
        err_quit("socket()");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(client_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        err_quit("connect()");
        closesocket(client_sock);
        WSACleanup();
        return 1;
    }

    char file_path[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    while (1)
    {
        // ����� �Է� �ޱ�
        printf("\n[#1 ���� ����, #2 ä��] ");
        int choice;
        scanf("%d", &choice);

        if (choice == 00)
        {
            // Ŭ���̾�Ʈ ����
            send_chat_message(client_sock, "00");
            break;
        }
        else if (choice == 1)
        {
            // ���� ����
            printf("[���� ���] ");
            scanf("%s", file_path);
            send_file(client_sock, file_path);
        }
        else if (choice == 2)
        {
            // ä�� �޽��� ����
            printf("[���� ������] ");
            fflush(stdin);
            fgets(message, BUFFER_SIZE, stdin);
            message[strcspn(message, "\n")] = '\0';
            send_chat_message(client_sock, message);
        }
        else
        {
            printf("Invalid choice\n");
        }
    }

    closesocket(client_sock);
    WSACleanup();

    return 0;
}
