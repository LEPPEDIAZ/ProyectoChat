#include "../jsonLib/json.hpp"
#include <string>
#include <vector>
#include <iostream>
#include<ctime>


using namespace std;
using json = nlohmann::json;
time_t _tm = time(NULL);
struct tm *curtime = localtime(&_tm);

class Mensaje {
private:
    json j;
public:
    /**
     * constructor de Mensaje, hace el set automatico del set del mensaje
     * @param int code : codigo de respuesta del mensaje
     */
    Mensaje(int code) {
        j["code"] = code;
    }

    /**
     * construye un json para hacer una solicitud de conexion
     *
     * @param int username stack ID de
     */
    void build_request_connection_json(string username) {
        j["data"]["username"] = username;
    }

    /**
     * construye un json para hacer una solicitud de envio de mensaje
     *
     * @param int[] destinatarios: stack ID's de destinatarios
     * @param string message: mensaje a enviar
     * @param string file_type: formato de mensaje a enviar
     */
    void build_send_message_json(vector<int> destinatarios, string message, string file_type) {
        j["data"]["to"] = destinatarios;
        j["data"]["message"] = message;
        j["data"]["file_type"] = file_type;
    }

    /**
     * construye un json para hacer una solicitud de información de usuario
     *
     * @param int id[]: stack ID del usuario
     */
    void build_get_user_json(vector<int> id) {
        j["data"]["user"] = id;
    }

    /**
     * construye json para hacer un request de cambio de estado
     *
     * @param int user: stack ID del usuario
     * @param int status: usuario status ID
     */
    void build_change_status_json(int user, int status) {
        j["data"]["user"] = user;
        j["data"]["new_status"] = status;
    }

    /**
     * construye un json con el mensaje de exito en body
     *
     * @param success_message
     */
    void build_success_json(string success_message) {
        j["data"]["success_message"] = success_message;
    }

    /**
     * construye un json de error
     *
     * @param string error_message: mensaje de error
     * @param string origin : ID del request que causo el error
     */
    void build_error_json(string error_message) {
        j["data"]["error_message"] = error_message;
    }

    void build_connection_success_json(int user_id, string username, int status, string last_connected) {
        j["user"]["id"] = user_id;
        j["user"]["username"] = username;
        j["user"]["status"] = status;
        j["user"]["last_connected"] = last_connected;
    }

/** request connection */
    void request_connection_json(int code, string username) {
        j["code"] = code;
        j["data"]["username"] = username;

    }

/*success */
    void success_connection_json(int code, int user_id, string username, int status) {
        j["code"] = code;
        j["data"]["user"]["id"] = user_id;
        j["data"]["user"]["username"] = username;
        j["data"]["user"]["status"] = status;
    }

//*error *//
    void error_connection_json(int code, string error_message) {
        j["code"] = code;
        j["data"]["error_message"] = error_message;;
    }

//* send *//
    void send_message_json(int code, vector<int> List, string message, string file_type) {
        j["code"] = code;
        j["data"]["to"] = List;
        j["data"]["message"] = message;
        j["data"]["file_type"] = file_type;
    }
    void send_message_success_json(string from, message string){
        j["data"]["from"] = from;
        j["data"]["from"] = string;
    }

//* Receive user *//
    void recieve_user_json(int code, vector<int> List) {
        j["code"] = code;
        j["data"]["users"] = List;

    }

//* GET user *//
    void get_user_json(int code, vector<int> List) {
        j["code"] = code;
        j["data"]["user"] = List;

    }

//* receive *//
    void receive_message_json(int code, string username, string message) {
        j["code"] = code;
        j["data"]["from"] = username;
        j["data"]["message"] = message;

    }

//* change status *//
    void change_status_json(int code, int user_id, int new_status) {
        j["code"] = code;
        j["data"]["user"] = user_id;
        j["data"]["new_status"] = new_status;

    }

    void goodbye_handshake_json(int code, string username) {
        j["code"] = code;
        j["data"]['username'] = username;

    }

    /**
     * convierte el json a un string en formato
     *
     * @return string: string en formato JSON
     */
    string to_string() {
        return j.dump();
    }
};
