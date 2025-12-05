
#include "include/steg.h"

void print_bmp_header(BMPFILEHEADER bmp_header) {
    fprintf(stdout, "\nType: %.2s, Bitmap image file", bmp_header.signature);
    fprintf(stdout, "\nFile size: %u", bmp_header.file_size);
    fprintf(stdout, "\nExtra reversed1 size: %u", bmp_header.reserved1);
    fprintf(stdout, "\nExtra reversed1 size: %u", bmp_header.reserved1);
    fprintf(stdout, "\nStart of pixel data (offsetBits) : %u\n", bmp_header.offsetBits);
//    fprintf(stdout, "", );
}


void print_bmp_info_header(BITMAPINFOHEADER info_header) {
    fprintf(stdout, "\ninfo header size: %u", info_header.infoheader_size);
    fprintf(stdout, "\nX-Axis width in pixels: %u", info_header.hr_width_pixels );
    fprintf(stdout, "\nY-Axis height in pixels: %u", info_header.vt_height_pixels);
    fprintf(stdout, "\n Image planes: %u", info_header.planes);
    fprintf(stdout, "\n bits per pixel: %u", info_header.bits_per_pixel);
    fprintf(stdout, "\n Type of compression: %u", info_header.bi_compression);
    fprintf(stdout, "\n size of actual pixel data: %u", info_header.bi_size);
    fprintf(stdout, "\n pixel per meter horizontally: %u", info_header.x_pixels_per_meter);
    fprintf(stdout, "\n pixel per meter vertically: %u", info_header.y_pixels_per_meter);
    fprintf(stdout, "\n No.of colors used : %u", info_header.bi_clr);
    fprintf(stdout, "\n No.of only important colors which are useful: %u\n", info_header.bi_imp_clr);
}

void set_bmp_header_info(pixels *bmp_buff, size_t pixel_len, BITMAPINFOHEADER info_header, BMPinfo *bmp_info) {
//    bmp_info->pixel_data = bmp_buff;
//    bmp_info->pixel_length = pixel_len;
    bmp_info->pix_width = info_header.hr_width_pixels;
    bmp_info->pix_height = info_header.vt_height_pixels;
    bmp_info->bits_per_pixel = info_header.bits_per_pixel;
//    bmp_info->offset =
//    bmp_info->row_size =
}

char *get_filename(char *st, Filetype type) {
    /* get file name */
    if(type == BITMAP) {
        printf("Enter the bitmap file name : ");
        scanf("%s", st);
        return st;
    }

    if(type == EMBED_FILE) {
        printf("Enter the secret file name : ");
        scanf("%s", st);
        return st;
    }
}

FILE *fopen_files(char *filename, Filetype type) {
    FILE *file = NULL;
    if(type == BITMAP) {
        file = fopen(filename, "rb");
        if(!file) {
            fprintf(stderr, "Cannot open BMP: %s\n", strerror(errno));
            exit(1);
        }
    }
    if(type == EMBED_FILE) {
        file = fopen(filename, "wb");
        if(!file) {
            fprintf(stderr, "Cannot open file: %s\n", strerror(errno));
        }
    }
    return file;
}

void bmp_file_check(BMPFILEHEADER bmp_file_header, BITMAPINFOHEADER bmp_info_header, char *bmp_fname) {
    /* check if it's a bitmap file */
    if(bmp_file_header.signature[0] != 'B' || bmp_file_header.signature[1] != 'M') {
        fprintf(stderr, "Invalid file Error: %s not a bitmap file\n", bmp_fname);
        exit(1);
    }


    /* to check for the 24-bit bitmap image */
    if(bmp_info_header.bits_per_pixel != 24) {
        fprintf(stderr, "Doesn't support for the %u bit image\n", bmp_info_header.bits_per_pixel);
        exit(1);
    }

    else {
        fprintf(stdout, "\nThis is a 24-bit bitmap image\n");
    }
}


void fread_bmp(BMPFILEHEADER *bmp_file_header, BITMAPINFOHEADER *bmp_info_header, FILE *in_bmp_file) {
        /* reading bitmap file header */
    fread(bmp_file_header, sizeof(BMPFILEHEADER), 1, in_bmp_file);
    if(ferror(in_bmp_file)) {
        fprintf(stderr, "\nError reading bitmap file header...\n");
        exit(1);
    }

    /* reading bitmap header info */
    fread(bmp_info_header, sizeof(BITMAPINFOHEADER), 1, in_bmp_file);
    if(ferror(in_bmp_file)) {
        fprintf(stderr, "\nError reading bitmap info header...\n");
        exit(1);
    }
}

void get_pixel_buff(FILE *in_bmp_file, BMPinfo *bmp_info, BMPFILEHEADER bmp_file_header, BITMAPINFOHEADER bmp_info_header) {
    unsigned int width = bmp_info_header.hr_width_pixels;
    unsigned int height = bmp_info_header.vt_height_pixels;
    unsigned int bytes_per_pixel = bmp_info_header.bits_per_pixel / 8;  // bits to bytes
    bmp_info->row_size = (width * bytes_per_pixel + 3) & ~3;
    /*  padding the row-size suitable for the cpu to process the data
        efficiently which is usually the multiple of 4
        The math is:
        we AND the pixel bytes width with the NOT (N-1),
        (width + (N-1)) & ~(N-1).
        where N is the 4 byte alignment for the ease of CPU to load the data in one clean instruction
        ~(N-1) is used to preserve the upper bits of the pixel data
     */
    bmp_info->pixel_length = bmp_info->row_size * abs(height);
    /*  The whole pixel data in bytes, and the abs() returns the positive height if we ever encounter the
        top-down image which usually has negative height
    */
    bmp_info->pixel_data = (pixels *) malloc(bmp_info->pixel_length);
    if(!(bmp_info->pixel_data)) {
        fprintf(stderr, "Failed to allocate memory for the pixel data...\n");
        exit(1);
    }

    bmp_info->offset = sizeof(bmp_file_header) + bmp_info_header.infoheader_size;
    fprintf(stdout, "bmp_info->offset = %u\n", bmp_info->offset);
    size_t buff_len = fseek(in_bmp_file, bmp_info->offset, SEEK_SET); // setting the file position to the offset bits which is the start of the pixel data
    size_t read_bytes = fread(bmp_info->pixel_data, 1, bmp_info->pixel_length, in_bmp_file);
    if(bmp_info->pixel_length != read_bytes) {
        fprintf(stderr, "Error: only read %zu of %zu bytes\n", read_bytes, bmp_info->pixel_length);
        exit(1);
    }
}

