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
    //making a string from argv
    if (argc<=1)
    {
        printf("Wrong args\r\n");
        return 0;
    }
    for( auto count = 1; count < argc; count++ )
    {
        message+=argv[count]; message+=" ";
    }
    message+='\n';
    //
    printf("commandString = %s\r\n", message.c_str());
    //opening connection
    int sock;
    struct sockaddr_in addr;
    struct hostent *hostinfo;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("error in socket\r\n");
        return 0;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("error in connect\r\n");
        return 0;
    }
    //sending message
    send(sock, message.c_str(), message.length(), 0);
    printf("waiting unswer...\r\n");
    //waiting answer
    char readBuf[2048];
    int bytes_read = recv(sock, readBuf, sizeof(readBuf), 0);
    //printf("count %d bytes\tmess: %s\n", bytes_read, readBuf);
    printf("answer = %s", readBuf);
    return 0;
}
