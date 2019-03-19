#include "../jsonLib/json.hpp"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
using json = nlohmann::json;
/**
 * genera un json para hacer una solicitud de conexion
 *
 * @param int username stack ID de
 * @return string: JSON del request
 */
string build_request_connection_json(string username) {
    json j;
    j["code"] = 0;
    j["data"]["username"] = username;
    return j.dump();
}

/**
 * genera un json para hacer una solicitud de envio de mensaje
 *
 * @param int[] destinatarios: stack ID's de destinatarios
 * @param string message: mensaje a enviar
 * @param string file_type: formato de mensaje a enviar
 * @return string: JSON del request
 */
string build_send_message_json(vector<int> destinatarios, string message, string file_type) {
    json j;
    j["code"] = 1;
    j["data"]["to"] = destinatarios;
    j["data"]["message"] = message;
    j["data"]["file_type"] = file_type;
    return j.dump();
}

/**
 * genera un json para hacer una solicitud de información de usuario
 *
 * @param int id[]: stack ID del usuario
 * @return string: JSON del request
 */
string build_get_user_json(vector<int> id) {
    json j;
    j["code"] = 3;
    j["data"]["user"] = id;
    return j.dump();
}

/**
 * genera un string formato json para hacer un request de cambio de estado
 *
 * @param int user: stack ID del usuario
 * @param int status: usuario status ID
 * @return
 */
string build_change_status_json(int user, int status) {
    json j;
    j["code"] = 4;
    j["data"]["user"] = user;
    j["data"]["new_status"] = status;
    return j.dump();
}

string build_success_json(int origin, string success_message) {
    json j;
    j["code"] = 200 + origin;
    j["data"]["success_message"] = success_message;
    return j.dump();
}

/**
 * construye un json de error
 *
 * @param string error_message: mensaje de error
 * @param string origin : ID del request que causo el error
 * @return
 */
string build_error_json(int origin, string error_message) {
    json j;
    j["code"] = 500 + origin;
    j["data"]["error_message"] = error_message;
    return j.dump();
}

string tester(string data){
	json j;
	j["data"] = data;
	cout<<j["data"];
}
