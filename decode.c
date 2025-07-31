/*
Name :- Shiva K V
Date:- 30-08-2024
Description :- This C file contains code for decoding a hidden message from the least significant bits (LSB) of an input BMP file and saving the extracted message to a new file with an encoded extension. 
              It reads one bit at a time from the LSB of each pixel to reconstruct the secret message.
*/

#include <stdio.h>
#include "decode.h"
#include "types.h"


                            /* Function Definitions */ 

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)       //function defination for validating arguments
{
if(argv[2]!=NULL)
 {
  char *res;

    if(res=strstr(argv[2],".bmp"))                                //checking input file is .bmp extension or not
     {
      if(strcmp(res,".bmp")==0)
      {
        decInfo->src_image_fname = argv[2];
        printf("INFO: file cointains .bmp extension\n");              //printing success massage
       
      }
      else
      {
        printf("ERROR:  Input file is not .bmp\nINFO: /lsb_steg: Decoding: ./lsb_stege <.bmp file> [output file] \n");
        exit(0);
      }
     }
    else                                                     //else printing error massage
     {
        printf("ERROR; Argument 3 not contain .bmp Extension\nINFO: /1sb_steg: Decoding: ./lsb_stege <.bmp file> [output file] \n");
        exit(0);
     }

   open_src_file_decode(&decoInfo);                          //function call for open source files

   return e_success;

}
else                                            //if argument is missing printing error message
{
  printf("ERROR;  Argument 2 is must needed\nINFO: /1sb_steg: Decoding: ./lsb_stege <.bmp file> [output file] \n");
  return 0;
}
}
Status do_decoding(DecodeInfo *decoInfo) {
    if (fseek(decoInfo->fptr_src_image, 54, SEEK_SET) != 0) {
        perror("fseek");
        fprintf(stderr, "ERROR: Unable to seek to position 54 in file\n");
        return e_failure;
    }

    printf("\nINFO: Moving pointer to %ldth position\n", ftell(decoInfo->fptr_src_image)); // Display current pointer position

    long res1 = decode_int_data(decoInfo->fptr_src_image); // Decoding size
    decoInfo->magic_str_size = res1;
    //printf("INFO: Size of magic string: %ld\n", decoInfo->magic_str_size);
    printf("INFO: Magic string size successfully Decoded.\n");
    
    decode_string_data(decoInfo->magic_str_size, decoInfo, decoInfo->fptr_src_image);
    printf("INFO: Magic string is : %s\n", decode_string);
    printf("INFO: Magic string successfully Decoded.\n");

    long res2 = decode_int_data(decoInfo->fptr_src_image); // Decoding output extension size
    decoInfo->out_exten_size = res2;
    printf("INFO: Output Extension size successfully Decoded.\n");
    
    decode_output_extension(decoInfo->out_exten_size, decoInfo, decoInfo->fptr_src_image);
    
    int res3 = Decode_magic_string(decoInfo); // Check magic string
    if (res3 == e_failure) {
        return e_failure;
    }

    printf("....Decoding Procedure Started......\n");
    return e_success;
}

Status open_src_file_decode(DecodeInfo *decoInfo)         //function defination for open bmp file
{
    printf("INFO: Opening required Files\n");

    decoInfo->fptr_src_image=fopen(decoInfo->src_image_fname,"r");    //opening bmp file in read mode
    
    if(decoInfo->fptr_src_image==NULL)
    {
        printf("Unable to open source image file");                 //error massage if not opened
        return e_failure;
    }
    else
    {
      printf("INFO: Source file successfully opened");         //else success massage

      return e_success;
    }

}
Status open_output_file(DecodeInfo *decoInfo)                 //function defination for open output file
{
    decoInfo->fptr_secret=fopen(decoInfo->secret_fname,"w");    //opening output file in read mode
    if(decoInfo->fptr_secret==NULL)
    {
        printf("Unable to open output secret file");          //if not opened print error massage
        return e_failure;
    }
  printf("INFO: output file is opened");                       //else success massage

    return e_success;
}
/*uint move_file_pointer(FILE *fptr_image,DecodeInfo *decoInfo)  //function defination for move file pointer
{
  fseek(fptr_image,54,SEEK_SET);       //skiping 54 bytes for header file
   //int res=ftell(fptr_image);

   printf("Moving pointer to 54th position\n");    
   return e_success;
}*/
Status decode_int_data(FILE *str_out_img)                     //function defination for decode integer datas
{
  int i;
  int decoded_int=0;
  char temp_buffer;                                   //declaration of temorary variable
  for(i=0;i<32;i++)
  {
    fread(&temp_buffer,sizeof(char),1,str_out_img);

    decoded_int= decoded_int | ((temp_buffer&0x01)<<i);     //reading sizes from input file
  }
  
   return decoded_int;                                        //returning the outputs
   
}

