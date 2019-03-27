void enviar_mensaje(string mensaje, int socket) {
    int message_len_bytes = str.length(mensaje);
    send(socket, mensaje, message_len_bytes, 0);
}

string recibir_mensaje(int socket) {
    char buffer[256];
    bzero(buffer, 256);

    string response = read(socket, buffer, 255);
    if (response < 0) error("ERROR reading from socket");
    return str(response);
}
