#include <stdio.h>
#include<string.h>
#include<unistd.h>
#include "encode.h"
#include "types.h"
#include"common.h"
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    fseek(fptr_image, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, fptr_image);
    fread(&height, sizeof(int), 1, fptr_image);
    return width * height * 3;
}

Status open_files(EncodeInfo *encInfo)
{
    
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file0 %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file1 %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file2 %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    return e_success;
}
OperationType check_operation_type(char *argv[])
{
    if(!strcmp(argv[1],"-e"))
        return e_encode;
        else if(!strcmp(argv[1],"-d"))
            return e_decode;
            else
                return e_unsupported;
}
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo)//Read and validate the encoding arguments 
{
    if((argv[3]!=NULL&&argv[4]==NULL)||(argv[4]!=NULL&&argv[5]==NULL)&&argc>=3)
    {
        
        char *ret = strstr(argv[2], ".bmp");
        if(ret==NULL) return e_failure;
        int i = strcmp(ret, ".bmp");
        if(i)
        {
            printf("You are Not Giving the input file as .bmp file\n");
            return e_failure;
       }
       if(!strstr(argv[3],"."))
       {
           printf("You are Not Giving the secret file as not .  file\n");
           return e_failure;
       }
       char *ext = strstr(argv[3], ".");
       strcpy(encInfo->extn_secret_file,ext);
       if (argv[4] != NULL)
       {
           char *ptr = strstr(argv[4], ".bmp");
           int i = (strcmp(ptr, ".bmp"));
           if(i)
           {
               printf("You are Not Giving the output file as .bmp file\n");
               return e_failure;
           }
       }
       encInfo->src_image_fname=argv[2];
       encInfo->secret_fname=argv[3];
       if (argv[4] != NULL)
       {
           encInfo->stego_image_fname=argv[4];
       }
       else
       {
           encInfo->stego_image_fname="stego_img.bmp";
           printf("INFO : Output File Not mentioned Ceating stego_img.bmp as a Default:\n");
       }
       return e_success;
    }
    else    
    {
        printf("INFO : Too Few Arguments......\n");
        printf("This is the Format : ./a.out -e inputfile.bmp secretfile.txt\n");
        return e_failure;
    }
}
Status do_encoding(EncodeInfo *encInfo)//Encoding part Starts
{
    Status i=open_files(encInfo);
    if(i==e_success)
    {
        printf("\nINFO : Opening Requriend Files :\n");
        printf("INFO : Done \n");
    }
    else
    {
        printf("Files Do not Opens:\n");
        return e_failure;
    }
    uint beautifulsize = get_image_size_for_bmp(encInfo->fptr_src_image);//function call for image size
    encInfo->image_capacity = beautifulsize;//Saving beautiful sizeinto th =e image capacity
    Status capacity = check_capacity(encInfo);//function call for cheaking the size
    if(capacity==e_failure)
    {
        printf("INFO : Cheking for beautiful.bmp capacity to handle %s Not capable:\n",encInfo->secret_fname);
        return e_failure;
    }
        printf("INFO : Cheking for beautiful.bmp capacity to handle %s:\n",encInfo->secret_fname);
        printf("INFO : Done\n");

    copy_bmp_header(encInfo);   // Function for copying a header Data
  
    printf("INFO : Copying Header Data:\n");
    printf("INFO ; Done\n");

    encode_magic_string(MAGIC_STRING, encInfo); // Function for magic string Encoding

    printf("INFO : Encode a Magic string:\n");
    printf("INFO : Done\n");

    int size = strlen(encInfo->extn_secret_file);
    encode_secret_file_extnsize(size,encInfo); // Function for size of secret file  its Encoding
    
    printf("INFO : Encoding a Secret file size\n");
    printf("INFO : Done\n");

    encode_secret_file_extn(encInfo); // function for has extension
  
    printf("INFO : Encoding a Secret file extension\n");
    printf("INFO : Done\n");
    int size_of_data = ftell(encInfo->fptr_secret);
    encode_data_size_of_sec_file(size_of_data,encInfo);
    printf("INFO : Encoding a Size of Data file \n");
    printf("INFO : Done\n");

    encode_secret_file_data(encInfo);//Function for Encoding a Data
    printf("INFO : Encoding a Data\n");
    printf("INFO : Done\n");

    copy_remaining_img_data(encInfo);//copying a remening data
    
    printf("INFO : Encoding a remaning Data\n");
    printf("INFO : Done\n");
    fclose(encInfo->fptr_src_image);  
    
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
    for (int i = 0; i < 101;i++)
    {
        printf("Loading........%d%%\r", i);
        usleep(75000);
        fflush(stdout);
    }

        return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    uint sizeof_file= get_file_size(encInfo->fptr_secret,encInfo);      
    if (encInfo->image_capacity > sizeof_file)
    {
        return e_success;
    }   
}
uint get_file_size(FILE *fptr,EncodeInfo *encInfo)
{
    int slength =  strlen(MAGIC_STRING)* sizeof(char) * 8; // length of magic string
    char *length_extsec=strstr(encInfo->secret_fname, ".");//cheaking for condition

    strcpy(encInfo->extn_secret_file,length_extsec);

    int l = strlen(length_extsec);
    int lengthsecext = sizeof(l) * 8;  //length of secret extention file
    int extdatasize = l * 8;   //length of extdatasize  
    int file_size = 4 * 8;
    fseek(encInfo->fptr_secret, 0, SEEK_END);
    int file_length=ftell(encInfo->fptr_secret);
    int file = file_length * sizeof(char) * 8;
    int sizeof_file = slength + lengthsecext + extdatasize + file_size + file + 54;
    return sizeof_file;
}
Status copy_bmp_header(EncodeInfo *encInfo)//Function for copying Header
{
    rewind(encInfo->fptr_src_image);
    char buff[54];
    fread(buff, 1, 54, encInfo->fptr_src_image);
    fwrite(buff, 1, 54, encInfo->fptr_stego_image);   
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)//Function for encoding a magic string 
{
    char buff[8];
    for (int i = 0; i < strlen(magic_string); i++)

    {
        fread(buff, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],buff);
        fwrite(buff, 1, 8, encInfo->fptr_stego_image);
    }   
}
Status encode_byte_to_lsb(char data, char *image_buffer)//function for magic sting char by char
{
    for (int i = 0; i < 8;i++)
    {
        image_buffer[i] = (image_buffer[i] & ~1) | ((data >> i)&1);
    }
}
Status encode_secret_file_extnsize(int size,EncodeInfo *encInfo)//extension size cheak and give
{
   
    char sec_buffer[32];
    fread(sec_buffer, 1, 32, encInfo->fptr_src_image);
    encode_byte_to_lsb_ext(size, sec_buffer);
    fwrite(sec_buffer, 1, 32, encInfo->fptr_stego_image);
}
Status encode_byte_to_lsb_ext(int data, char *sec_buffer)//encode the 32 byte 
{
    for (int i = 0; i < 32;i++)
    {
     sec_buffer[i]= (sec_buffer[i] & ~1) | ((data >> i)&1);  
    }
}
Status encode_secret_file_extn(EncodeInfo *encInfo)//For Extension Copy
{
   char buffer[8];
   for (int i = 0; i <strlen(encInfo->extn_secret_file);i++)
   {
       fread(buffer, 1, 8, encInfo->fptr_src_image);
       encode_byte_to_lsb(encInfo->extn_secret_file[i], buffer);
       fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
   }
}
Status encode_data_size_of_sec_file(int size_of_data,EncodeInfo *encInfo)
{
    
    char fsize_buff[32];
        fread(fsize_buff, 1, 32, encInfo->fptr_src_image);
        encode_byte_to_lsb_ext(size_of_data, fsize_buff);
        fwrite(fsize_buff, 1, 32, encInfo->fptr_stego_image);
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int length = ftell(encInfo->fptr_secret);
    char arr[length];
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    fread(arr,1,length, encInfo->fptr_secret);
    char sec_buff[8];
    for (int i = 0; i <= length;i++)
    {
        fread(sec_buff, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(arr[i], sec_buff);
        fwrite(sec_buff, 1, 8, encInfo->fptr_stego_image);
    }
}
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    char rem_buff[1];
    while (fread(rem_buff,1,1,encInfo->fptr_src_image))
    {
        fwrite(rem_buff, 1, 1, encInfo->fptr_stego_image);
    }
}
