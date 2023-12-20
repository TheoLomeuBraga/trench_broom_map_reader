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

    char *key;
    char *data;

} MapPropriety;

typedef struct
{

    PrimitiveCoord positions[3];
    char *texture;
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

    // Allocate memory for key and data
    propriety->key = NULL;
    propriety->data = NULL;

    // Use sscanf to directly extract quoted strings
    int parsed = sscanf(data, "\"%m[^\"]\" \"%m[^\"]\"", &propriety->key, &propriety->data);

    if (parsed != 2)
    {
        // If parsing is unsuccessful, free allocated memory and return NULL
        free(propriety->key);
        free(propriety->data);
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

    char first_element_skip = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {

        // Check for the end of the aditionalInfo section
        if (strstr(line, "}"))
        {
            break;
        }

        if (strstr(line, "{"))
        {
            if (first_element_skip == 0)
            {
                continue;
                first_element_skip++;
            }
            else
            {
                break;
            }
        }

        // Using sscanf to directly extract quoted strings
        MapPropriety *prop = read_map_propriety(line);
        if (prop)
        {
            
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
        fprintf(stderr, "Error allocating memory for PrimitiveMapStructurePrimitive\n");
        return NULL;
    }

    // Attempt to decode the input string
    int parsed = sscanf(data, "( %d %d %d ) ( %d %d %d ) ( %d %d %d ) %ms %d %d %d %d %d",
                        &primitive->positions[0].x, &primitive->positions[0].y, &primitive->positions[0].z,
                        &primitive->positions[1].x, &primitive->positions[1].y, &primitive->positions[1].z,
                        &primitive->positions[2].x, &primitive->positions[2].y, &primitive->positions[2].z,
                        &primitive->texture,
                        &primitive->textureCord.offsetX, &primitive->textureCord.offsetY,
                        &primitive->textureCord.rotation, &primitive->textureCord.scaleX, &primitive->textureCord.scaleY);

    if (parsed != 15)
    {
        // If parsing is unsuccessful, free allocated memory and return NULL
        free(primitive->texture);
        free(primitive);
        return NULL;
    }

    return primitive;
}


char is_primitive_entity(const char *line) {
    // Check if the line contains the beginning of a PrimitiveMapEntity block
    return strstr(line, "(") != NULL;
}

char is_map_propriety(const char *line) {
    // Check if the line contains quoted strings, indicating a MapPropriety
    return strstr(line, "\"") != NULL;
}

simple_vector load_primitive_map_entitys(FILE *file)
{
    simple_vector entities;
    entities.data = NULL;
    entities.size = 0;

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "}")) // Check for the end of the entities section
        {
            break;
        }

        if (is_primitive_entity(line))
        {
            PrimitiveMapEntity *entity = malloc(sizeof(PrimitiveMapEntity));
            if (!entity)
            {
                fprintf(stderr, "Error allocating memory for PrimitiveMapEntity\n");
                exit(EXIT_FAILURE);
            }

            // Initialize the primitives vector
            entity->primitives.data = NULL;
            entity->primitives.size = 0;

            // Initialize the additionalInfo vector
            entity->aditionalInfo.data = NULL;
            entity->aditionalInfo.size = 0;

            // Process the current line
            PrimitiveMapStructurePrimitive *primitive = read_primitive_map_propriety(line);
            if (primitive)
            {
                entity->primitives.data = realloc(entity->primitives.data, sizeof(PrimitiveMapStructurePrimitive *) * (entity->primitives.size + 1));
                entity->primitives.data[entity->primitives.size++] = primitive;
                //printf("%d",entity->primitives.size);printf(line);
            }
            else
            {
                fprintf(stderr, "Error parsing PrimitiveMapStructurePrimitive from line: %s", line);
                // Handle parsing failure if needed
            }

            // Continue reading the file for either PrimitiveMapStructurePrimitive or MapPropriety
            while (fgets(line, sizeof(line), file) != NULL)
            {
                if (strstr(line, "}")) // Check for the end of the PrimitiveMapEntity
                {
                    break;
                }

                if (is_primitive_entity(line))
                {
                    primitive = read_primitive_map_propriety(line);
                    if (primitive)
                    {
                        entity->primitives.data = realloc(entity->primitives.data, sizeof(PrimitiveMapStructurePrimitive *) * (entity->primitives.size + 1));
                        entity->primitives.data[entity->primitives.size++] = primitive;
                        //printf("%d",entity->primitives.size);printf(line);
                    }
                    else
                    {
                        fprintf(stderr, "Error parsing PrimitiveMapStructurePrimitive from line: %s", line);
                        // Handle parsing failure if needed
                    }
                }
                else if (is_map_propriety(line))
                {
                    // Using read_map_propriety to directly extract quoted strings
                    MapPropriety *prop = read_map_propriety(line);
                    if (prop)
                    {
                        entity->aditionalInfo.data = realloc(entity->aditionalInfo.data, sizeof(MapPropriety *) * (entity->aditionalInfo.size + 1));
                        entity->aditionalInfo.data[entity->aditionalInfo.size++] = prop;
                        
                    }
                }
            }

            entities.data = realloc(entities.data, sizeof(PrimitiveMapEntity *) * (entities.size + 1));
            entities.data[entities.size++] = entity;
        }
    }

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

    fclose(file);
    file = fopen(path, "r");

    ret->entitys = load_primitive_map_entitys(file);
    fclose(file);

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
        printf("    key: %s data: %s \n", prop->key, prop->data);
        i++;
    }

    printf("    entity size: %d\n", map->entitys.size);

    
    printf("    entitys {\n");

    i = 0;
    while (i < map->entitys.size)
    {
        
        
        i++;
    }

    printf("    }\n");
    
    printf("}\n");

    
}