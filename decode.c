#include<stdio.h>
#include<string.h>
#include"decode.h"
#include"types.h"
Status open_files_Decode(DecodeInfo *decInfo)//Functions For open the Files
{
    decInfo->fptr_output_file = fopen(decInfo->output_File_name, "r");
    if(decInfo->fptr_output_file == NULL)
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file0 %s\n", decInfo->output_File_name);
    	return e_failure;   
    }
     return e_success;
}
Status validate_Decode(int argc, char *argv[],DecodeInfo* decInfo)//Validations
{
    if(argc==3||argc==4)
    {
       
        char *str = strstr(argv[2], ".bmp");
        if(str==NULL)
        {
            printf("Its Not a bmp file");
            return e_failure;
        }
        int i = strcmp(str, ".bmp");
        if (i)
        {
            printf("\nGiving file is not a bmp file\n");
            return e_failure;
       }
       decInfo->output_File_name = argv[2];
       
       if (argv[3] != NULL)
       {
           strcpy(decInfo->stego_output_text,argv[3]);
           printf("INFO : output file is  Mentioned Creating %s \n",decInfo->stego_output_text);
       }
       else
       {
       strcpy(decInfo->stego_output_text,"stego_output");
       printf("INFO : output file is Not Mentioned.Creating stego_output %s as default\n",decInfo->stego_output_text);
       }
       return e_success;
    }
    else
    {
        printf("INFO : Too Few Arguments....\n");
        printf("INFO : This is the Format ./a.out -d sourcefile \n");
    }
}
Status Do_Decoding(DecodeInfo* decInfo)//Decoding Function
{
    Status i = open_files_Decode(decInfo);
    if(i==e_success)
    {
        printf("INFO : Opening Requriend Files :\n");
        printf("INFO : Done :\n");
    }
    if(i==e_failure){
        printf("Files Not Open:");
        return e_failure;
    }
    char magic_str[10];// user given 
    char match[10];// decoded stored
    printf("INFO : Enter the Magic String:\n"); // Magic string takes from a user;
    scanf("%[^\n]", magic_str);
    printf("INFO : Decoding magic string;");
    magic_string_decode(decInfo, match); // magic string decode
    if (strcmp(magic_str, match))
    {
        printf("INFO : Magic String Does not Match\n");
        return e_failure;
    }

    decode_file_ext_size(decInfo);//file extension size decode
    printf("\nINFO : Decoding %s file size ", decInfo->output_File_name);
    printf("\nINFO : Done");

    decode_file_ext_(decInfo);//file extension decode
    printf("\nINFO : Decoding %s file Extension", decInfo->output_File_name);
    printf("\nINFO : Done");

    strcat(decInfo->stego_output_text, decInfo->ext_out);
    decInfo->fptr_output_text = fopen(decInfo->stego_output_text, "w");
    if (decInfo->fptr_output_text == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file1 %s\n", decInfo->stego_output_text);

        return e_failure;   
     }

    decode_main_data_size(decInfo);//Decode main data size
    printf("\nINFO : Decoding %s file Data", decInfo->output_File_name);
    printf("\nINFO : Done");

    decode_main_data(decInfo);
    fclose(decInfo->fptr_output_text);
    return e_success;
}
Status magic_string_decode(DecodeInfo* decInfo,char* match)//Magic string Decoding
{
    fseek(decInfo->fptr_output_file, 54, SEEK_SET);
    char buffer_out[8];
    char ch = 0;
    int i = 0;
    for (i; i < 2; i++)
    {
        fread(buffer_out, 1, 8, decInfo->fptr_output_file);
        decode_char(buffer_out, &ch);
        match[i] = ch;
    }
    match[i] = '\0';
}
/*Decoding char by char data*/
Status decode_char(char *buffer_out, char* ch)
{
    *ch = 0;
    for (int i = 0; i < 8; i++)
    {
        *ch = *ch | (buffer_out[i] & 1) << i;
    }
}
Status decode_file_ext_size(DecodeInfo* decInfo)//Decoding The File Extension size
{
    char buff_size[32];
    int num = 0;
    fread(buff_size, 1, 32, decInfo->fptr_output_file);
    decode_file_size_int(buff_size,&num);
    decInfo->output_ext_size = num;
}
Status decode_file_size_int(char* buff_size,int *num)//Decode the size of data
{
    for (int i = 0; i < 32; i++)
    {
        *num = *num | (buff_size[i] & 1) << i;
    }
}
Status decode_file_ext_(DecodeInfo* decInfo)//Decoding the File Extension
{
    char buff_ext[8];
    char ch = 0;
    for (int i = 0; i<decInfo->output_ext_size;i++)
        {
            fread(buff_ext, 1, 8, decInfo->fptr_output_file);
            decode_char(buff_ext, &ch);
            decInfo->ext_out[i] = ch; //Saving a extension char by char
        }
}
Status decode_main_data_size(DecodeInfo* decInfo)//Decoding the main data size
{
    char buff_size[32];
    int num = 0;
    fread(buff_size,1,32,decInfo->fptr_output_file);
    decode_file_size_int(buff_size, &num);
    decInfo->output_data_size = num;
}
Status decode_main_data(DecodeInfo* decInfo)//Decoding the Main Data
{
    char buff_data[8];
    char ch = 0;
    for (int i = 0; i < decInfo->output_data_size;i++)
    {
        fread(buff_data, 1, 8, decInfo->fptr_output_file);
    decode_char(buff_data, &ch);
    fwrite(&ch, 1, 1, decInfo->fptr_output_text);
    }
}
void secret_code(DecodeInfo* decInfo)//Displaying the Secreat Code
{
    char str[100];
    decInfo->secret_code = fopen(decInfo->stego_output_text, "r");
    fscanf(decInfo->secret_code,"%[^\n]", str);
    printf("Your Secreat Message is : %s\n",str);
    fclose(decInfo->secret_code);
}