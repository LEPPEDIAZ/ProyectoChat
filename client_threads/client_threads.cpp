
struct thread_data {
	int clientSd;
	string username;
	int user_id;
};

/*-------------------------------
	BROADCASTING THREADS
--------------------------------*/
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

		if (data == "close") {
		  
		    //send(clientSd, "3312wazos", strlen("3312wazos"), 0);
		    break;
		}
		
		vector<int> List;
		List.push_back(1);

		Mensaje sendMessage = new Mensaje(1);
		sendMessage.send_message_json(1, List, data, "");
		strcpy(msg, sendMessage.to_string().c_str());

		send(clientSd, (char *) &msg, strlen(msg), 0);
		cout << username << ": " << data << endl;
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
		    cout << "\r" << flush;
		    auto mensaje_parseado = json::parse(mensaje);
		    int code = mensaje_parseado["code"];
		    if(code ==201){
		        string username = mensaje_parseado["data"]["from"];
		        string message = mensaje_parseado["data"]["message"];
		        std::cout << username << ": " << message << endl;
		    }
		    cout << ">";
		    cout.flush();

        	}
		
	}
}


/*-------------------------------
	      STATUS
--------------------------------*/

void *SendThreadStatus(void *threadarg){
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int clientSd = my_data->clientSd;
	int userId = my_data->user_id;
	
	char msg[1500];
	bool isGood = false;
	int option;

	while(isGood != true){
		cout << "Escriba su nuevo estado: "<< endl;
		cout << "1. Activo" << endl;
		cout << "2. Ausente" << endl;
		cout << "3. Ocupado" << endl;

		cout << ">" << flush;
		
		string newStatus;
		getline(cin, newStatus);

		if(newStatus == "1" || newStatus == "2" || newStatus == "3"){
			isGood = true;
			Mensaje status = new Mensaje(1);
			status.change_status_json(4, userId, atoi(newStatus.c_str()));
	
			strcpy(msg, status.to_string().c_str());

			send(clientSd, (char *) &msg, strlen(msg), 0);
		} else {
			system("clear");
		}
	}

	return (void*)"hey";
}


/*-------------------------------
	  LISTA USUARIOS
--------------------------------*/
void *ReadThreadUsersList(void *threadarg){
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int clientSd = my_data->clientSd;

	while(1){
		string mensaje = recibir_mensaje(clientSd);
		if (mensaje != "3312wazo") {
		    auto mensaje_parseado = json::parse(mensaje);
		    int code = mensaje_parseado["code"];
		    if(code ==203){
		        vector<int> users = mensaje_parseado["data"]["users"];
		        cout << "Lista recibida" << endl;
			cout << ">" << flush;
			
			string option;
			getline(cin, option);
			break;
		    }

        	}
		
	}
	return (void*) "hey";
}

