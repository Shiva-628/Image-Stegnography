/*
Name :- Shiva K V
Date:- 30-08-2024
description :- This C file encodes a secret message into a BMP image and decodes it using a password. 
              It processes user input to save the encoded image with a .encoded extension and extracts the message from the output image.
  */
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
int main(int argc,char *argv[])     //Collecting command line arguments  vector(argv) and (argc)count
{
  if(argc==1)
  {
    printf("INFO: /lsb_steg: Encoding: /lsb_stege <.bmp file> <.txt file> [output file]\nINFO: /lsb steg: Decoding: /lsb_steg-d <.bmp file> [output file]\n");        //if less commmand line arguments print error

    return 0;
  }
   EncodeInfo encInfo;
   int count=argc;
  int val=check_operation_type(argv,count);                  //checking operation type - encoding & decoding
  

  if (val==e_encode)
  {
    if(argc>5)                                          //checking argument count is more than 5 or not
    {
      printf("Invalid arguments  (less then 4)\n");  //printing error massage
      return 0;
    }
  if(argc>3)
  {                                   
   if(val==e_encode)                                    //checking for encode or decode operation
   {
    

    int res=read_and_validate_encode_args(argv,&encInfo);      //if e_encode ->read and validate encode argv
     if(res==e_failure)
     {
        printf("Failed to read arguments\n");             //printing error massage     
     }
     else
     {
       printf("Validation of arguments Successfully completed.\n");  //validation completed means do encoding part
       int val=do_encoding(&encInfo);
       if(val==e_failure)
       {                                              //checking returning value is success or failure
         return 0;
       }
       if(val==e_success)
       {
        printf("Encoded Done Successfull\n");          //printing message for successful encode
        return 0;
       }
     }
   } 
  }
  else
  {
    printf("Less arguments for encoding opetation (at least 4)\nINFO: /1sb_steg: Decoding: ./lsb_stege <.bmp file> [output file] \n");  //printing error massage for less arguments
  }  
  }
  else
  {
    printf("Argument 2 is Unsupported(-e or -d)\nINFO: /lsb_steg: Encoding: /lsb_stege <.bmp file> <.txt file> [output file]\nINFO: /lsb steg: Decoding: /lsb_steg-d <.bmp file> [output file]\n");     //printing error massage invalid argument
    return 0;
  }                                                       
    return 0;
}

OperationType check_operation_type(char *argv[],int argc)
{

  if(!strcmp(argv[1], "-e"))                         //checking argv[1] is "-e"==> return e_encode
  {
    return e_encode;
  }
  else if(strstr(argv[1],"-d"))                    //checking argv[1] is "-d"==> return e_decode
  {
    

    if(argc>2)
    {
    read_and_validate_decode_args(argv,&decoInfo);     //checking validation part 

    
    int res=do_decoding(&decoInfo);                //if validation complete do decoding 
    
    if(res==e_failure)
     {
      exit(0);                                      //if fail to decode exit                                             
     }

     validate_output_exten(argv,&decoInfo);         //validating output extension file
     decoding_rest(&decoInfo);                     //final decoding part
     exit(0);
    }
    else
    {
      printf("Less arguments for decoding(at least 3)\nINFO: /lsb_steg-d <.bmp file> [output file]\n"); //print error massage if less arguments
      exit(0);
    }
  }
  else
  {
     return e_unsupported;                        //else return e_unsupported
  }
}