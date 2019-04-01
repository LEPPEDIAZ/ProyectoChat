#include <string>
#include <vector>
#include <iostream>
#include <sys/socket.h>

using namespace std;

void enviar_mensaje(string mensaje, int socket) {
    int message_len_bytes = mensaje.length();
    send(socket,& mensaje, message_len_bytes, 0);

}

int is_message_waiting(int sockSd){
    char buffer[256];
    bzero(buffer, 256);
    return recv(sockSd, buffer, 256, 0);
}

string recibir_mensaje(int socket) {
    char buffer[256];
    bzero(buffer, 256);

    int response_len = read(socket, buffer, 256);

    if (response_len < 0)
        return "error";
    string mensaje = buffer;
    return mensaje;
}
string test_auto_io(int sockSd){
    int m_size = is_message_waiting(sockSd);
    char buffer[m_size];
    bzero(buffer, m_size);

    int response_len = read(socket, buffer, m_size);

    if (response_len < 0)
        return "error al recibir el mensaje";
    string mensaje = buffer;
    return mensaje;

}
