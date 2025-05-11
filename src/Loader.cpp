#include "Loader.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h> // For isalnum()

// Drop-in replacement for LoadGameTexture that uses code when available
Texture2D LoadGameTexture(const char *imagePath)
{
    Texture2D texture = {0};

    // Try to find in embedded assets first
    // const char *assetName = GetAssetName(imagePath);
    Image img = LoadImage(imagePath);

    if (img.data != NULL)
    {
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    return texture;
}
