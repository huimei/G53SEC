#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define STRSIZE 4096
#define SHIFT 3
#define INSIZE 50
#define BLOCK 8

char * encryptSub(char *);
char * decryptSub(char *);
char * encryptTransp(char *, int[]);
char * decryptTransp(char *, int[]);
void allEncrypt(char *, int[]);
void allDecrypt(char *, int[]);
void readFile(const char *, char *);
void dispMenu();

void dispMenu() {
    printf("=================== USAGE ===================");
    printf("\nTo encrypt a text file, e.g. abc.txt, using:");
    printf("\na) substitution cipher:\n\tSE abc.txt");
    printf("\nb) transposition cipher:\n\tTE abc.txt");
    printf("\nc) product cipher:\n\tPE abc.txt");
    printf("\nd) ALL 3 ciphers:\n\tAE abc.txt");
    printf("\n---------------------------------------------");
    printf("\nTo decrypt a text file, e.g. abc.txt, using:");
    printf("\na) substitution cipher:\n\tSD abc.txt");
    printf("\nb) transposition cipher:\n\tTD abc.txt");
    printf("\nc) product cipher:\n\tPD abc.txt");
    printf("\nd) ALL 3 ciphers:\n\tAD abc.txt");
    printf("\n=============================================\nYour input: ");
}

int main(int argc, char * argv[]) {
    dispMenu();

    char opt[INSIZE];
    char str[STRSIZE];
    char filename[INSIZE];
    srand((int)time(NULL));

    if (scanf("%s %s", opt, filename) < 0)
    {
        printf("Invalid arguments.\n");
        exit(1);
    }

    if (strcmp(opt, "SE") == 0) {
        readFile(filename, str);
        char encryptText[strlen(str)];
        strcpy(encryptText, encryptSub(str));
        printf("Encrypted text: %s\n", encryptText);
        printf("Decrypted text: %s\n", decryptSub(encryptText));
    } else if (strcmp(opt,"SD") == 0) {
        readFile(filename, str);
        char decryptText[strlen(str)];
        strcpy(decryptText, decryptSub(str));
        printf("Decrypted text: %s\n",decryptText);
    } else if (strcmp(opt,"TE") == 0) {
        readFile(filename, str);
        char encryptText[strlen(str)];
        int permutations[BLOCK];
        strcpy(encryptText, encryptTransp(str, permutations));
        printf("Encrypted text: %s\n", encryptText);
        printf("Decrypted text: %s\n", decryptTransp(encryptText, permutations));
    } else if (strcmp(opt,"TD") == 0) {
        readFile(filename, str);
        char decryptText[strlen(str)];
        int permutations[BLOCK]; // for cba.txt only //4,5,6,1,0,7,3,2
        strcpy(decryptText, decryptTransp(str, permutations));
        printf("Decrypted text: %s\n", decryptText);
    } else if (strcmp(opt, "PE") == 0) {
        readFile(filename, str);
        char encryptText[strlen(str)];
        int permutations[BLOCK];
        strcpy(encryptText, encryptTransp(encryptSub(str), permutations));
        printf("Encrypted text: %s\n", encryptText);
        printf("Decrypted text: %s\n", decryptSub(decryptTransp(encryptText, permutations)));
    } else if (strcmp(opt, "PD") == 0) {
        readFile(filename, str);
        char decryptText[strlen(str)];
        int permutations[BLOCK]; // for prodcba.txt only // = {5,6,7,1,0,3,2,4}
        strcpy(decryptText, decryptSub(decryptTransp(str, permutations)));
        printf("Decrypted text: %s\n", decryptText);
    } else if (strcmp(opt, "AE") == 0) {
        readFile(filename, str);
        int permutations[BLOCK];
        allEncrypt(str, permutations);
    } else if (strcmp(opt, "AD") == 0) { // not implemented yet
        readFile(filename, str);
        int permutations[BLOCK];
        allDecrypt(str, permutations);
    } else {
        printf("Invalid argument.\n");
        exit(1);
    }
    return 0;
}

