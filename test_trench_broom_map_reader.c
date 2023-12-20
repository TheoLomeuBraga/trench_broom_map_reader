#include <stdio.h>

#include "trench_broom_map_reader.h"

int main(int argc, char **argv){

    if(argc == 2){

        printf("hello trenchbroom map\n");

        SimpleMap *sm = load_simple_map(argv[1]);
        print_simple_map_content(sm);
        delete_simple_map(sm);

    }else{
        printf("please specify the map file\n");
    }
    printf("\n^_^\n\n");
    return 0;
}