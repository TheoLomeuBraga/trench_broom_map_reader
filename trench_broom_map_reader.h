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


simple_vector primitivesShapes_to_triangles(simple_vector primitivesShapes){
    simple_vector ret;
    ret.size = 0;
    ret.data = 0;

    return ret;
}

SimpleEntity primitiveEntity_to_simpleEntity(PrimitiveMapEntity primitiveEntity){
    SimpleEntity ret;
    ret.aditionalInfo = copy_simple_vector(&primitiveEntity.aditionalInfo);
    ret.triangles = primitivesShapes_to_triangles(primitiveEntity.primitives);
    return ret;
}

simple_vector primitiveEntitys_to_simpleEntitys(simple_vector primitiveEntitys)
{
    simple_vector ret;
    ret.size = 0;
    ret.data = NULL;

    // Converte entidades primitivas em entidades simples
    for (size_t i = 0; i < primitiveEntitys.size; i++)
    {
        SimpleEntity *simpleEntity = malloc(sizeof(SimpleEntity));
        if (!simpleEntity)
        {
            fprintf(stderr, "Error allocating memory for SimpleEntity\n");
            exit(EXIT_FAILURE);
        }

        // Usa a função auxiliar para converter entidade primitiva em entidade simples
        *simpleEntity = primitiveEntity_to_simpleEntity(*((PrimitiveMapEntity *)primitiveEntitys.data[i]));

        // Adiciona a entidade simples ao vetor
        ret.data = realloc(ret.data, sizeof(void *) * (ret.size + 1));
        ret.data[ret.size++] = simpleEntity;
    }

    return ret;
}



void delete_simple_entity(SimpleEntity *entity)
{
    // Libera a memória da simple_vector triangles
    for (size_t i = 0; i < entity->triangles.size; i++)
    {
        SimpleTriangle *triangle = (SimpleTriangle *)entity->triangles.data[i];
        // Libera a memória da string 'texture'
        free((void *)triangle->texture);
        free(triangle);
    }
    free(entity->triangles.data);

    // Libera a memória da simple_vector aditionalInfo
    for (size_t i = 0; i < entity->aditionalInfo.size; i++)
    {
        MapPropriety *prop = (MapPropriety *)entity->aditionalInfo.data[i];
        // Libera a memória da chave e dos dados
        free((void *)prop->key);
        free((void *)prop->data);
        free(prop);
    }
    free(entity->aditionalInfo.data);

    // Libera a memória da própria estrutura SimpleEntity
    free(entity);
}


void delete_simple_map(SimpleMap *map)
{
    // Libera a memória da string 'path'
    free((void *)map->path);

    // Libera a memória da simple_vector entitys
    for (size_t i = 0; i < map->entitys.size; i++)
    {
        SimpleEntity *entity = (SimpleEntity *)map->entitys.data[i];
        // Chama a função de deletar para cada entidade
        delete_simple_entity(entity);
    }
    free(map->entitys.data);

    // Libera a memória da simple_vector aditionalInfo
    for (size_t i = 0; i < map->aditionalInfo.size; i++)
    {
        MapPropriety *prop = (MapPropriety *)map->aditionalInfo.data[i];
        // Libera a memória da chave e dos dados
        free((void *)prop->key);
        free((void *)prop->data);
        free(prop);
    }
    free(map->aditionalInfo.data);

    // Libera a memória da própria estrutura SimpleMap
    free(map);
}

void print_simple_triangles(unsigned char spaces , simple_vector triangles){
    for(size_t i = 0 ; i < triangles.size;i++){
        for (size_t a = 0; a < spaces; a++)
        {
            printf("    ");
        }
        SimpleTriangle *st = ((SimpleTriangle*)triangles.data[i]);
        printf("texture: %s\n",st->texture);
    }
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
        print_components(3,ent->triangles);
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
    map->entitys = primitiveEntitys_to_simpleEntitys(pmap->entitys);
    

    

    delete_primitive_map(pmap);

    return map;
}
