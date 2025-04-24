#include "ImageExport.h"
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h> // For isalnum()

// Helper function to get code filename from image path
static const char *GetCodeFilename(const char *imagePath)
{
    // Create embedded directory if it doesn't exist
    static const char *embeddedDir = "embedded";
    struct stat st = {0};
    if (stat(embeddedDir, &st) == -1)
    {
#ifdef _WIN32
        mkdir(embeddedDir);
#else
        mkdir(embeddedDir, 0700);
#endif
    }

    // Extract filename without extension
    static char codeFilename[512] = {0};
    const char *lastSlash = strrchr(imagePath, '/');
    const char *lastBackslash = strrchr(imagePath, '\\');
    const char *filename = (lastSlash > lastBackslash) ? lastSlash + 1 : ((lastBackslash) ? lastBackslash + 1 : imagePath);

    // Format the output path: embedded/filename_data.h
    snprintf(codeFilename, sizeof(codeFilename), "%s/%s_data.h", embeddedDir, filename);

    // Replace any dots in the filename with underscores
    char *dot = strrchr(codeFilename, '.');
    if (dot)
        *dot = '\0';

    // Add .h suffix
    strcat(codeFilename, ".h");

    return codeFilename;
}

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

// Global registry for embedded assets
#define MAX_EMBEDDED_ASSETS 100
typedef struct
{
    char name[256];
    Image image;
    bool loaded;
} EmbeddedAsset;

static EmbeddedAsset embeddedAssets[MAX_EMBEDDED_ASSETS] = {0};
static int embeddedAssetCount = 0;

// Register an embedded asset
void RegisterEmbeddedAsset(const char *name, Image image)
{
    if (embeddedAssetCount < MAX_EMBEDDED_ASSETS)
    {
        strncpy(embeddedAssets[embeddedAssetCount].name, name, 255);
        embeddedAssets[embeddedAssetCount].image = image;
        embeddedAssets[embeddedAssetCount].loaded = true;
        embeddedAssetCount++;
    }
}

// Find an embedded asset by name
Image *FindEmbeddedAsset(const char *name)
{
    for (int i = 0; i < embeddedAssetCount; i++)
    {
        if (strcmp(embeddedAssets[i].name, name) == 0 && embeddedAssets[i].loaded)
        {
            return &embeddedAssets[i].image;
        }
    }
    return NULL;
}

bool ExportImageAsCode(Image image, const char *fileName)
{
    if (image.data == NULL || fileName == NULL)
        return false;

    // Create the embedded directory if it doesn't exist
    const char *dir = "embedded";
    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
#ifdef _WIN32
        mkdir(dir);
#else
        mkdir(dir, 0700);
#endif
    }

    // Open the file for writing
    FILE *file = fopen(fileName, "wt");
    if (file == NULL)
        return false;

    int dataSize = image.width * image.height * 4; // RGBA format = 4 bytes per pixel

    // Create a clean asset name (no path, no extension, no special chars)
    const char *lastSlash = strrchr(fileName, '/');
    const char *lastBackslash = strrchr(fileName, '\\');
    const char *baseName = (lastSlash > lastBackslash) ? lastSlash + 1 : ((lastBackslash) ? lastBackslash + 1 : fileName);

    // Create a clean macro name for include guards
    char macroName[256] = {0};
    strncpy(macroName, baseName, sizeof(macroName) - 1);

    // Replace non-alphanumeric characters with underscores
    for (int i = 0; macroName[i]; i++)
    {
        if (!isalnum((unsigned char)macroName[i]) && macroName[i] != '_')
        {
            macroName[i] = '_';
        }
    }

    // Convert to uppercase for include guard
    for (int i = 0; macroName[i]; i++)
    {
        macroName[i] = toupper((unsigned char)macroName[i]);
    }

    // Write header information with include guards
    fprintf(file, "// Image data exported with raylib\n");
    fprintf(file, "// Image size: %dx%d, channels: 4, format: %d\n\n",
            image.width, image.height, image.format);

    // Add include guards
    fprintf(file, "#ifndef %s_INCLUDED\n", macroName);
    fprintf(file, "#define %s_INCLUDED\n\n", macroName);

    fprintf(file, "#include <raylib.h>\n");
    fprintf(file, "#include \"../src/ImageExport.h\"\n\n");

    // Create a clean variable name (no special chars)
    char varName[256] = {0};
    strncpy(varName, baseName, sizeof(varName) - 1);

    // Replace non-alphanumeric characters with underscores
    for (int i = 0; varName[i]; i++)
    {
        if (!isalnum((unsigned char)varName[i]) && varName[i] != '_')
        {
            varName[i] = '_';
        }
    }

    fprintf(file, "static const unsigned char %s_data[%d] = {\n    ", varName, dataSize);

    // Write pixel data as hex values
    for (int i = 0; i < dataSize; i++)
    {
        fprintf(file, "0x%02x", ((unsigned char *)image.data)[i]);

        if (i < dataSize - 1)
            fprintf(file, ", ");

        // Format the output for readability (16 values per line)
        if ((i + 1) % 16 == 0)
            fprintf(file, "\n    ");
    }

    fprintf(file, "\n};\n\n");

    // Add helper code to register the image at program startup
    fprintf(file, "// Auto-registration function\n");

