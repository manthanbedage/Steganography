#include <stdio.h>
#include "encode.h"
#include "types.h"
#include"decode.h"  
int main(int argc,char*argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if(argc<2)  {printf("INFO : Give Expected inputs:"); return 0;}
    int ret = check_operation_type(argv);
    if(ret==e_encode)
    {     
        {
            printf("Encoding Part start going to Cheak validations.....:\n");
            Status res=read_and_validate_encode_args(argc,argv,&encInfo);
            if(res==e_success)
            {
                printf("INFO : Validations Succesfully Done:");
            }
            if(res==e_failure)
            {
                printf("INFO : Validations Do not Succesfull:");
                return 0;
            }
             Status Enco=do_encoding(&encInfo);
             if(Enco==e_success)
             {
                 printf("\nINFO :# Encoding Done Succesfully #:\n");
             }
             if(Enco==e_failure){
                 printf("\n INFO : Encoding Not Done Succesfully :\n");
                 return 0;
             }
        }
    }
    else if(ret==e_decode)
    {
           printf("Decoding Part start:\n");
           Status ret=validate_Decode(argc,argv,&decInfo);
           if(ret==e_success)
           {
               printf("INFO : Validations Done Succesfully:\n");
           }
           else
           {
               printf("INFO : Validations Failed:\n");
               return 0;
           }
          Status res=Do_Decoding(&decInfo);
          if(res==e_success)
          {
              printf("\nINFO : ##Decoding Done Sucessfully:##\n");
              secret_code(&decInfo);
          }
          if(res==e_failure){
              printf("\nINFO : Decoding Do not Done Sucessfully:\n");
              return 0;
          }
    }
    if(ret==e_unsupported)
    {
        printf("You are giving a wrong info:\n");    
    }
    return 0;
   
}

