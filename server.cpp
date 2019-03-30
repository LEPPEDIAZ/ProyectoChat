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
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int SocketSend(int hSocket, char* Rqst, short lenRqst)
{
	int shortRetval = -1;
	struct timeval tv;
	tv.tv_sec = 20;
	tv.tv_usec = 0;
	if (setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0)
	{
		return -1;
	}
	shortRetval = send(hSocket , Rqst, lenRqst, 0);
	
	return shortRetval;
}

int SocketReceive (int hSocket, char* Rsp, short RvcSize)
{
	int shortRetval = -1;
	struct timeval tv;
	tv.tv_sec = 20;
	tv.tv_usec = 0;
	if (setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0)
	{
		return -1;
	}
	shortRetval = recv(hSocket , Rsp, RvcSize, 0);
	
	return shortRetval;
}


int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
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

    listen(sockSd, 5);
     
    sockaddr_in newSockAddr;
//* La llamada acepta la conexion *//
    socklen_t newSockAddrSize = sizeof(newSockAddr);

    int newSd = accept(sockSd, (struct sockaddr *) &newSockAddr, &newSockAddrSize);
    if (newSd < 0) {
        cerr << "Error aceptando el request del client" << endl;
        exit(1);
    }

    cout << "Cliente conectado!" << endl;
    
    //creat

    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    int bytesRead, bytesWritten = 0;
    while (1) {

        cout << "Esperando respuesta del cliente..." << endl;
	Mensaje respuesta = new Mensaje(1);
	respuesta.success_connection_json(200, 2, "hola", 0);
	std::cout << respuesta.to_string() << endl;
	
	
        

       
    }

    gettimeofday(&end1, NULL);
    close(newSd);
    close(sockSd);
    cout << "********Session********" << endl;
    return 0;
}
