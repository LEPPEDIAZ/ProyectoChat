#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>

using namespace std;

void enviar_mensaje(string mensaje, int socket) {
    int message_len_bytes = str.length(mensaje);
    send(socket, mensaje, message_len_bytes, 0);
}

string recibir_mensaje(int socket) {
    char buffer[256];
    bzero(buffer, 256);

    string response = read(socket, buffer, 255);
    if (response < 0)
        return "error al recibir el mensaje";
    return response;
}
