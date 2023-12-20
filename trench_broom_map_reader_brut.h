#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// basics
typedef struct
{
    void **data;
    size_t size;
} simple_vector;

typedef struct
{
    int x;
    int y;
    int z;
} PrimitiveCoord;

typedef struct
{

    int offsetX;
    int offsetY;
    int rotation;
    int scaleX;
    int scaleY;

} PrimitiveTextureCoord;

typedef struct
{

    const char *key;
    const char *data;

} MapPropriety;

typedef struct
{

    PrimitiveCoord positions[3];
    const char *texture;
    PrimitiveTextureCoord textureCord;

} PrimitiveMapStructurePrimitive;

// map basics

typedef struct
{

    PrimitiveMapStructurePrimitive primitives;
    simple_vector aditionalInfo;

} PrimitiveMapEntity;

typedef struct
{

    const char *path;
    simple_vector entitys;
    simple_vector aditionalInfo;

} PrimitiveMap;




simple_vector load_primitive_map_aditionalInfo(FILE *file)
{
    simple_vector aditionalInfo;
    aditionalInfo.data = NULL;
    aditionalInfo.size = 0;

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Check for the end of the aditionalInfo section
        if (strstr(line, "}"))
        {
            break;
        }

        // Using sscanf to directly extract quoted strings
        char key[256];
        char value[256];

        if (sscanf(line, "\"%255[^\"]\" \"%255[^\"]\"", key, value) == 2)
        {
            
            // Allocate memory for a new MapPropriety
            MapPropriety *prop = malloc(sizeof(MapPropriety));
            if (!prop)
            {
                // Handle memory allocation failure
                fprintf(stderr, "Error allocating memory for MapPropriety\n");
                exit(EXIT_FAILURE);
            }

            // Allocate memory for key and data
            prop->key = strdup(key);
            prop->data = strdup(value);

            // Add the MapPropriety to the list
            aditionalInfo.data = realloc(aditionalInfo.data, sizeof(MapPropriety *) * (aditionalInfo.size + 1));
            aditionalInfo.data[aditionalInfo.size++] = prop;
        }
    }

    return aditionalInfo;
}

simple_vector load_primitive_map_entitys(FILE *file) {
    simple_vector entities;
    entities.data = NULL;
    entities.size = 0;

    

    return entities;
}

PrimitiveMap *load_primitive_map(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        // Handle file open error
        fprintf(stderr, "Error opening file: %s\n", path);
        exit(EXIT_FAILURE);
    }
    PrimitiveMap *ret = malloc(sizeof(PrimitiveMap));
    ret->path = path;

    ret->aditionalInfo = load_primitive_map_aditionalInfo(file);
    ret->entitys = load_primitive_map_entitys(file);

    return ret;
}



void delete_primitive_map(PrimitiveMap *map)
{
    size_t i = 0;

    i = 0;
    while (i < map->entitys.size)
    {
        free(map->entitys.data[i]);
        i++;
    }
    free(map->entitys.data);

    i = 0;
    while (i < map->aditionalInfo.size)
    {
        free(map->aditionalInfo.data[i]);
        i++;
    }
    free(map->aditionalInfo.data);

    free(map);
}

void print_primitive_map_content(PrimitiveMap *map)
{
    printf("%s {\n", map->path);

    size_t i = 0;

    

    i = 0;
    while (i < map->aditionalInfo.size)
    {
        MapPropriety *prop = map->aditionalInfo.data[i];
        printf("        key: %s data: %s \n", prop->key, prop->data);
        i++;
    }
    printf("    }\n");

    printf("}\n");
}