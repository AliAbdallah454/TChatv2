#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::strcat;
using std::strcspn;

int configSocket(){

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connection_status = connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    return clientSocket;
    
}

void sendMessage(int socket, char *message, char *name){

    char messageWithName[256] = {0};
    message[strcspn(message, "\n")] = '\0';

    strcat(messageWithName, name);
    strcat(messageWithName, " >> ");
    strcat(messageWithName, message);

    size_t messageWithNameSize = strlen(messageWithName);
    send(socket, messageWithName, messageWithNameSize, 0);

}