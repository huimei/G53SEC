#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define STRSIZE 4096
#define SHIFT 20
#define INSIZE 50

void encryptMsg(char*);
void decryptMsg(char*);
void readFile(const char*, char*);

int main(int argc, char * argv[]) {
    printf("=================== USAGE ===================\nTo encrypt a text file, e.g. abc.txt, input:\n");
    printf("\tE abc.txt\nTo decrypt a text file, e.g. abc.txt, input:\n");
    printf("\tD abc.txt\n=============================================\nYour input: ");

    char opt;
    char str[STRSIZE];
    char filename[INSIZE];

    if (scanf("%c %s", &opt, filename) < 0)
    {
        printf("Invalid arguments.\n");
        exit(1);
    }
    opt = toupper(opt);
    if (opt == 'E') {
        readFile(filename, str);
        encryptMsg(str);
    } else if (opt == 'D') {
        readFile(filename, str);
        decryptMsg(str);
    } else {
        printf("Invalid argument.\n");
        exit(1);
    }
    return 0;
}

void encryptMsg(char * str) {
    size_t length = strlen(str);
    int shift = SHIFT;
    int i;

    for(i = 0; i < length; i++){
        if (str[i] == ' '){
            str[i] = str[i];
        } else if(str[i]>=48 && str[i]<=57){
            str[i] = str[i] + shift;
            if(str[i]>57){
                str[i] = 48 + (str[i] - 58);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=65 && str[i]<=90){
            str[i] = str[i] + shift;
            if(str[i]>90){
                str[i] = 65 + (str[i] - 91);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=97 && str[i]<=122){
            str[i] = str[i] + shift;
            if(str[i]>122){
                str[i] = 97 + (str[i] - 123);
            }else{
                str[i] = str[i];
            }
        }
    }
    printf("\nEncrypted: %s\n\n", str);
    decryptMsg(str);
}

void decryptMsg(char * str){
    size_t length = strlen(str);
    int shift = SHIFT;
    int i;

    for(i = 0; i < length; i++){
        if (str[i] == ' '){
            str[i] = str[i];
        } else if(str[i]>=48 && str[i]<=57){
            str[i] = str[i] - shift;
            if(str[i]<48){
                str[i] = 58 - (48 - str[i]);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=65 && str[i]<=90){
            str[i] = str[i] - shift;
            if(str[i]<65){
                str[i] = 91 + (65 - str[i]);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=97 && str[i]<=122){
            str[i] = str[i] - shift;
            if(str[i]<97){
                str[i] = 123 - (97 - str[i]);
            }else{
                str[i] = str[i];
            }
        }
    }
    printf("Decrypted: %s\n\n", str);
}

void readFile(const char * filename, char * str) {
    FILE *fp;
    long length;
    char *buffer;
    fp = fopen (filename,"r");
    if(!fp) perror(filename),exit(1);

    fseek(fp,0L,SEEK_END);
    length = ftell(fp);
    rewind( fp );

    //allocate memory
    buffer = calloc( 1, length+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc failed",stderr),exit(1);

    // copy file into buffer
    if( 1!=fread( buffer , length, 1 , fp) )
        fclose(fp),free(buffer),fputs("read failed",stderr),exit(1);
    fclose(fp);
    strcpy(str, buffer);
    free(buffer);
}







