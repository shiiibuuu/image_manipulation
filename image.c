#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "image.h"

// Function of load bmp file
int load_bmp(const char *filename) {
    if (!filename) return 0;

    FILE *input = fopen(filename, "rb");
    if (!input) return 0;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    
    if (fread(&fileHeader, sizeof(BMPFileHeader), 1, input) != 1 ||
        fread(&infoHeader, sizeof(BMPInfoHeader), 1, input) != 1) {
        fclose(input);
        return 0;
    }
//checking bm signature
    if (fileHeader.type != 0x4d42 || infoHeader.bitCount != 24 || infoHeader.compression != 0) {
        fclose(input);
        return 0;
    }

    int is_top_down = (infoHeader.height < 0);
    int new_w = infoHeader.width;
    int new_h = is_top_down ? -infoHeader.height : infoHeader.height;

    //image size validation
    if (new_w <= 0 || new_h <= 0) {
        fclose(input);
        return 0;
    }

    //alocate memory for pixel buffer
    BMPPixel *new_pixels = (BMPPixel *)malloc(new_w * new_h * sizeof(BMPPixel));
    if (!new_pixels) {
        fclose(input);
        return 0;
    }

    if (fseek(input, fileHeader.pixelOffset, SEEK_SET) != 0) {
        free(new_pixels);
        fclose(input);
        return 0;
    }

    // 4 byte padding
    int padding = (4 - (new_w * sizeof(BMPPixel)) % 4) % 4;

    for (int y = 0; y < new_h; y++) {
        int target_row = is_top_down ? (new_h - 1 - y) : y;
        if (fread(&new_pixels[target_row * new_w], sizeof(BMPPixel), new_w, input) != (size_t)new_w) {
            free(new_pixels);
            fclose(input);
            return 0;
        }
        if (padding > 0) {
            fseek(input, padding, SEEK_CUR);
        }
    }

    fclose(input);

    // free of old memory
    if (img_pixels) free(img_pixels);
    if (undo_pixels) {
        free(undo_pixels);
        undo_pixels = NULL;
        undo_w = 0;
        undo_h = 0;
    }

    img_pixels = new_pixels;
    img_w = new_w;
    img_h = new_h;

    return 1;
}

// Function of BMP file save
int save_bmp(const char *filename) {
    if (!filename || !img_pixels || img_w <= 0 || img_h <= 0) return 0;

    FILE *output = fopen(filename, "wb");
    if (!output) return 0;

    int padding = (4 - (img_w * sizeof(BMPPixel)) % 4) % 4;
    uint32_t imageSize = (img_w * sizeof(BMPPixel) + padding) * img_h;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // setup of fileheader
    fileHeader.type = 0x4d42; 
    fileHeader.size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + imageSize;
    fileHeader.reserved1 = 0;
    fileHeader.reserved2 = 0;
    fileHeader.pixelOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    
    memset(&infoHeader, 0, sizeof(BMPInfoHeader));
    infoHeader.size = sizeof(BMPInfoHeader);
    infoHeader.width = img_w;
    infoHeader.height = img_h;
    infoHeader.planes = 1;
    infoHeader.bitCount = 24;
    infoHeader.compression = 0;
    infoHeader.imageSize = imageSize;
    infoHeader.xPixelsPerM = 2835; 
    infoHeader.yPixelsPerM = 2835;
    infoHeader.colorsUsed = 0;
    infoHeader.colorsImportant = 0;

    
    if (fwrite(&fileHeader, sizeof(BMPFileHeader), 1, output) != 1 ||
        fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, output) != 1) {
        fclose(output);
        remove(filename); 
        return 0;
    }

  
    uint8_t zero = 0;
    for (int y = 0; y < img_h; y++) {
        if (fwrite(&img_pixels[y * img_w], sizeof(BMPPixel), img_w, output) != (size_t)img_w) {
            fclose(output);
            remove(filename);
            return 0;
        }
        for (int p = 0; p < padding; p++) {
            if (fwrite(&zero, 1, 1, output) != 1) {
                fclose(output);
                remove(filename);
                return 0;
            }
        }
    }

    fclose(output);
    return 1;
}
