#ifndef DECODE_H
#define DECODE_H
#include<stdio.h>
#include"types.h"

typedef struct DecodeInfo
{
    /*output file in which our secret code is present*/
    char *output_File_name;//output file save in which data is present
    FILE *fptr_output_file;
    uint output_ext_size; // extension size of sec file 
    char ext_out[5];//extension of sec file
    uint output_data_size;

    /*output file txt*///IN which we are storing the data
    char stego_output_text[20];
    FILE *fptr_output_text;
    FILE *secret_code;

} DecodeInfo;

/*To Validate a Decode */
Status validate_Decode(int argc, char *argv[],DecodeInfo* decInfo);

/*De-Coding function*/
Status Do_Decoding(DecodeInfo* decInfo);
/*To open Files*/
Status open_files_Decode(DecodeInfo *decInfo);

/*Decode A Magic String*/
Status magic_string_decode(DecodeInfo *decInfo,char* match);

/*Decode a char by char in string Main logic */
Status decode_char(char *buffer_out,char *ch);

/*find a file text size*/
Status decode_file_ext_size(DecodeInfo *decInfo);

/*decode a size Main logic to find the decoding*/
Status decode_file_size_int(char *buff_size, int *num);

/*Deode a file ext*/
Status decode_file_ext_(DecodeInfo *decInfo);

/* Decode a main data size secreat code*/
Status decode_main_data_size(DecodeInfo *decInfo);

/*Decode a main Data */
Status decode_main_data(DecodeInfo *decInfo);

/*For printing A secret Code*/
void secret_code(DecodeInfo *decInfo);

#endif
