
#include <string>
#include <vector>
#include<ctime>

#define MAX_USER_CONNECTIONS 5
using namespace std;

/**
 * estructura de datos encagada de manejar datos atómicos de un usuario
 */
class User {
private:
    string _username;
    int _status;
    int _last_action_timestamp;
    int _is_active;
    int socket;
    //otras cosas?
public:

    /**
     * void User constructor, inicializa un usuario como inactivo
     */
    void User() {
        _username = "";
        _status = 1;
        _last_action_timestamp = "";
        _is_active = false;

    }

    void User(string username, int status, int last_connected) {
        _username = username;
        _status = status;
        _last_action_timestamp = last_connected;
        _is_active = true;
    }

    int get_user_status() {
        return _status;
    }

    void set_user_status(int status) {
        _status = status;
    }

    int get_last_connected() {
        return _last_connected;
    }

    void set_last_connected(int last_connected) {
        _last_action_timestamp = last_connected;
    }

    void set_username(string username) {
        _username = username;
    }

    string get_username() {
        return _username;
    }

    void set_user_is_active(bool status) {
        _is_active = status;
    }

    bool get_user_is_active() {
        return _is_active;
    }
};


/**
 * estructura de datos encargada de manejar usuarios, brinda multiples facilidades de managing
 */
class User_Manager {
private:
    User userList[MAX_USER_CONNECTIONS];

    /**
     * recorre el userList y busca el siguiente index de usuario vacio
     * @return int: -1 si la cantidad de usuarios esta llena, si no, el index mas pequeno del usuario vacio
     */
    int get_next_empty_user_index() {
        int index = 0;
        for (int i = 0; i < 5; i++) {
            if (userList->get_user_is_active()) {
                return i;
            }
        }
        //no hay casillas de usuario vacias
        return -1;
    }

    /**
     * hace la operacion matematica encargada de calcular tiempo desde ultima accion
     *
     * @return diferencia de tiempo entre _last_action_timestamp y el tiepo actual en segundos
     */
    int get_last_acction_diff(int userIndex) {
        unsigned long int curr_time = time(NULL);
        return userList[userIndex].get_last_connected() - _last_action_timestamp;
    }

public:

    /**
     * agrega un usuario al userstack
     *
     * @param string username : nombre del usuario
     * @param int status: status del usuario
     * @param int last_connected : timestamp de la ultima conexion
     *
     * @return -1 si el userstack esta lleno, 0 si fue exitoso
     */
    int add_user(string username, int status, int last_connected) {
        int next_index = get_next_empty_user_index();
        if (next_index == -1) {
            return -1;
        }
        userList[next_index] = new User(username, status, last_connected);
        return 0;
    }

    /**
     * borra un usuario del userstack
     * @param int index: index del usuario en el userstack que se quiere eliminar
     */
    void whipe_user(int index) {
        userList[index].set_last_connected(0);
        userList[index].set_user_is_active(false);
        userList[index].set_user_status(0);
        userList[index].set_username("");
    }

    /**
     * busca un usuario por su username en el userstack
     *
     * @param string username: nombre de usuario a buscar
     * @return int: index del usuario dentro del userstack, -1 si no se encontro al usuario
     */
    int search_user_by_username(string username) {
        int index = 0;
        for (int i = 0; i < 5; i++) {
            if (userList[i].get_username() == username) {
                return i;
            }
        }
        return -1;
    }

    /**
     * busca en el userstack que usuarios se recomienda descoenctar por inactividad
     *
     * @return vector<int>: vector de indexes del userstack que se recomienda desconectar
     */
    vector<int> get_recommended_timeouts() {
        vector<int> response;
        for (int i = 0; i < 5; i++) {
            if (userList[i].get_last_acction_diff() > 300) {
                response.push_back(i);
            }
        }
        return response;
    }

    /**
     * al llamar este metodo, se actualizara el timestamp de conexion a  todos los users dentro del userstack
     */
    void update_all_user_timestamps() {
        unsigned long int curr_time = time(NULL);
        for (int i = 0; i < 5; i++) {
            userList[i].get_last_connected(curr_time);
        }
    }

};