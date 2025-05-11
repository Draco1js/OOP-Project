#pragma once
#include <raylib.h>
#include <stdio.h>

// Export image as code file defining an array of bytes
bool ExportImageAsCode(Image image, const char *fileName);

// Register an embedded asset
void RegisterEmbeddedAsset(const char *name, Image image);

// Smart image loading function that uses code if available
Image LoadImageWithCodeFallback(const char *imagePath);

// Function to convert all assets in a directory
void ConvertAllAssetsToCode(const char *directory);

// Drop-in replacement for LoadImage that uses code when available
Texture2D LoadTexture(const char *imagePath);
