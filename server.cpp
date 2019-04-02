#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include "requestBuilder/message_builder.cpp"
#include "Users/user_classes.cpp"
#include "comunicacion/port_io_manager.cpp"
#include "comunicacion/transfer_functions.cpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define MAX_USERS_CONNECTED 5
using namespace std;

User_Manager users[MAX_USERS_CONNECTED];

//funcion dummy
void user(int Z) {
    for (int i = 0; i < Z; i++) {
        cout << "Thread using fuction";
    }
}

class thread_obj {
public:
    void operator()(int x) {
        for (int i = 0; i < x; i++)
            cout << "thread usando funcion";
    }
};

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    int counter;
    counter = 0;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char client_message[200] = {0};
    int n;
    if (argc != 2) {
        cerr << "Usage: port" << endl;
        exit(0);
    }

    int port = atoi(argv[1]);

    char msg[1500];


    sockaddr_in servAddr;
    bzero((char *) &servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);


    int sockSd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockSd < 0) {
        cerr << "Error " << endl;
        exit(0);
    }

    int bindStatus = bind(sockSd, (struct sockaddr *) &servAddr,
                          sizeof(servAddr));
    if (bindStatus < 0) {
        cerr << "Error " << endl;
        exit(0);
    }
    cout << "Esperando que el cliente se conecte..." << endl;

    listen(sockSd, MAX_USERS_CONNECTED);

    sockaddr_in newSockAddr;

//* La llamada acepta la conexion *//
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    int newSd = accept(sockSd, (struct sockaddr *) &newSockAddr, &newSockAddrSize);
    if (newSd < 0) {
        Mensaje error_connect = new Mensaje(1);
        error_connect.error_connection_json(1, "no se puedo conectar al servidor");
        std::cout << error_connect.to_string() << endl;
        cerr << "Error aceptando el request del client" << endl;
        exit(1);
    }

    cout << "Cliente conectado!" << endl;
//    thread(thread_obj(), counter);
//    counter += 1;

    //creat

    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    int bytesRead, bytesWritten = 0;

    while (1) {

        //cout << "Esperando respuesta del cliente..." << endl;


//        Mensaje respuesta = new Mensaje(1);
//        respuesta.success_connection_json(200, 2, "hola", 0);
//        std::cout << respuesta.to_string() << endl;

        string mensaje = recibir_mensaje(newSd);
        if (mensaje != "3312wazo") {
            printf("hay un mensaje en cola!\n");
            cout << "Mensajes: " << mensaje << "\n" << endl;
            auto mensaje_parseado = json::parse(mensaje);
            int code = mensaje_parseado["code"];
            cout << "codigo fue: " << code << endl;
//            cout << "tipo de dato: " << typeid(mensaje_parseado).name() << endl;

            cout << "-----";

        }


    }
    return 0;
}



