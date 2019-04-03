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
#include <pthread.h>

#define MAX_USERS_CONNECTED 5
using namespace std;

User_Manager users[MAX_USERS_CONNECTED];
struct thread_data {
    int sockID;
};

void *handle_connection(void *sock_arg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) sock_arg;
    int sockID = my_data->sockID;
    cout << "thread in!" << sockID << endl;
    if (sockID < 0) {
        Mensaje error_connect = new Mensaje(1);
        error_connect.error_connection_json(1, "no se puedo conectar al servidor");
        std::cout << error_connect.to_string() << endl;
        cerr << "Error aceptando el request del client" << endl;
        exit(1);
    }
    cout << "Ahora escuchando para cliente en socket: " << sockID << endl;
    while (1) {
        string mensaje = recibir_mensaje(sockID);
        if (mensaje != "3312wazo") {
            cout << "Request: " << mensaje << "\n" << endl;
            auto mensaje_parseado = json::parse(mensaje);
            int code = mensaje_parseado["code"];
            switch (code) {
                case 1:
                case 4:

                    string mensaje = mensaje_parseado["data"]["message"];
                    string targets = mensaje_parseado["data"]["to"];

                    for (auto it = targets.begin(); it != targets.end(); it++) {
                        cout<<it<<endl;
//                        Mensaje user_message = new Mensaje(201);
//                        user_message.send_message_json(it,targets,mensaje,"");
//                        enviar_mensaje(user_message.to_string(),users->get_user_socket(it));
//                        write(user->fd, msg.c_str(), msg.length());
                    }
                    break;
            }
            //send(sockID, "Mensaje recibido!", strlen("Mensaje recibido!"), 0);
        }
    }

}


int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr, cli_addr;
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
    cout << "Esperando conexiones..." << endl;

    listen(sockSd, MAX_USERS_CONNECTED);

    sockaddr_in newSockAddr;

//* La llamada acepta la conexion *//
    socklen_t newSockAddrSize = sizeof(newSockAddr);

//    int newSd = accept(sockSd, (struct sockaddr *) &newSockAddr, &newSockAddrSize);
    int accepted;
    while ((accepted = accept(sockSd, (struct sockaddr *) &newSockAddr, &newSockAddrSize)) > 0) {
        cout << "Nueva solicitud de conexion de usuario" << endl;
        string mensaje = recibir_mensaje(accepted);
        auto mensaje_parseado = json::parse(mensaje);
        int code = mensaje_parseado["code"];
        if (code == 0) {
            string new_username = mensaje_parseado["data"]["username"];
            unsigned long int curr_time = time(NULL);

            int user_stack_id = users->add_user(new_username, 1, 1, accepted);
            cout << user_stack_id << endl;
            if (user_stack_id != -1) {
                pthread_t thr;
                struct thread_data td;
                td.sockID = users->get_user_socket(user_stack_id);
                pthread_create(&thr, NULL, handle_connection, (void *) &td);
            }
        }
    }


//    if (newSd < 0) {
//        Mensaje error_connect = new Mensaje(1);
//        error_connect.error_connection_json(1, "no se puedo conectar al servidor");
//        std::cout << error_connect.to_string() << endl;
//        cerr << "Error aceptando el request del client" << endl;
//        exit(1);
//    }

    cout << "Cliente conectado!" << endl;


    return 0;
}

