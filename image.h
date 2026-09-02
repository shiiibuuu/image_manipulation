#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <iup.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t type;       
    uint32_t size;        
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelOffset;
} BMPFileHeader;

typedef struct {
    uint32_t size;           
    int32_t  width;          
    int32_t  height;     
    uint16_t planes;       
    uint16_t bitCount;     
    uint32_t compression;    
    uint32_t imageSize;    
    int32_t  xPixelsPerM;    
    int32_t  yPixelsPerM;   
    uint32_t colorsUsed;   
    uint32_t colorsImportant; 
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
