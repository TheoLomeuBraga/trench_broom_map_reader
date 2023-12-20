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

SimpleMap *load_simple_map(const char *path)
{
    SimpleMap *map = malloc(sizeof(SimpleMap));

    PrimitiveMap *pmap = load_primitive_map(path);
    print_primitive_map_content(pmap);
    delete_primitive_map(pmap);

    return map;
}