char * encryptSub(char * str) {
    size_t length = strlen(str);
    int i;
    for(i = 0; i < length; i++){
        if (str[i] == ' '){
            str[i] = str[i];
        } else if(str[i]>=48 && str[i]<=57){ // 0 - 9
            str[i] = str[i] + SHIFT;
            if(str[i]>57){
                str[i] = 48 + (str[i] - 58);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=65 && str[i]<=90){ // A - Z
            str[i] = str[i] + SHIFT;
            if(str[i]>90){
                str[i] = 65 + (str[i] - 91);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=97 && str[i]<=122){ // a - z
            str[i] = str[i] + SHIFT;
            if(str[i]>122){
                str[i] = 97 + (str[i] - 123);
            }else{
                str[i] = str[i];
            }
        }
    }
    return str;
}

char * decryptSub(char * str){
    size_t length = strlen(str);
    int i;
    for(i = 0; i < length; i++){
        if (str[i] == ' '){
            str[i] = str[i];
        } else if(str[i]>=48 && str[i]<=57){
            str[i] = str[i] - SHIFT;
            if(str[i]<48){
                str[i] = 58 - (48 - str[i]);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=65 && str[i]<=90){
            str[i] = str[i] - SHIFT;
            if(str[i]<65){
                str[i] = 91 + (65 - str[i]);
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=97 && str[i]<=122){
            str[i] = str[i] - SHIFT;
            if(str[i]<97){
                str[i] = 123 - (97 - str[i]);
            }else{
                str[i] = str[i];
            }
        }
    }
    return str;
}


char * encryptTransp(char * str, int permutations[BLOCK]){
    int length = (int) strlen(str);
    int columns = BLOCK;
    int rows = (int)(ceil((float)length/(float)columns));

    int permutationSequence[columns];

    char cipherBlock[columns][rows];

    int x,y,i,j,zero;

    printf("Length = %d\n", length);
    printf("Columns = %d\n", columns);
    printf("Rows = %d\n", rows);

    i = 0;
    y = 0;

    printf("\nPlain text in matrix:\n");
    while(1){ // loop by row
        if (y == rows){
            break;
        }
        int n = 'a';
        for (x = 0; x < columns; x++){ // filling row by row
            if (i < length){
                cipherBlock[x][y] = str[i];
            } else{
                cipherBlock[x][y] = n++; // filler, abcde..
            }
            i++;
            printf("%c", cipherBlock[x][y]);
        }
        y++;
        printf("\n");
    }

    i = 0;
    j = 0;
    zero = 0;

    printf("\nKey sequence:\n");
    for(i = 0; i < columns; i++){
        int duplicate;
        int permutation;
        // check for duplicates
        while(1){
            duplicate=0;
            permutation = rand() % columns; // 0 to 7

            for(j = 0; j < columns; j++){
                if(permutation == 0 && zero == 0){
                    zero=1;
                    break;
                } else if(permutation == permutations[j]){
                    duplicate = 1;
                }
            }

            if(duplicate==0){
                permutations[i] = permutation;
                permutationSequence[permutation] = i;
                break;
            }
        }
        permutations[i] = permutation;
        printf("%d ", permutations[i]);
    }
    printf("\n");

    i = 0;
    x = 0;
    y = 0;

    for(x = 0; x < columns; x++){
        for(y = 0; y < rows; y++){
            str[i] = cipherBlock[permutationSequence[x]][y];
            i++;
        }
    }
    str[i] = '\0';
    return str;
}

char * decryptTransp(char * str, int permutations[BLOCK]){
    int length = (int)strlen(str);
    int columns = BLOCK;
    int rows = (int)(ceil((float)length/(float)columns));

    char cipherBlock[columns][rows];

    int x,y,i;

    i = 0;

    printf("\nCipher text in matrix:\n");
    for(x = 0; x < columns; x++){
        for(y = 0; y < rows; y++){
            cipherBlock[x][y] = str[i];
            i++;
            printf("%c", cipherBlock[x][y]);
        }
        printf("\n");
    }

    i = 0;

    for(y = 0; y < rows; y++){
        for(x = 0; x < columns; x++){
            str[i] = cipherBlock[permutations[x]][y];
            i++;
        }
    }
    return str;
}

void allEncrypt(char * str, int permutations[BLOCK]) {
    char str1[strlen(str)];
    char str2[strlen(str)];
    char str3[strlen(str)];

    strcpy(str1, str);
    strcpy(str2, str);
    strcpy(str3, str);

    // sub
    printf("Substitution cipher encrypted text: %s\n\n", encryptSub(str1));
    // transp
    int permutations1[BLOCK];
    printf("Transposition cipher encrypted text: %s\n\n", encryptTransp(str2, permutations1));
    // prod
    int permutations2[BLOCK];
    printf("Product cipher encrypted text: %s\n", encryptTransp(encryptSub(str3), permutations2));
}

void allDecrypt(char * str, int permutations[BLOCK]) {

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

