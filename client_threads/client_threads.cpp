
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

		if (data == "close") {
		  
		    send(clientSd, "3312wazos", strlen("3312wazos"), 0);
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
		    cout << "Mensajes: " << mensaje << endl;
		    cout << ">";
		    cout.flush();

        	}
		
	}
}
