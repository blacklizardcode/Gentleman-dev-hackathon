#ifndef LOADTEXTURE_H
#define LOADTEXTURE_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_IMAGES 100

typedef struct {
    char name[64];
    Image image;
    bool loaded;
} ManagedImage;

typedef struct {
    const char* name;
    const char* path;
} TextureEntry;

Image* LoadTextures(const char* name, const char* path);
void LoadAllTextures(TextureEntry textures[], int count);
void UnLoadTextures(void);
Image* GetTexture(const char* name);

#endif