Status decode_string_data(long size,DecodeInfo *decoInfo, FILE *str_out_img)    //function defination for decoding string data
{
  int i,j;
   char temp_buffer;                                                         //declaring a temp char variable
  
  for(i=0;i<size;i++)
  {
    decode_string[i]=0x00;                                                  //initializing array elements as zero
    for(j=0;j<8;j++)
    {
       fread(&temp_buffer,sizeof(char),1,str_out_img);                    //reading data from input image

       decode_string[i]=decode_string[i]|((temp_buffer&0x01)<<j);       //storing data after bit operation

    }
  }
  decode_string[size]='\0';                                       //At last putting null character
  
} 
Status decode_output_extension(long size,DecodeInfo *decoInfo, FILE *str_out_img)   //function defination for decode output file extension
{
  int i,j;
   char temp_buffer;                                                 //taking a character variable 
  
  for(i=0;i<size;i++)
  {
    output_exten[i]=0x00;
    for(j=0;j<8;j++)
    {
       fread(&temp_buffer,sizeof(char),1,str_out_img);                 //reading one one byte from src image

      output_exten[i]=output_exten[i]|((temp_buffer&0x01)<<j);    //putting in variable after some bit operation

    }
  }
  output_exten[size]='\0';                                      //at last putting null character

}
Status Decode_magic_string(DecodeInfo *decInfo)                  //function defination for checking magic string
{
  char magic_string[10];
  printf("INFO: Please Enter Password : ");                          //taking magic string input from user for validation
  scanf("%[^\n]",magic_string);

  if(strstr(magic_string,decode_string))                                 //checking magic string
  {
    printf("INFO: You have Entered Correct password\n");
    return e_success;
  }
  else
  {
    printf("INFO: You have Entered wrong password\n");                  //printing error message if not matched
    return e_failure;
  }


}
Status validate_output_exten(char *argv[],DecodeInfo *decInfo)     //function defination for validate output extension
{
  if(argv[3]==NULL)                                             //checking output file name is present or not
  {
    strcpy(last_arg,"Secret_msg");                               

    strcat(last_arg,output_exten);                           //if present copy extension

    decInfo->secret_fname = last_arg;
  }
  else
  {
    strcpy(last_arg,argv[3]);
    strcat(last_arg,output_exten);                        //if not present add extension in default name
    decInfo->secret_fname = last_arg;
  }
  

   return e_success;

}
Status decoding_rest(DecodeInfo *decInfo)
{

   long res=decode_int_data(decInfo->fptr_src_image);            //decode secret message size 
    secret_msg_len=res;
    //printf("Secret massage size is %ld",sec_msg_len);
    printf("INFO: Secret message size successfully Decoded.\n"); 
    printf("INFO: Done\n");             
    open_output_file(decInfo);

    decode_secret_massage(secret_msg_len,decInfo,decInfo->fptr_src_image,decInfo->fptr_secret);   //function call for decode secret massage

}
Status decode_secret_massage(long size,DecodeInfo *decoInfo, FILE *str_out_img,FILE *str_src_file)   //function defination 
{

  int i,j;
   char temp_buffer;                                           //taking a temp variable buffer for store data

   
  //printf("\nposition is %ld\n",ftell(str_out_img));
  for(i=0;i<size;i++)
  {
    secret_message[i]=0;
    for(j=0;j<8;j++)
    {
       fread(&temp_buffer,sizeof(char),1,str_out_img);               //reading one one byte from source image

      secret_message[i]=secret_message[i]|((temp_buffer&1)<<j);    //storing in temp variable after bit operation

    }
  }
  secret_message[size]='\0';                                            //putting null character at last

  //printf("\nposition is %ld\n",ftell(str_out_img));
 
  printf("\nINFO: Secret message successfully Decoded.\n");
  printf("INFO: Done\n") ;                                            //printing Done message

  //printf("\n%s",secret_massage);
   
  printf("....Decoding Done Successfully....\n");//printing Done message 
  printf("INFO: Done\n");          

  fprintf(str_src_file,"%s",secret_message);              //putting secret message to  file
  
  fclose(str_out_img);
  fclose(str_src_file);                            //closing files
  
  exit(0);
}