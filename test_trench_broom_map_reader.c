#include <stdio.h>

#include "trench_broom_map_reader.h"

int main(int argc, char **argv){

    if(argc == 2){
        printf("hello trenchbroom map\n");
        PrimitiveMap *map = load_primitive_map(argv[1]);
        print_primitive_map_content(map);
        delete_primitive_map(map);
    }else{
        printf("please specify the map file\n");
    }
    printf("\n^_^\n\n");
    return 0;
}