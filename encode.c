/*
Name : Shiva K V
Date : 30-08-2024
Description : This C file encodes a secret message into a BMP image and decodes it using a password. 
                It processes user input to save the encoded image with a .encoded extension and extracts the message from the output image. 
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"


                            /* Function Definitions */ 


uint get_image_size_for_bmp(FILE *fptr_src_image)                  //function defination for get image size
{
    uint width, height;
    
    fseek(fptr_src_image, 18, SEEK_SET);                  // Seek to 18th byte

    
    fread(&width, sizeof(int), 1, fptr_src_image);        // Read the width (an int)
    //printf("width = %u\n", width);

    
    fread(&height, sizeof(int), 1, fptr_src_image);       // Read the height (an int)
    //printf("height = %u\n", height);

    
    return width * height * 3;                            // Return image capacity
}

Status open_files(EncodeInfo *encInfo)                   //function defination for opening files
{

  printf("INFO: Entred into Open files function. \n");
    //printf("Given file name - %s\n", encInfo->src_image_fname);
    // printf("Given secrete file name - %s\n", encInfo->secret_fname);
    //printf("Given file name - %s\n", encInfo->stego_image_fname);
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");        //opening src image file in read mode
    
    if (encInfo->fptr_src_image == NULL)                                       // Error handling
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);      //error message if file not opened

    	return e_failure;
    }
    else
    {
      printf("INFO: Image file is opening.\n");                          //if file opened print success massage
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");                  //opening secret file in read mode
    
    if (encInfo->fptr_secret == NULL)                                          // Error handling
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);   //error massage if file not opened

    	return e_failure;
    }
     else{
      printf("INFO: Secrete file is opening.\n");                     //if file opened print success message
     }

     // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }
    else
    {
        printf("INFO: Opening  stego file\n");
    }

    return e_success; // No failure return e_success
     return e_success;

}

Status open_output_files(EncodeInfo *encInfo)
{
    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");          //opening output file in write mode

    if(sec_message_size>0)
    {
    if (encInfo->fptr_stego_image == NULL)                                     // Error handling
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);      //error message if file not opened

    	return e_failure;
    }
    else
    {
      printf("INFO: Output file opened Successfully.\n");                //if file opened print success message
    }
    }
                                                                     
    return e_success;                                                         // No failure return e_success
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
  char *contn;
  if(contn=strstr(argv[2],".bmp"))                       //checking bmp file is present or not
  {
    if(strcmp(contn,".bmp")==0)
    {
    encInfo->src_image_fname = argv[2];
   //printf("Given file name - %s\n", encInfo->src_image_fname);
    printf("INFO: Input file contains .bmp exten\n");     //printing success message
    }
    else{
      
      printf("Input file is not containing .bmp\nINFO: /lsb_steg: Encoding: ./lsb_stege <.bmp file> <.txt file> [output file] \n");
      //exit(0);
    }

  }
  else
   {
    printf("INFO: /lsb_stego: Encoding: ./lsb_stego <.bmp file> <.txt file> [output file] \n");   //printing error message for .bmp
    return e_failure;
   }
  if(encInfo->exten=strstr(argv[3],"."))                //checking secret file contain . extension or not
  {
    encInfo->secret_fname=argv[3];
     //printf("Given secrete file name - %s\n", encInfo->secret_fname);
    
    extn_size=strlen(encInfo->exten);                //finding (extn)extension length

    //printf("secret file extn size is %ld\n",extn_size);
  }
  else
  {
    printf("Argument 4 does't contain any argument...\nINFO: /lsb_stego: Encoding: ./lsb_stego <.bmp file> <.txt file> [output file] \n");    //printing error for extension
    return e_failure;
  }

  if(argv[4]!=NULL)                            //checking output file present or not
  {
    char *contn;
    if(contn=strstr(argv[4],".bmp"))                 //checking output file contains bmp or not
    {
      if(strcmp(contn,".bmp")==0)
    {
        encInfo->stego_image_fname=argv[4];
        //printf("Given file name - %s\n", encInfo->stego_image_fname);
        printf("..........Output file validation is successfull............\n");         //printing success message
    }
    else{
      printf("Output file name  is not containing .bmp\nINFO: /1sb_steg: Encoding: ./lsb_stege <.bmp file> <.txt file> [output file] \n");
      exit(0);
    }   
    }
    else
    {
      printf("Output file name does not contain .bmp extension !\n ");   //error massage for invalid output file name
      exit(0);                        
        
    }
  }
  else
  {
    encInfo->stego_image_fname="Stego.bmp";                            //if output file is not present create one
    printf("Output file is not present by default created Stego.bmp file...\n");
  }
  return e_success;
}
        
