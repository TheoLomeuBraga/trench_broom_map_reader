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

    simple_vector primitives;
    simple_vector aditionalInfo;

} PrimitiveMapEntity;

typedef struct
{

    const char *path;
    simple_vector entitys;
    simple_vector aditionalInfo;

} PrimitiveMap;


MapPropriety *read_map_propriety(const char *data)
{
    MapPropriety *propriety = malloc(sizeof(MapPropriety));
    if (!propriety)
    {
        // Handle memory allocation failure
        fprintf(stderr, "Error allocating memory for MapPropriety\n");
        return NULL;
    }

    // Attempt to decode the input string
    int parsed = sscanf(data, "\"%255[^\"]\" \"%255[^\"]\"", propriety->key, propriety->data);

    if (parsed != 2)
    {
        // If parsing is unsuccessful, free allocated memory and return NULL
        free(propriety);
        return NULL;
    }

    return propriety;
}

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


PrimitiveMapStructurePrimitive *read_primitive_map_propriety(const char *data)
{
    PrimitiveMapStructurePrimitive *primitive = malloc(sizeof(PrimitiveMapStructurePrimitive));
    if (!primitive)
    {
        // Handle memory allocation failure
        fprintf(stderr, "Error allocating memory for PrimitiveMapStructurePrimitive\n");
        return NULL;
    }

    // Attempt to decode the input string
    int parsed = sscanf(data, " ( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %s %d %d %d %d %d",
                        &primitive->positions[0].x, &primitive->positions[0].y, &primitive->positions[0].z,
                        &primitive->positions[1].x, &primitive->positions[1].y, &primitive->positions[1].z,
                        &primitive->positions[2].x, &primitive->positions[2].y, &primitive->positions[2].z,
                        primitive->texture,
                        &primitive->textureCord.offsetX, &primitive->textureCord.offsetY,
                        &primitive->textureCord.rotation, &primitive->textureCord.scaleX, &primitive->textureCord.scaleY);

    if (parsed != 15)
    {
        // If parsing is unsuccessful, free allocated memory and return NULL
        free(primitive);
        return NULL;
    }

    return primitive;
}



simple_vector load_primitive_map_entitys(FILE *file)
{
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

    

    
    while (i < map->aditionalInfo.size)
    {
        MapPropriety *prop = map->aditionalInfo.data[i];
        printf("        key: %s data: %s \n", prop->key, prop->data);
        i++;
    }
    printf("    }\n");

    i = 0;
    printf("    entity size: %d\n",map->entitys.size);

    printf("}\n");
}