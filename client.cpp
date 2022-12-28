#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
int main( int argc, char *argv[], char *envp[] )
{
    printf("client\r\n");
    std::string message;
    if (argc<=1)
    {
        printf("Wrong args");
        return 0;
    }
    for( auto count = 1; count < argc; count++ )
    {
        message+=argv[count]; message+=" ";
    }
    //message+=argv[argc-1];

         //cout << "  argv[" << count << "]   "
           //     << argv[count] << "\n";

    printf("commandString = %s\r\n", message.c_str());

    //char message[2048] = "set-led-state on";
    //char buf[2048];
    int sock;
    struct sockaddr_in addr;
    struct hostent *hostinfo;
    //int port = 12345;
    //hostinfo = "127.0.0.1";
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        exit(2);
    }
    const char* buf = message.c_str();
    printf("buf = %s, sizeof(buf) = %ld\r\n",buf, sizeof(buf));
    send(sock, message.c_str(), message.length(), 0);
    printf("Ожидание сообщения\n");
    char readBuf[2048];
    int bytes_read = recv(sock, readBuf, sizeof(readBuf), 0);
    printf("Получено %d bytes\tСообщение: %s\n", bytes_read, readBuf);
    return 0;
}
