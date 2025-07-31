#ifndef DECODE_H
#define DECODE_H
#include<stdlib.h>
#include "types.h"

#define unsigned int  uint
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    char *src_image_fname; 
    FILE *fptr_src_image;//store adress of source file
    
    char *exten_outf; 
    long out_exten_size; //extension size of uotput file
                   //output File Info 
    char *secret_fname; //store address of secret file
    FILE *fptr_secret;
    

    long magic_str_size;   //magic string size
    char *magic_str;       //magic string base address
    
    
     
}DecodeInfo;

char last_arg[20];        //to store the argv 3 

char decode_string[10];

char output_exten[10];  //secret massage extension

long secret_msg_len;           //length of secret massage
char secret_message[50];   //secret massage string

DecodeInfo decoInfo;

                     

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decoInfo);

Status open_src_file_decode(DecodeInfo *decoInfo);

Status open_output_file(DecodeInfo *decoInfo);

Status do_decoding(DecodeInfo *decoInfo);

//uint move_file_pointer(FILE *fptr_image,DecodeInfo *decoInfo);

uint decode_int_data(FILE *str_out_img);

Status decode_string_data(long size,DecodeInfo *decoInfo, FILE *str_out_img);

Status decode_output_extension(long size,DecodeInfo *decoInfo, FILE *str_out_img);

Status Decode_magic_string(DecodeInfo *decInfo);

Status validate_output_exten(char *argv[],DecodeInfo *decInfo);

Status decoding_rest(DecodeInfo *decInfo);

Status decode_secret_massage(long size,DecodeInfo *decoInfo, FILE *str_out_img,FILE *str_src_file);

#endif