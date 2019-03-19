#include "requestBuilder/message_builder.cpp"
#include <iostream>

int main(){
    Mensaje respuesta = new Mensaje(1);
    respuesta.build_success_json("conexión exitosa");

    std::cout << respuesta.to_string()<< endl;
}
