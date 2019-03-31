#include "../Users/user_classes.cpp"
#include <string>

int request_connection(User_Manager &manager, string username){
    unsigned long int curr_time = time(NULL);

    manager.add_user(username,0,curr_time);
}