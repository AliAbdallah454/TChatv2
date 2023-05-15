#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include <iostream>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "myTime.hpp"

using namespace std;

mutex clientsSocketsMutex;
mutex fileMutex;

void handleConnectionLost(int clientSocket, vector<int> &clientSockets){

    clientsSocketsMutex.lock();
    clientSockets.erase(remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
    clientsSocketsMutex.unlock();
    
    close(clientSocket);
    cout << "Connection lost" << endl;
}

void broadcast(int clientSocket, int serverSocket, vector<int> &clientsSockets){

    ofstream outputFile("messages.txt", ios::app);

    char buffer[256];
        
    while(true){

        size_t s = recv(clientSocket, buffer, sizeof(buffer), 0);

        if(s == 0){
            handleConnectionLost(clientSocket, clientsSockets);
            break;
        }
        size_t bufferLength = strlen(buffer);

        if(outputFile.is_open()){
            fileMutex.lock();
            outputFile << getTime() << " -- " << string(buffer, bufferLength) << endl;
            fileMutex.unlock();
        }
        cout << string(buffer, bufferLength) << endl;

        clientsSocketsMutex.lock();
        for(int i = 0; i < clientsSockets.size(); i++){
            if(clientsSockets[i] != 0){
                send(clientsSockets[i], buffer, bufferLength, 0);
            }
        }
        clientsSocketsMutex.unlock();

        memset(buffer, 0, sizeof(buffer));

    }
    
}

void acceptClients(vector<int> &clientsSockets, int serverSocket){

    while(true){
        int clientSocket = accept(serverSocket, NULL, NULL);

        clientsSocketsMutex.lock();
        clientsSockets.push_back(clientSocket);
        clientsSocketsMutex.unlock();

        thread broadcastThread(broadcast, clientSocket, serverSocket, ref(clientsSockets));
        broadcastThread.detach();
    }

}

int main(){

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(serverSocket,(struct sockaddr *) &serverAddress, sizeof(serverAddress));
    listen(serverSocket, 10);

    vector<int> clientsSockets;

    thread acceptClientsThread(acceptClients ,ref(clientsSockets), serverSocket);
    acceptClientsThread.join();

    return 0;

}