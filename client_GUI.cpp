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
#include <gtk/gtk.h> 


static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Close chat\n");
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add (GTK_CONTAINER (window), button_box);

  button = gtk_button_new_with_label ("Close Chat");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (button_box), button);

  gtk_widget_show_all (window);
}
using namespace std;

string z[10];

struct thread_data {
	int clientSd;
	string username;
};

void *SendThreadBroadcasting(void *threadarg){
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int clientSd = my_data->clientSd;
	string username = my_data->username;

	char msg[1500];
	
	while (1) {
		cout << ">";
		string data;
		getline(cin, data);
		cout << "\r\e[A" << flush;
		Mensaje receive = new Mensaje(1);
		receive.receive_message_json(1, username, data);
		std::cout << receive.to_string() << endl;
		strcpy(msg, receive.to_string().c_str());


		if (data == "close") {
		  
		    send(clientSd, "3312wazos", strlen("3312wazos"), 0);
		    break;
		}
		send(clientSd, (char *) &msg, strlen(msg), 0);
		cout << "Esperando respuesta del server..." << endl;

		cout <<username << ":  " << msg << endl;
	}

	return (void*) "3312wazo";

}

void *ReadThreadBroadcasting(void *threadarg){
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int clientSd = my_data->clientSd;

	while(1){
		string mensaje = recibir_mensaje(clientSd);
		if (mensaje != "3312wazo") {
		    cout << "\r\e[A" << flush;
		    cout << "\r\e[A" << flush;
		    cout << "\n\nEl servidor ha respondido"<< endl;
		    cout << "Mensajes: " << mensaje << "\n" << endl;
		    cout << ">";
		    cout.flush();
			
        	}
		
	}
}


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
    GtkApplication *app;
    int status_gui;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status_gui = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    
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
	
	if(option == "7"){
		inMenu = false;
	}
	
    }

    cout << "********Session********" << endl;
    
    return 0;
}


