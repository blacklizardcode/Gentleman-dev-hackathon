#include "loadTexture.h"
#include <stdio.h>
#include <string.h>

// Helper: build a full path relative to the running executable.
static void BuildFullPath(char *out, size_t outSize, const char *relativePath) {
    const char *appDir = GetApplicationDirectory();
    if (appDir == NULL || appDir[0] == '\0') {
        // Fallback: use relative path directly if we somehow can't determine the application directory.
        strncpy(out, relativePath, outSize - 1);
        out[outSize - 1] = '\0';
        return;
    }

    // Ensure we don't overflow.
    snprintf(out, outSize, "%s/%s", appDir, relativePath);
}

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
        char fullPath[1024] = {0};
        BuildFullPath(fullPath, sizeof(fullPath), path);

        printf("LoadTexture '%s' from: %s\n", name, fullPath);
        Image img = LoadImage(fullPath);
        
        if (img.data != NULL) {
            strcpy(images[imageCount].name, name);
            images[imageCount].image = img;
            images[imageCount].loaded = true;
            imageCount++;
            printf("Texture '%s' loadet sucsesfully!\n", name);
            return &images[imageCount - 1].image;
        } else {
            printf("Error loading texture '%s' (path: %s)\n", name, fullPath);
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