Status do_encoding(EncodeInfo *encInfo)
{
  printf("INFO: Successfully opened all files.\n");                    //opening files
  int val=open_files(encInfo);
  if(val==e_success)
  {
  printf("INFO: Enter Magic String: ");                   //reading the magic string from user
  
  scanf("%[^\n]",magic_string);
  
  printf("........Encoding started.......\n");         //printing started message
  int res=check_capacity(encInfo);                      //checking capacity 
  if(res==e_success)
  {
    open_output_files(encInfo);                          //opening output files in write mode

    printf("INFO: Started Checking Capacity.\n");
    copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);       //function call for copy header file
    printf("INFO; Copying Image header.\n");                                  //printing success massage
    printf("INFO: Done.\n");


    encode_magic_string_size( encInfo->fptr_src_image,encInfo->fptr_stego_image,magic_string_size, encInfo);//function call for encoding magic string size
                                                     
    printf("INFO: Magic string size encoding.\n");                 //printing success message
    printf("INFO: Done.\n");
    
    encode_magic_string(magic_string,encInfo);              //function call for encoding magic string                                            
    printf("INFO: Magic string Encoding.\n");             //printing success message
    printf("INFO: Done.\n");

    encode_extn_size(encInfo->exten,encInfo);          //function call for encoding extension size                                      
    printf("INFO: Extension size Encoding.\n");//printing success massage
    printf("INFO: Done.\n");

    encode_secret_file_extn(encInfo->exten,encInfo);     //function call for encoding secretfile extension                                          
    printf("INFO: Extension Encoding.\n");
    printf("INFO: Done.\n");                                     //printing success massage

    encode_secret_file_size(encInfo->size_secret_file,encInfo);      //function call for encoding secretfile size
                                                         
    printf("INFO: secret file size Encoding.\n");                         //printing successmassage
    printf("INFO: Done.\n");

    encode_secret_file_data(encInfo);            //function call for encoding secretfile data                              
    printf("INFO: Secret File Encoding.\n");        //printing success massage
    printf("INFO: Done.\n");
    

    int val3=copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
     if(val3==e_success)                                                                            //function call for encode remaining data
     {
      printf("INFO:  Copying remaining data. \n");
      printf("INFO: Done.\n");
      printf("INFO: Encoding Completed Successfully.\n");                                    //printing successfull massage
      return e_success;
     }
  }
  else
  {
    printf("INFO: Image size is less then Encoding File size\n");                                 //printing error message for check capacity
    return e_failure;
  }
  }
  else
  {
    printf("INFO: Fail to open required files\n");                                       //printing error message for opening files
    return e_failure;
  }
}


