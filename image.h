#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <iup.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t type;        // 'BM' Magic Number
    uint32_t size;        // File Size
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelOffset; // Offset to image data
} BMPFileHeader;

typedef struct {
    uint32_t size;            // Header Size
    int32_t  width;           // Image Width
    int32_t  height;          // Image Height
    uint16_t planes;          // Color Planes
    uint16_t bitCount;        // Bits Per Pixel (24-bit)
    uint32_t compression;     // Compression Method
    uint32_t imageSize;       // Image Data Size
    int32_t  xPixelsPerM;     // Horizontal Resolution
    int32_t  yPixelsPerM;     // Vertical Resolution
    uint32_t colorsUsed;      // Colors Used
    uint32_t colorsImportant; // Important Colors
} BMPInfoHeader;
#pragma pack(pop)

// BMP Pixel Array Layout (Standard 24-bit BMP is stored as B, G, R on disk)
typedef struct {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} BMPPixel;

// External Global Variables
extern BMPPixel *img_pixels;
extern int img_w;
extern int img_h;

extern BMPPixel *undo_pixels;
extern int undo_w;
extern int undo_h;

// Function Declarations
int load_bmp(const char *filename);
int save_bmp(const char *filename);

#endif // IMAGE_H