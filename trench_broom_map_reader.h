#pragma once

#include "trench_broom_map_reader_brut.h"

// structs defined in other files
/*

typedef struct
{
    size_t size;
    void **data;
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
    float scaleX;
    float scaleY;

} PrimitiveTextureCoord;

typedef struct
{

    char *key;
    char *data;

} MapPropriety;

typedef struct
{

    PrimitiveCoord positions[3];
    const char *texture;
    PrimitiveTextureCoord textureCord;

} PrimitiveMapStructurePrimitive;

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

*/

typedef struct
{
    float x, y;
} SimpleCoord_2;

typedef struct
{
    float x, y, z;
} SimpleCoord_3;

typedef struct
{
    SimpleCoord_3 position;
    SimpleCoord_2 uv;
} SimpleVertex;

typedef struct
{
    SimpleVertex vertex[3];
    const char *texture;
} SimpleTriangle;

typedef struct
{

    simple_vector triangles;
    simple_vector aditionalInfo;

} SimpleEntity;

typedef struct
{

    const char *path;
    simple_vector entitys;
    simple_vector aditionalInfo;

} SimpleMap;

simple_vector primitives_to_triangles(simple_vector primitives){
    simple_vector ret;
    ret.size = 0;
    ret.data = 0;

    return ret;
}

simple_vector primitiveEntitys_to_simpleEntitys(simple_vector primitives){
    simple_vector ret;
    ret.size = 0;
    ret.data = 0;

    return ret;
}

simple_vector copy_simple_vector(simple_vector *source)
{
    simple_vector destination;
    destination.size = source->size;
    destination.data = malloc(sizeof(MapPropriety *) * destination.size);

    if (!destination.data)
    {
        fprintf(stderr, "Error allocating memory for simple_vector\n");
        exit(EXIT_FAILURE);
    }

    // Copia cada elemento de source para destination
    for (size_t i = 0; i < destination.size; i++)
    {
        MapPropriety *prop = (MapPropriety *)source->data[i];

        // Aloca memória para a nova key e data e as copia
        MapPropriety *newProp = malloc(sizeof(MapPropriety));
        newProp->key = strdup(prop->key);
        newProp->data = strdup(prop->data);

        destination.data[i] = newProp;
    }

    return destination;
}

void print_simple_map_content(SimpleMap *map)
{
    printf("%s {\n", map->path);

    size_t i = 0;

    
    print_components(1,map->aditionalInfo);

    printf("    entity size: %d\n", map->entitys.size);

    
    printf("    entitys {\n");

    i = 0;
    while (i < map->entitys.size)
    {
        SimpleEntity *ent = map->entitys.data[i];
        printf("        {\n");
        print_components(3,ent->aditionalInfo);
        
        printf("        }\n");
        
        i++;
    }

    printf("    }\n");
    
    printf("}\n");

    
}

SimpleMap *load_simple_map(const char *path)
{
    SimpleMap *map = malloc(sizeof(SimpleMap));

    PrimitiveMap *pmap = load_primitive_map(path);
    

    map->path = strdup(path);
    map->aditionalInfo = copy_simple_vector(&pmap->aditionalInfo);
    

    print_simple_map_content(map);

    delete_primitive_map(pmap);

    return map;
}
