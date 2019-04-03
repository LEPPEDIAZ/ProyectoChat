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
#include <pthread.h>
#include <cstdlib>
#include "comunicacion/transfer_functions.cpp"
using namespace std;

string z[10];

struct thread_data {
	int clientSd;
	string username;
};

void *SendThread(void *threadarg){
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int clientSd = my_data->clientSd;
	string username = my_data->username;

	char msg[1500];
	
	while (1) {
		cout << ">";
		string data;
		getline(cin, data);
		Mensaje receive = new Mensaje(1);
		receive.receive_message_json(1, username, data);
		std::cout << receive.to_string() << endl;
		strcpy(msg, receive.to_string().c_str());


		if (data == "close") {
		  
		    send(clientSd, (char *) &msg, strlen(msg), 0);
		    break;
		}
		send(clientSd, (char *) &msg, strlen(msg), 0);
		cout << "Esperando respuesta del server..." << endl;

		cout <<username << ":  " << msg << endl;
	}

}


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
        Mensaje error_connect = new Mensaje(1);
        error_connect.error_connection_json(1, "no se puedo conectar al servidor");
        std::cout << error_connect.to_string() << endl;
    }
    //parsing code of the request
    Mensaje request = new Mensaje(1);
    request.request_connection_json(1, username);
    std::cout << request.to_string() << endl;
    
    respuesta.build_success_json("Server Connection successfull");
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);


    //Threads
    pthread_t threadSend;
    struct thread_data td;
    int rc;
    td.clientSd = clientSd;
    td.username = username;
    rc = pthread_create(&threadSend, NULL, SendThread, (void *)&td);
	
    while(1){
    }
    cout << "********Session********" << endl;
    
    return 0;
}

