#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

int main()
{
    WSAData wsaData;
    int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (Result < 0)
    {
        cout << "Error : " << GetLastError() << '\n';
        exit(-1);
    }

    SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket == INVALID_SOCKET)
    {
        cout << "Error : " << GetLastError() << '\n';
        exit(-1);
    }

    SOCKADDR_IN ServerAddr;
    memset(&ServerAddr, 0, sizeof(SOCKADDR_IN));
    ServerAddr.sin_family = PF_INET;
    ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    ServerAddr.sin_port = htons(23500);

    int status = bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    if (status == SOCKET_ERROR)
    {
        cout << "Error : " << GetLastError() << '\n';
        exit(-1);
    }

    status = listen(ServerSocket, 5);
    if (status == SOCKET_ERROR)
    {
        cout << "Error : " << GetLastError() << '\n';
        exit(-1);
    }

    SOCKADDR_IN ClientAddr;
    memset(&ClientAddr, 0, sizeof(SOCKADDR_IN));
    int ClientLength = sizeof(ClientAddr);

    SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClientLength);
    if (ClientSocket == INVALID_SOCKET)
    {
        cout << "Error : " << GetLastError() << '\n';
        exit(-1);
    }

    while (1)
    {
        char Buffer[1024] = { 0 };

        int RecvBytes = recv(ClientSocket, Buffer, sizeof(Buffer), 0);
        if (RecvBytes <= 0)
        {
            cout << "Error : " << GetLastError() << '\n';
            exit(-1);
        }

        cout << "Client: " << Buffer << '\n';

        string Message;
        cout << "Enter reply: ";
        getline(cin, Message); // 개행 포함 입력 받기

        int SendBytes = send(ClientSocket, Message.c_str(), Message.size(), 0);
        if (SendBytes <= 0)
        {
            cout << "Error : " << GetLastError() << '\n';
            exit(-1);
        }
    }

    closesocket(ClientSocket);
    closesocket(ServerSocket);
    WSACleanup();
    return 0;
}
