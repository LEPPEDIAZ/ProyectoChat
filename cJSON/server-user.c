#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int main (void){
    FILE *fp = fopen("data.txt", "r");
    char field_name[32], value[32], *out;
    cJSON *root, *names, *ages;

    root  = cJSON_CreateObject();
    names = cJSON_CreateArray();
    ages  = cJSON_CreateArray();
    while(2 == fscanf(fp, " %31[^:]: %31[^\n]", field_name, value)){
        size_t len = strlen(field_name);
        while(field_name[len-1] == ' ')
            field_name[--len] = '\0';//end trim

        if(strcmp(field_name,  "data") == 0)
            cJSON_AddItemToObject(root, "data", cJSON_CreateString(value));
        if(strcmp(field_name,  "host") == 0)
            cJSON_AddItemToArray(hosts, cJSON_CreateString(value));
        if(strcmp(field_name,  "origin") == 0)
            cJSON_AddItemToArray(origins, cJSON_CreateNumber(atof(value)));
    }
    cJSON_AddItemToObject(root, "host", names);
    cJSON_AddItemToObject(root, "origin", ages);
    fclose(fp);
    out = cJSON_Print(root);
    printf("%s\n",out);
    cJSON_Delete(root);
    free(out);

    return 0;
}