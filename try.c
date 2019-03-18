#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int main (void){
    FILE *fp = fopen("data.txt", "r");
    char field_name[32], value[32], *out;
    cJSON *root, *username, *ids, *user, *host, *origin, *status, *last_connected, *code, *data;

    root  = cJSON_CreateObject();
    username = cJSON_CreateArray();
    ids  = cJSON_CreateArray();
    while(2 == fscanf(fp, " %31[^:]: %31[^\n]", field_name, value)){
        size_t len = strlen(field_name);
        while(field_name[len-1] == ' ')
            field_name[--len] = '\0';//end trim

        if(strcmp(field_name,  "data") == 0)
            cJSON_AddItemToObject(user, "data", cJSON_CreateString(value));
        if(strcmp(field_name,  "username") == 0)
            cJSON_AddItemToArray(username, cJSON_CreateString(value));
        if(strcmp(field_name,  "id") == 0)
            cJSON_AddItemToArray(ids, cJSON_CreateNumber(atof(value)));
    }
    cJSON_AddItemToObject(root, "user", user = cJSON_CreateObject());
    cJSON_AddItemToObject(user, "username", username);
    cJSON_AddItemToObject(user, "id", ids);
    cJSON_AddNumberToObject(user, "status", 0);
    cJSON_AddNumberToObject(user, "time", 7);
    cJSON_AddNumberToObject(root, "code", 0);
    cJSON_AddStringToObject(root, "host", "192.168.1.2");
    cJSON_AddStringToObject(root, "origin", "192.168.1.3");
    cJSON_AddItemToObject(root, "data", data = cJSON_CreateObject());
    
    out = cJSON_Print(root);
    printf("%s\n",out);
    FILE *f = fopen("data1.txt", "a");
    fprintf(f, "%s\n",out);
    fclose(f);
    fclose(fp);
    free(out);
    return 0;
}

