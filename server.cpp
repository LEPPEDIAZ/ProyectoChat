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
using namespace std;

int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    
    int port = atoi(argv[1]);
    
    char msg[1500];
     
    
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error " << endl;
        exit(0);
    }
   
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error " << endl;
        exit(0);
    }
    cout << "Esperando que el cliente se conecte..." << endl;
    
    listen(serverSd, 5);
    
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error aceptando el request del client" << endl;
        exit(1);
    }
    cout << "Cliente conectado!" << endl;
    
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    
    int bytesRead, bytesWritten = 0;
    while(1)
    {
        
        cout << "Esperando respuesta del cliente..." << endl;
        memset(&msg, 0, sizeof(msg));
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Client se a salido" << endl;
            break;
        }
        cout << "Client: " << msg << endl;
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg)); 
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    }
    
    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);
    cout << "********Session********" << endl;
    cout << "Bytes escritos: " << bytesWritten << " Bytes leidos: " << bytesRead << endl;
    cout << " Tiempo transcurrido: " << (end1.tv_sec - start1.tv_sec) 
        << " secs" << endl;
    cout << "Saliendo de conexion..." << endl;
    return 0;   
}