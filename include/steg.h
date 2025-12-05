#ifndef BMP_H
#define BMP_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#define VERSION "1.0"
// #define BITMAP "bitmap"
// #define SECRET_FILE "secret_file"

extern int errno;
typedef unsigned char pixels;

typedef enum Filetype {
    BITMAP,
    EMBED_FILE
} Filetype;

#pragma pack(push, 1)
typedef struct bmpfileheader {
    char signature[2];                   // the first two ASCII codes'BM' to check that the file is a BMP file
    unsigned int file_size;         // file size of bitmap file
    unsigned short int reserved1;   // Extra reversed space for the metadata (if needed)
    unsigned short int reserved2;
    unsigned int offsetBits;        // The start of the pixel data
    // total 14 bytes
} BMPFILEHEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct bitmapinfoheader {
    unsigned int infoheader_size;
    unsigned int hr_width_pixels;   // horizontal width in pixels
    unsigned int vt_height_pixels;  // vertical height in pixels
    unsigned short int planes;      // layer of bits in pixel
    unsigned short int bits_per_pixel;  // this field helps identify the bits per pixel
    unsigned int bi_compression;    // field for value of compression
    unsigned int bi_size;           // the size of pixel data
    unsigned int x_pixels_per_meter;    // pixels per meter horizontally
    unsigned int y_pixels_per_meter;    // pixlels per meter vertically
    unsigned int bi_clr;            // colors used
    unsigned int bi_imp_clr;        // only the important colors which are needed
    // 40 bytes
} BITMAPINFOHEADER;

typedef struct BMPinfo {
    pixels *pixel_data;
    size_t pixel_length;
    unsigned int pix_width;
    unsigned int pix_height;
    unsigned int bits_per_pixel;
    unsigned int offset;
    size_t row_size;
} BMPinfo;

typedef struct embed_finfo {
    unsigned char *embed_buff;
    size_t buff_len;
} embed_finfo;

/*
typedef enum arg_mode {
    MODE_HELP = 0,
    MODE_HEADER,
    MODE_EMBED,
    MODE_EXTRACT,
    MODE_VERBOSE,
    MODE_VERSION,
    MODE_F_INFO
} arg_mode;

typedef struct Config {
    enum arg_mode mode;
    unsigned int argcount;
    char *cover_file;
    char *secret_file;
    char *embed_opt;
    char *extract_opt;
    char *pass_phrase;
} Config;

*/
#pragma pack(pop)

char *get_filename(char *st, Filetype type);
void print_bmp_header(BMPFILEHEADER bmp_header);
void print_bmp_info_header(BITMAPINFOHEADER bmp_header);
void bmp_file_check(BMPFILEHEADER bmp_file_header, BITMAPINFOHEADER bmp_info_header, char *bmp_fname);
FILE *fopen_files(char *filename, Filetype type);
void fread_bmp(BMPFILEHEADER *bmp_file_header, BITMAPINFOHEADER *bmp_info_header, FILE *in_bmp_file);
// size_t get_pixel_buff(FILE *in_bmp_file, pixels *bmp_buff, BMPFILEHEADER bmp_file_header, BITMAPINFOHEADER bmp_info_header);
void get_pixel_buff(FILE *in_bmp_file, BMPinfo *bmp_info, BMPFILEHEADER bmp_file_header, BITMAPINFOHEADER bmp_info_header);
void get_embed_file_buff(FILE *embed_file, embed_finfo *file_info);
void set_bmp_header_info(pixels *bmp_buff, size_t pixel_len, BITMAPINFOHEADER info_header, BMPinfo *bmp_info);
//void get_pixel_buff(FILE *in_bmp_file, BMPinfo *bmp_info, BMPFILEHEADER bmp_file_header, BITMAPINFOHEADER bmp_info_header);
void embed_file(embed_finfo *embed, BMPinfo *bmp);

#endif
