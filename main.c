#include "include/steg.h"

int main(int argc, char **argv) {

    FILE *in_bmp_file;
    FILE *embed_file;
    char bmp_fname[256];
    char secret_fname[256];
    BMPFILEHEADER bmp_file_header;
    BITMAPINFOHEADER bmp_info_header;
    pixels *bmp_buff;
    // unsigned char *embed_file_buff;
    // unsigned char *out_pixel_bits;

    // parse_args(argc, argv);

    /* get the bitmap and the secret file name */
    get_filename(bmp_fname, BITMAP);
    get_filename(secret_fname, EMBED_FILE);

    /* allocating memory for BMPinfo struct */
    BMPinfo *bmp = (BMPinfo *) malloc(sizeof(BMPinfo));
    embed_finfo *embed = (embed_finfo *) malloc(sizeof(embed_finfo));

/*
    if(!strstr(file_name, ".bmp")) {
        fprintf(stderr, "Not a valid bitmap image...\n");
        exit(1);
    }
*/

    /* open bitmap file */
    in_bmp_file = fopen_files(bmp_fname, BITMAP);
    embed_file = fopen_files(secret_fname, EMBED_FILE);

/*
    if(!(in_bmp_file = fopen(file_name, "rb"))) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }
*/
    /* reading the bitmap file header and the bmp info header */
    fread_bmp(&bmp_file_header, &bmp_info_header, in_bmp_file);

    print_bmp_header(bmp_file_header);
    print_bmp_info_header(bmp_info_header);

    /* bitmap file validation check */
    bmp_file_check(bmp_file_header, bmp_info_header, bmp_fname);

    /* allocates memory for the buttmap pixel data (bmp_buff) and returns the length of the data buffer */
    // get_pixel_buff(in_bmp_file, bmp, bmp_file_header, bmp_info_header);

    /* create a embed file buffer */
    return 0;
}
