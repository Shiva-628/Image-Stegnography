#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname; 
    FILE *fptr_src_image;//store adress of source file
    uint image_capacity; //source image capacity in bytes
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char* exten;      //address of secret file extension
    char *secret_fname; //store address of secret file
    FILE *fptr_secret;
    
    char extn_secret_file[MAX_FILE_SUFFIX];   //store secret file extension
    char secret_data[MAX_SECRET_BUF_SIZE];  //store secret data
    long size_secret_file;//size of string within secret file(size of secret file)

    /* Stego Image Info */
    char *stego_image_fname;  //output file name
    FILE *fptr_stego_image;  //o/p file pointer adress

} EncodeInfo;

/* Check operation type */


OperationType check_operation_type(char *argv[],int argc);

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

Status open_output_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

Status encode_magic_string_size(FILE *fptr_src_image,FILE *fptr_stego_image,long magic_string_size, EncodeInfo *encInfo);

Status encode_size_to_lsb(long data, char *image_buffer);

Status encode_extn_size(const char *file_extn, EncodeInfo *encInfo);
/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_string_to_image(char *data, int size, FILE *fptr_src_image,FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest,EncodeInfo *encInfo);

/* Encoding function prototype*/
char magic_string[10];
long magic_string_size; //length of magic string
long extn_size; //length of secret file extension
long encode_things_size;
int sec_message_size;

#endif
