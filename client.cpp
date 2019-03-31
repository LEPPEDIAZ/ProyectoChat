#include <iostream>
#include "requestBuilder/message_builder.cpp"
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
#include <ctime>

using namespace std;

string z[10];


int main(int argc, char *argv[]) {
    time_t _tm= time(NULL );
    struct tm * curtime = localtime( &_tm);
    //se determina el ip y el numero de puerto
    if (argc != 3) {
        cerr << "Usage: ip_address port" << endl;
        exit(0);
    } //agarra el IP y el numero de puerto
    char *serverIp = argv[1];
    int port = atoi(argv[2]);
    //crea un mensaje al buffer  
    char msg[1500];
    //setup del socket y herramientas de conexion
    struct hostent *host = gethostbyname(serverIp);
    //IP socket adress 
    sockaddr_in sendSockAddr;
    cout << "Ingrese el Usuario:  " << endl;
    Mensaje respuesta = new Mensaje(1);
    string username;
    getline(cin, username);
    respuesta.build_connection_success_json(1, username, 1, "7 de la noche");
    std::cout << respuesta.to_string() << endl;
    bzero((char *) &sendSockAddr, sizeof(sendSockAddr));
    bzero((char *) &respuesta, sizeof(respuesta));
    // http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html  
    sendSockAddr.sin_family = AF_INET;
    //IP adress del socket
    sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr *) *host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //tratando de conectar...
    int status = connect(clientSd,
                         (sockaddr * ) & sendSockAddr, sizeof(sendSockAddr));
    if (status < 0) {
        respuesta.build_error_json("error al conectarse");
    }
    
    respuesta.build_success_json("Server Connection successfull");
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    while (1) {
        cout << ">";
        string data;
        getline(cin, data);
        strcpy(msg, data.c_str());
	Mensaje receive = new Mensaje(1);
    	receive.receive_message_json(1, username, data);
	
        if (data == "exit") {
            send(clientSd, (char *) &msg, strlen(msg), 0);
            break;
        }
        cout << "Esperando respuesta del server..." << endl;
        
        cout << username << ":  " << msg << endl;
    }
   
    cout << "********Session********" << endl;
    
    return 0;
}