void get_embed_file_buff(FILE *embed_file, embed_finfo *b_info) {
    fseek(embed_file, 0, SEEK_END); // to set the file position to the last byte of the file
    size_t len = ftell(embed_file);              // to get the length of the file in bytes
    rewind(embed_file);             // resets the position pointer back to the start of the file
    b_info->embed_buff = (unsigned char *) malloc(len);
    b_info->buff_len = fread(b_info->embed_buff, 1, len, embed_file);
}


void embed_file(embed_finfo *embed, BMPinfo *bmp) {
    size_t buff_bits = embed->buff_len * 8; // to get the length in bits
    size_t max_bits = bmp->pixel_length;
    size_t bits_to_embed = (buff_bits < max_bits) ? buff_bits : max_bits;
    // least bit flipping Algorithm (lsb)

    for(size_t i = 0; i < bits_to_embed; i++) {

        int byte_index = i / 8;
        /*  byte_index gives you the exact ith byte of the file data you're in currently cauz
            for instance., if i becomes 12 then 12/8 which is 1,
            so in memory we are in the 2nd byte (index starts from 0)
        */

        int bit_pos = 7 - (i % 8);
        /*  bit_pos is calculated by let's say if i = 12; then 10 % 8 = 2.
            This gives out the bit count starting from lsb, and then substracting
            it from 7 flips it so now the count starts from the msb which is 5.
            let's observe the bit manipulation happening below...
            7 - (10 % 8) = 5;
            7 - (11 % 8) = 4;
            7 - (12 % 8) = 3;
            7 - (13 % 8) = 2;
            7 - (14 % 8) = 1;
            7 - (15 % 8) = 0;   //  as soon as the byte_index reaches the end,
             . . . . . . .      //  the bit position reaches the lsb, and resets to 7
             . . . . . . .      //  again for the msb of the pixel byte
            7 - (16 % 8) = 7; <------ see!
                                   */
        int bit = (embed->embed_buff[byte_index] >> bit_pos) & 1;
        /*  we move the msb to lsb by right-shifing the byte and AND it with 1
            which gives us the msb of that respective byte and so on.
        */

        bmp->pixel_data[i] &= 0xFE;           /*    0xFE (1111 1110) filters pixel data byte to facilitate the
                                                  encoding data in the lsb      */
        bmp->pixel_data[i] |= bit;      // OR's the bit with pixel to encode the bit from the file data
    }
}

/* extract the file from the bmp
void extract_file() {

}
*/



/* Failed attempt error prone and unmodularized initial code below that i've written compared to the slightly working one */


/*
//     Creating a buffer to store the pixel data of the bitmap
    unsigned int bytes_per_pix = bmp_info_header.bits_per_pixel/8;
    unsigned int row_padding = (bmp_info_header.hr_width_pixels * 3 + 3)& ~3; // adds the padding for the pixel bits efficient alignment such that it should be divisible by 4, which is what the CPU works fast with the 4 and 8 byte data-sets
    unsigned int pixel_buff_size = row_padding * abs(bmp_info_header.vt_height_pixels);  // abs() because to cover the exception for the negative height (top-down image)
    pixel_bits = (unsigned char *) malloc(pixel_buff_size);


    fseek(in_bmp_file, (sizeof(bmp_file_header) + bmp_info_header.infoheader_size), SEEK_SET);
    fread(pixel_bits, 1, pixel_buff_size, in_bmp_file);

    // test print the acquired pixel bytes
    printf("printing the pixel data : \n");
    for(int i = 873; i < 960; i++) {
        for(int j = 0; j < 6; j++) {
            printf("%02X ", pixel_bits[i+j]);
        }
        putchar('\n');
    }



    // opening and reading the embed file

    char output_file_name[128];
    strcat(file_name, "2");
    strcat(output_file_name, file_name);
    FILE *output_file;
    out_bmp_file = fopen(output_file_name, "wb");
    output_file = fopen("Test_text.txt", "w");
    unsigned int text[6] = {0x6E, 0x69, 0x67, 0x67, 0x61, 0x00};
  //  fwrite(text, 1, 6, output_file);

// encoding the text into bitmap
    unsigned char bit;
    int p = 0;
    for(int i = 0; text[i] != '\0'; i++) {
        for(int i = 0; i < 8; i++) {
            bit = (text[i] >> i) & 1;
            pixel_bits[i] &= 0xFE;
            pixel_bits[i] |= text[i];
        }
    }

 int p1 = 0;

while (1) {
    unsigned char ch = 0;

    for (int bit = 0; bit < 8; bit++) {
        unsigned char bit_val = pixel_bits[p1] & 1;
        ch |= (bit_val << bit);
        p1++;
    }

    if (ch == '\0') break;   // stop at null terminator

    fputc((unsigned int) ch, output_file);
}

    fclose(output_file);
    fclose(in_bmp_file);
    fclose(out_bmp_file);


    for(int i = 0; i < 6; i++) {

    }
*/
