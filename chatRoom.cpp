#include <ncurses.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <thread>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client.hpp"

using namespace std;

void renderInputWindow(WINDOW *inputWindow, int inputWindowHeight){
    werase(inputWindow);
    box(inputWindow, 0, 0);
    wmove(inputWindow, (int)(inputWindowHeight / 2), 1);
    wprintw(inputWindow, ">> ");
    wrefresh(inputWindow);
}

void receiveAndPrint(int socket, WINDOW *window){

    int y = 1;
    char message[256] = {0};
    while(true){

        size_t length = recv(socket, message, sizeof(message), 0);

        if(length == 0) break;

        wmove(window, y++, 1);
        wprintw(window, "%s", message);
        wrefresh(window);

        memset(message, 0, sizeof(message));

    }

}

void displayName(WINDOW *window, char *name){

    wmove(window, 1, 11);
    wprintw(window, "Name : %s", name);
    wrefresh(window);

}

int main(){

    initscr();

    // windows Initialization

    int maxX = 0;
    int maxY = 0;
    getmaxyx(stdscr, maxY, maxX);

    int messageWindowHeight = 0.8 * maxY;
    int messageWindowWidth = 0.7 * maxX;
    WINDOW *messagesWindow = newwin(messageWindowHeight, messageWindowWidth, 0, 0);\

    int inputWindowHeight = 0.2 * maxY;
    int inputWindowWidth = 1 * maxX;
    WINDOW *inputWindow = newwin(inputWindowHeight, inputWindowWidth, messageWindowHeight, 0);

    int sideWindowHeight = 0.8 * maxY;
    int sideWindowWidth = 0.3 * maxX;
    WINDOW *sideWindow = newwin(sideWindowHeight, sideWindowWidth, 0, messageWindowWidth);

    refresh();
    box(messagesWindow, 0, 0);
    box(inputWindow, 0, 0);
    box(sideWindow, 0, 0);
    wrefresh(messagesWindow);
    wrefresh(inputWindow);
    wrefresh(sideWindow);

    // End of windows Initialization

    renderInputWindow(inputWindow, inputWindowHeight);

    char name[256] = {0};
    wgetstr(inputWindow, name);

    displayName(sideWindow, name);

    char buffer[256] = {0};

    int socket = configSocket();

    thread printThread(receiveAndPrint, socket, messagesWindow);

    while(true){

        renderInputWindow(inputWindow, inputWindowHeight);
        wgetstr(inputWindow, buffer);

        if(!strcmp(buffer, "exit")){
            break;
        }
        else if(!strcmp(buffer, "")){
            wmove(inputWindow, (int)(inputWindowHeight / 2), 4);
            continue;
        }
        else if(!strcmp(buffer, "changeName")){
            memset(name, 0, sizeof(name));
            wgetstr(inputWindow, name);
            displayName(inputWindow, name);
        }

        renderInputWindow(inputWindow, inputWindowHeight);
        sendMessage(socket, buffer, name);

    }

    endwin();

    return 0;

}