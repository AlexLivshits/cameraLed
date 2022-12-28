//#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <functional>
#include <iostream>
#include <map>
#include <cstring>
struct camStruct
{
public:
    bool GetState() {return state;}
    void SetState(bool _state) {state=_state;}

    std::string GetColor() {return color;}
    void SetColor(std::string _color) {color=_color;}


    int GetFreq() {return freq;}
    void SetFreq(int _freq) {freq=_freq;}

private:
    bool state = false;
    std::string color = "red";
    int freq = 0;
};

int main()
{
    printf("server\r\n");
    camStruct cam;
    std::map<std::string,std::function<std::string(std::string)>> commands;
    commands["get-led-state"]=[&cam](std::string in){return cam.GetState()?"OK on":"OK off";};
    commands["set-led-state"]=[&cam](std::string in){
        if(in=="on") {cam.SetState(true);  return "OK";}
        if(in=="off") {cam.SetState(false); return "OK";}
        else return "FAULT";
    };

    char buf[2048];
    int bytes_read;
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        printf("can not init socket\r\n");
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("can not bind\r\n");
        return 0;
    }
    listen(listener, 1);
    int sock;
    while (true)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            printf("can not accept\r\n");
            return 0;
        }
        //memset(buf,0,2048);
        bytes_read = recv(sock, buf, 2048, 0);
        printf("Получено %d bytes\tСообщение: %s\n", bytes_read, buf);
        std::string commandString(buf);
        if(commandString.length()==0)
        {
            char buff_send[2048] = "FAULT";
            send(sock, buff_send, sizeof(buff_send), 0);
            continue;
        }
        auto firstSpace = commandString.find(' ');
        printf("%ld\r\n", firstSpace);
        std::string command = commandString.substr(0,firstSpace);
        std::string argument;
        if (firstSpace == -1)
        {
            argument = "";
        }
        else
        {
            auto secondSpace = commandString.find(' ', firstSpace+1);
            printf("secondSpace=%ld\r\n",secondSpace);
            if (secondSpace==-1)
                argument = commandString.substr(firstSpace+1, commandString.length());
            else
                argument = commandString.substr(firstSpace+1, secondSpace-firstSpace-1);
        }

        printf("commandString=%s\tcommand=\"%s\"\targument=\"%s\"\r\n",commandString.c_str(),command.c_str(),argument.c_str());
        //continue;
        if (commands.find(command)!=commands.end())
        {
            std::string returnStr = commands[command](argument);
            const char* buff_send = returnStr.c_str();
            send(sock, buff_send, sizeof(buff_send), 0);
        }
        else
        {
            std::string returnStr = "FAULT";
            const char* buff_send = returnStr.c_str();
            send(sock, buff_send, sizeof(buff_send), 0);
        }
        close(sock);

    }
    printf("Hello world\r\n");
    return 0;
}