#if defined(_WIN32)
    fprintf(file, "static void Register_%s(void) {\n", varName);
#else
    fprintf(file, "__attribute__((constructor))\n");
    fprintf(file, "static void Register_%s(void) {\n", varName);
#endif

    fprintf(file, "    Image image = { 0 };\n");
    fprintf(file, "    image.data = (void *)%s_data;\n", varName);
    fprintf(file, "    image.width = %d;\n", image.width);
    fprintf(file, "    image.height = %d;\n", image.height);
    fprintf(file, "    image.mipmaps = 1;\n");
    fprintf(file, "    image.format = %d;\n", image.format);
    fprintf(file, "    RegisterEmbeddedAsset(\"%s\", image);\n", baseName);
    fprintf(file, "}\n\n");

#if defined(_WIN32)
    // Add initialization for Windows
    fprintf(file, "// Windows initialization\n");
    fprintf(file, "class %s_Initializer {\n", varName);
    fprintf(file, "public:\n");
    fprintf(file, "    %s_Initializer() { Register_%s(); }\n", varName, varName);
    fprintf(file, "};\n");
    fprintf(file, "static %s_Initializer %s_initializer;\n\n", varName, varName);
#endif

    // Close include guard
    fprintf(file, "#endif // %s_INCLUDED\n", macroName);

    fclose(file);
    printf("Generated embedded asset: %s\n", fileName);
    return true;
}

// Smart image loading function that uses code if available
Image LoadImageWithCodeFallback(const char *imagePath)
{
    Image image = {0};

    // Try to find in embedded assets first
    const char *assetName = GetAssetName(imagePath);
    Image *embeddedImg = FindEmbeddedAsset(assetName);

    if (embeddedImg != NULL)
    {
        // Found in embedded assets, return a copy
        image = *embeddedImg;
        return image;
    }

    // Load from original file
    image = LoadImage(imagePath);

    // If loaded successfully, export as code for future use
    if (image.data != NULL)
    {
        const char *codeFilename = GetCodeFilename(imagePath);
        ExportImageAsCode(image, codeFilename);
    }

    return image;
}

// Function to convert all assets in a directory
void ConvertAllAssetsToCode(const char *directory)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char fullPath[512];

    // Create the embedded directory if it doesn't exist
    const char *embeddedDir = "embedded";
    struct stat st = {0};
    if (stat(embeddedDir, &st) == -1)
    {
#ifdef _WIN32
        mkdir(embeddedDir);
#else
        mkdir(embeddedDir, 0700);
#endif
    }

    dir = opendir(directory);
    if (dir == NULL)
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Create full path
        snprintf(fullPath, sizeof(fullPath), "%s/%s", directory, entry->d_name);

        // Get file info
        if (stat(fullPath, &statbuf) == -1)
            continue;

        if (S_ISDIR(statbuf.st_mode))
        {
            // Recursively process subdirectories
            ConvertAllAssetsToCode(fullPath);
        }
        else
        {
            // Check if it's an image file by extension
            const char *ext = strrchr(entry->d_name, '.');
            if (ext && (strcmp(ext, ".png") == 0 ||
                        strcmp(ext, ".jpg") == 0 ||
                        strcmp(ext, ".jpeg") == 0 ||
                        strcmp(ext, ".bmp") == 0 ||
                        strcmp(ext, ".gif") == 0 ||
                        strcmp(ext, ".tga") == 0))
            {
                // It's an image, try to convert it
                Image img = LoadImage(fullPath);
                if (img.data != NULL)
                {
                    // Create code filename
                    const char *codeFilename = GetCodeFilename(fullPath);

                    // Export as code
                    ExportImageAsCode(img, codeFilename);

                    // Unload image
                    UnloadImage(img);

                    printf("Converted %s to code\n", fullPath);
                }
            }
        }
    }

    closedir(dir);
}

// Drop-in replacement for LoadTexture that uses code when available
Texture2D LoadTextureWithCodeFallback(const char *imagePath)
{
    Texture2D texture = {0};

    // Try to find in embedded assets first
    const char *assetName = GetAssetName(imagePath);
    Image *embeddedImg = FindEmbeddedAsset(assetName);

    if (embeddedImg != NULL)
    {
        // Found in embedded assets
        // Add debug log
        printf("CACHE: Found embedded asset %s\n", assetName);
        texture = LoadTextureFromImage(*embeddedImg);
        return texture;
    }

    // Not found in embedded assets, load from file
    printf("NO CACHE: Loading image %s\n", imagePath);
    Image img = LoadImage(imagePath);

    if (img.data != NULL)
    {
        // Successfully loaded from file, export as code for next time
        const char *codeFilename = GetCodeFilename(imagePath);
        ExportImageAsCode(img, codeFilename);

        // Convert to texture
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    return texture;
}
