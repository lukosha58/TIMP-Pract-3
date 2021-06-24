#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void errHandler (const char * why, const int exitCode = 1)
{
    std::cerr << why << std::endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET;
    selfAddr->sin_port = 0;
    selfAddr->sin_addr.s_addr = 0;

    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;
    remoteAddr->sin_port = htons(7); //port
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.69"); //ip

    char *buf = new char [256];
    strcpy(buf, "ПГУ- выбор лучших!\n");
    int msgLen = strlen(buf);

    int mySocket = socket(AF_INET, SOCK_DGRAM, 0); //udp
    if (mySocket == -1) {
        errHandler("Error open socket", 11);
    }

    int rc = bind(mySocket, (const sockaddr*) selfAddr, sizeof (sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error bind socket with local address", 12);
    }

    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error connect socket with remote server", 13);
    }

    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        errHandler("Error send message", 14);
    }
    std::cout << "We send: " << buf << std::endl;

    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) {
        close(mySocket);
        errHandler("Error recevie answer", 15);
    }
    buf[rc]= '\0';
    std::cout<<"We receive: "<< buf << std::endl;

    close(mySocket);

    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}