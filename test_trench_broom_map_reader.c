#include <stdio.h>

#include "trench_broom_map_reader.h"

int main(int argc, char **argv){

    if(argc == 2){
        
        printf("hello trenchbroom map\n");

        load_simple_map(argv[1]);

    }else{
        printf("please specify the map file\n");
    }
    printf("\n^_^\n\n");
    return 0;
}