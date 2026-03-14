#include "loadTexture.h"
#include <stdio.h>
#include <string.h>

//copied from my old projeckt
static ManagedImage images[MAX_IMAGES];
static int imageCount = 0;

Image* LoadTextures(const char* name, const char* path) {
    for (int i = 0; i < imageCount; i++) {
        if (strcmp(images[i].name, name) == 0 && images[i].loaded) {
            printf("Texture '%s' Loadet\n", name);
            return &images[i].image;
        }
    }
    
    if (imageCount < MAX_IMAGES) {
        printf("LoadTexture '%s' from: %s\n", name, path);
        Image img = LoadImage(path);
        
        if (img.data != NULL) {
            strcpy(images[imageCount].name, name);
            images[imageCount].image = img;
            images[imageCount].loaded = true;
            imageCount++;
            printf("Texture '%s' loadet sucsesfully!\n", name);
            return &images[imageCount - 1].image;
        } else {
            printf("Error Cland loade: '%s' \n", name);
        }
    }
    return NULL;
}

void LoadAllTextures(TextureEntry textures[], int count) {
    printf("load %d textures...\n", count);
    for (int i = 0; i < count; i++) {
        LoadTextures(textures[i].name, textures[i].path);
    }
    printf("All textures loadet\n");
}

Image* GetTexture(const char* name) {
    for (int i = 0; i < imageCount; i++) {
        if (strcmp(images[i].name, name) == 0 && images[i].loaded) {
            return &images[i].image;
        }
    }
    printf("Texture '%s' not found!\n", name);
    return NULL;
}

void UnLoadTextures(void) {
    printf("Deload all textures...\n");
    for (int i = 0; i < imageCount; i++) {
        if (images[i].loaded) {
            UnloadImage(images[i].image);
            images[i].loaded = false;
            printf("texture '%s' entloadet\n", images[i].name);
        }
    }
    imageCount = 0;
    printf("All textures endloadet!\n");
}