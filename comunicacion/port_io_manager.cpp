void *inc_x(void *x_void_ptr) {

/* increment x to 100 */
    int *x_ptr = (int *) x_void_ptr;
    while (++(*x_ptr) < 100);

    printf("x increment finished\n");

/* the function must return something - NULL will do */
    return NULL;

}

void *listen_to_socket(){

}