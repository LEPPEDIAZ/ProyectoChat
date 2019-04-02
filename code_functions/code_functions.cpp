#include "../Users/user_classes.cpp"
#include <string>


int request_connection(string username, int socket, User_Manager &manager){
    unsigned long int curr_time = time(NULL);
    return user_stack_index = manager->add_user(username, 1, curr_time, socket);
}

double cm_to_inches(double cm) {
    return cm / 2.54;
}

// "strchr" is part of the C string handling (i.e., no need for declaration)
// See https://en.wikipedia.org/wiki/C_string_handling#Functions

int main(void) {
    double (*func1)(double) = cm_to_inches;
    printf("%i", func1(15.0));
    /* prints "5.905512 pedia" */
    return 0;
}

int parse_request_code(int code, User_Manager &manager){
    switch (code) {
        case 1:
            int (*fpointer)(int) = request_connection;
            break;
    }
}