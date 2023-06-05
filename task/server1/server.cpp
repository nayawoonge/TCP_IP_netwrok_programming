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

        // 클라이언트로부터 메시지 수신
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
        // 클라이언트가 "00"을 입력하면 채팅 종료
        if (strcmp(buffer, "00") == 0)
        {
            printf("[채팅 종료]\n");
            break;
        }

        // 수신한 메시지 출력
        printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buffer);

        // 클라이언트로부터 파일 전송 요청을 받았을 때 처리
        if (strcmp(buffer, "FILE") == 0)
        {
            // 서버로부터 파일 경로 수신
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

            // 파일 크기 계산
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            // 파일 크기 전송
            sprintf(buffer, "%ld", file_size);
            retval = send(client_sock, buffer, sizeof(buffer), 0);
            if (retval == SOCKET_ERROR)
            {
                err_display("send()");
                fclose(file);
                break;
            }

            // 파일 데이터 전송
            long bytes_send = 0;
            int progress = 0;
            while (bytes_send < file_size)
            {
                memset(buffer, 0, sizeof(buffer));

                // 파일에서 버퍼 크기만큼 읽기
                int bytes_read = fread(buffer, sizeof(char), sizeof(buffer), file);
                if (bytes_read <= 0)
                {
                    printf("fread() error\n");
                    fclose(file);
                    break;
                }

                // 읽은 데이터 전송
                int bytes_send_now = send(client_sock, buffer, bytes_read, 0);
                if (bytes_send_now == SOCKET_ERROR)
                {
                    err_display("send()");
                    fclose(file);
                    break;
                }

                // 전송 진행 상황 출력
                bytes_send += bytes_send_now;
                int new_progress = (int)((bytes_send * 100) / file_size);
                while (new_progress <= 100)
                {
                    if (new_progress == 100)
                        break;
                    printf("File 전송 진행 상황 : %d%%\n", new_progress);
                }
            }

            fclose(file);
            printf("File transmission completed.\n");
        }
    }

    // 클라이언트 소켓 종료
    closesocket(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));
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
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr, ntohs(clientaddr.sin_port));

        // 클라이언트 스레드 생성
        HANDLE hThread1 = CreateThread(NULL, 0, MyThread, (LPVOID)client_sock, 0, NULL);
        if (hThread1 == NULL)
        {
            closesocket(client_sock);
        }
        else
        {
            // 스레드 핸들을 닫음 (스레드는 자동으로 종료될 때까지 실행됨)
            CloseHandle(hThread1);
        }
    }

    //  소켓 닫기
    closesocket(listen_sock);
    //  윈속 종료
    WSACleanup();

    return 0;
}



