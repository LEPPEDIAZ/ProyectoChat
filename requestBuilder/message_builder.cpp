#include "../jsonLib/json.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using json = nlohmann::json;

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

    void build_connection_success_json(int user_id,string username, int status){
        j["data"]["id"] = user_id;
        j["data"]["username"] = username;
        j["data"]["status"] = status;

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