Status check_capacity(EncodeInfo *encInfo)                                  //function defination for check capacity  
{
    int image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);    //getting image size
    encInfo->image_capacity=image_capacity;

     sec_message_size=get_file_size(encInfo->fptr_secret);                  //getting secret file size
     encInfo->size_secret_file=sec_message_size;
     if(sec_message_size==0)
     {
      printf("\n Secret message file is Blank\n Please Try again..\n");        //if secret file size i szero printing error massage
      exit(0);
     }
     else{
      printf("INFO: Secret message checked Successfully..\n");          //printing success mssage 
      printf("INFO: Done Not Empty\n");                 
     }

    magic_string_size=strlen(magic_string);                                         //getting message string size

    long encode_size=54+((sizeof(int)+magic_string_size+sizeof(int)+extn_size+sizeof(int)+sec_message_size)*8);        //getting whole data size
    
     encode_things_size=encode_size;
     

    if(image_capacity>encode_size)                                    //checking image capacity 
    {
      return e_success;
    }
    else
    {
      printf("Image size less, it must be Greater.");
      exit(0);
    }


}
uint get_file_size(FILE *fptr)                                  //function defination for get secret file size
{
      fseek(fptr,0,SEEK_END);                                   //taking pointer last position to get the size

      int size=ftell(fptr);
      fseek(fptr,0,SEEK_SET);                                    //taking pointer 0th position using seek set
      //printf("secret massage size is %d\n",size);
      return size;
}
Status copy_bmp_header(FILE *fptr_src_image,FILE *fptr_dest_image)      //function defination for copy header file data      
{
  
  char temp_buffer[54];
  rewind(fptr_src_image);

  fread(temp_buffer,1,54,fptr_src_image);
  fwrite(temp_buffer,1,54,fptr_dest_image);                              //writing header file to output image

  //printf("BMP header copied successfully\nposition: %ld\n",ftell(fptr_dest_image));

  return e_success;

}
Status encode_magic_string_size(FILE *fptr_src_image,FILE *fptr_stego_image,long magic_string_size, EncodeInfo *encInfo)
{
unsigned char buffer;                                                                //function defination for encode magic sting size
int i;

for(i=0;i<32;i++)
{
  fread(&buffer,sizeof(unsigned char),1,fptr_src_image);                     //taking 1 byte from src image

  buffer=(buffer&~1)|((magic_string_size>>i)&1);                             //doing some bit operetions

  fwrite(&buffer,sizeof(unsigned char),1,fptr_stego_image);                    //storing in output file

}
 
 
 //printf("Magic sring size copied successfully\nposition: %ld\n",ftell(fptr_stego_image));
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)       //function defination for encoding magic string
{
 int i,j;
 unsigned char temp_buffer;

 for(i=0;i<magic_string_size;i++)
 {
  char value=magic_string[i];                                //storing one by one data in a character variable

  for(j=0;j<8;j++)
  {
  fread(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_src_image);     //reading 1 byte from src image

   temp_buffer=(temp_buffer&0xFE)|((value>>j)&0x01);                            //doing some bit operatin 

  fwrite(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_stego_image); //storing in output image
 }
 
 }
}
Status encode_extn_size(const char *file_extn, EncodeInfo *encInfo)   //function defination for encode secret file extension
{
unsigned char temp_buffer;
int i;
int extn_size=strlen(file_extn);                                   //finding file extention size
//printf("Secret file exten size for encode is %d\n",extn_size);

for(i=0;i<32;i++)
{
  fread(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_src_image);    //taking 1 byte data from src image

  temp_buffer=(temp_buffer&~1)|((extn_size>>i)&1);                      //doing some bit operations

  fwrite(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_stego_image);  //storing in output image

}
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)  //function defination for  encode extention
{
int i,j;
char data;
 unsigned char temp_buffer;

 for(i=0;i<extn_size;i++)
 {
   data=file_extn[i];                                            //storing 1 by 1 data to char veriable
   
  for(j=0;j<8;j++)
  {
  fread(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_src_image);

   temp_buffer=(temp_buffer&~1)|((data>>j)&1);                          //storing data in output file after bit operations

  fwrite(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_stego_image);
 }
 }
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)   //function defination for encode secret file size
{
  unsigned char temp_buffer;
  int i;

for(i=0;i<32;i++)
{
  fread(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_src_image);

  temp_buffer=(temp_buffer&~1)|((file_size>>i)&1);                         //storing data in output file after bit operations

  fwrite(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_stego_image);

}
}
Status encode_secret_file_data(EncodeInfo *encInfo)               //function defination for encode secret file data
{
  int i,j;
 unsigned char temp_buffer;
  char data;
 for(i=0;i<encInfo->size_secret_file;i++)
 {
  fread(&data,sizeof(char),1,encInfo->fptr_secret);             //reading one by one character from secretfile to a variable

  for(j=0;j<8;j++)
  {
  fread(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_src_image);

   temp_buffer=(temp_buffer&~1)|((data>>j)&1);                       //storing the data in output image after bit operation

  fwrite(&temp_buffer,sizeof(unsigned char),1,encInfo->fptr_stego_image);
 }
 }
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest,EncodeInfo *encInfo)   //function defination for encode remain data
{
  char temp_buffer;
 // for(int i=0;i<(encInfo->image_capacity-encode_things_size);i++)
//{
  while(fread(&temp_buffer,sizeof(unsigned char),1,fptr_src)>0)

  fwrite(&temp_buffer,sizeof(unsigned char),1,fptr_dest);             //storing one by one data in output file till eof

//}
  fclose(fptr_src);                                                 //cloing src image file
  fclose(fptr_dest);                                                //closing output file 
  return e_success;
}