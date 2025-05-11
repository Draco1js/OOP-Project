#include "ImageExport.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h> // For isalnum()

// Helper function to get the asset name (for registry)
static const char *GetAssetName(const char *imagePath)
{
    // Extract filename without extension
    static char assetName[256] = {0};
    const char *lastSlash = strrchr(imagePath, '/');
    const char *lastBackslash = strrchr(imagePath, '\\');
    const char *filename = (lastSlash > lastBackslash) ? lastSlash + 1 : ((lastBackslash) ? lastBackslash + 1 : imagePath);

    // Copy filename without extension
    strncpy(assetName, filename, sizeof(assetName) - 1);
    char *dot = strrchr(assetName, '.');
    if (dot)
        *dot = '\0';

    return assetName;
}

// Drop-in replacement for LoadTexture that uses code when available
Texture2D LoadTexture(const char *imagePath)
{
    Texture2D texture = {0};

    // Try to find in embedded assets first
    const char *assetName = GetAssetName(imagePath);
    Image img = LoadImage(imagePath);

    if (img.data != NULL)
    {
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    return texture;
}
