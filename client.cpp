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
#include <vector>
#include "comunicacion/transfer_functions.cpp"
#include "client_threads/client_threads.cpp"
using namespace std;

string z[10];


void DisplayClientMenu(){
	cout << "Main menu\n";
	cout << "1. Broadcasting\n";
	cout << "2. Mensaje privado\n";
	cout << "3. Cambiar status\n";
	cout << "4. Lista de usuarios conectados\n";
	cout << "5. Desplegar informacion de un usuario en particular\n";
	cout << "6. Ayuda\n";
	cout << "7. Salir\n";


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
    
    /*----------MENU---------*/
    bool inMenu = true;
    while(inMenu){
	system("clear");
	DisplayClientMenu();
	string option;
	getline(cin, option);
	system("clear");

	if(option == "1"){
		/*----------THREADS---------*/
		// SEND THREAD
		pthread_t threadSend;
		struct thread_data ts;
		int rs;
		ts.clientSd = clientSd;
		ts.username = username;
		rs = pthread_create(&threadSend, NULL, SendThreadBroadcasting, (void *)&ts);

		// READ THREAD
		pthread_t threadRead;
		struct thread_data tr;
		int rc;
		tr.clientSd = clientSd;
		rc = pthread_create(&threadRead, NULL, ReadThreadBroadcasting, (void *)&tr);

		void *returnSend;
		pthread_join(threadSend, &returnSend);
		pthread_cancel(threadRead);
	}
	
	if(option == "3"){
		cout << "Cambiar status" << endl;
		/*----------THREADS---------*/
		// SEND THREAD
		pthread_t threadSend;
		struct thread_data ts;
		int rs;
		ts.clientSd = clientSd;
		ts.user_id = 1;
		rs = pthread_create(&threadSend, NULL, SendThreadStatus, (void *)&ts);
		
		void *returnSend;
		pthread_join(threadSend, &returnSend);
	}

	if(option == "4"){
		vector<int> List;
		List.push_back(1);

		Mensaje getUsers = new Mensaje(1);
		getUsers.get_user_json(3, List);
		strcpy(msg, getUsers.to_string().c_str());

		send(clientSd, (char *) &msg, strlen(msg), 0);

		// READ THREAD
		pthread_t threadRead;
		struct thread_data tr;
		int rc;
		tr.clientSd = clientSd;
		rc = pthread_create(&threadRead, NULL, ReadThreadUsersList, (void *)&tr);

		void *returnSend;
		pthread_join(threadRead, &returnSend);
	}

	if(option == "6"){
		system("clear");
		cout << "Necesito ayuda, quiero dormir" << endl;
		string op;
		cout << ">" << flush;
		getline(cin, op);
		
	}
	
	if(option == "7"){
		Mensaje goodbye = new Mensaje(1);
		goodbye.goodbye_handshake_json(5, "");
		strcpy(msg, goodbye.to_string().c_str());

		send(clientSd, (char *) &msg, strlen(msg), 0);
		inMenu = false;
	}
	
    }

    cout << "********Session********" << endl;
    
    return 0;
}


