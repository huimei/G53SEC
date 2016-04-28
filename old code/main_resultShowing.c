#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define STRSIZE 4096
#define SHIFT 3
#define INSIZE 50
#define FACMAX 362880
#define BLOCKMAX 9

char * decryptTransp(char *, int[], int);
int * decryptTranspNoKey(char *, int[], int);
void permute(int *, int, int, int);
void swap(int *, int *);
void readFile(const char *, char *);
void dispMenu();



int permset[FACMAX][BLOCKMAX];
int k = 0;

void dispMenu() {
    printf("=================== USAGE ===================");
    printf("\nTo decrypt a text file, e.g. abc.txt, using:");
    printf("\nfor transposition cipher:\n\tTD abc.txt");
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

    if (strcmp(opt, "TD") == 0) { // Transp decrypt
        readFile(filename, str);
        char decryptText[strlen(str)];
        int permutations[BLOCKMAX];
        int blockSize;
        int * key = decryptTranspNoKey(str, permutations, blockSize);
        strcpy(decryptText, str);
        printf("Decrypted text:\n%s\n", decryptText);
    }else {
        printf("Invalid argument.\n");
        exit(1);
    }
    return 0;
}

char * decryptTransp(char * str, int permutations[BLOCKMAX], int block){
    int length = (int)strlen(str);
    int columns = block;
    //Calculate number of rows needed
    int rows = (int)(ceil((float)length/(float)columns));

    char cipherBlock[columns][rows];

    int x,y,i;

    i = 0;
    //Assign cipher text into matrix of rows*column
    for(x = 0; x < columns; x++){
        for(y = 0; y < rows; y++){
            cipherBlock[x][y] = str[i];
            i++;
        }
    }

    i = 0;
    //Reassign string based on permutations order
    for(y = 0; y < rows; y++){
        for(x = 0; x < columns; x++){
            str[i] = cipherBlock[permutations[x]][y];
            i++;
        }
    }
    return str;
}

int * decryptTranspNoKey(char * str, int permutations[BLOCKMAX], int blockSize) {

    int i, found = 0;
    int run = 0;

    size_t length = strlen(str);
    char str2[length];

    int x;

    //Minimum block size allowed is 2, and maximum block size allowed is 9
    for (x = 2; x <= 9; x++){

        int block = x;

        for (i = 0; i < block; i++) { //Fill with 0-7
            permutations[i] = i;
        }

        int fac = 1;

        int y;
        //Calculate the factorial of selected block size
        for (y = block; y > 0; y--){
            fac *= y;
        }

        //Find n! permutations
        permute(permutations, 0, block, fac);

        //Loop through all possible combinations
        for (i = 0; i < fac; i++) {
            strcpy(str2, str);
            //Decrypt cipher text based on selected combination in this run
            decryptTransp(str2, permset[i], x);
            int j;
            for (j = 0; j < length-8; j++) { //Find 'computer' or 'security'
                if ((str2[j]=='c' && str2[j+1]=='o' && str2[j+2]=='m' && str2[j+3]=='p'
                    && str2[j+4]=='u' && str2[j+5]=='t' && str2[j+6]=='e' && str2[j+7]=='r' ) ||
                    (str2[j]=='s' && str2[j+1]=='e' && str2[j+2]=='c' && str2[j+3]=='u'
                    && str2[j+4]=='r' && str2[j+5]=='i' && str2[j+6]=='t' && str2[j+7]=='y'))
                    found = 1;
            }
            run ++;
            printf("\n\n");
            printf("Block Size: %d || ", x);
            printf("Run Number: %d\n", run);
            printf("Decrypted String:\n%s", str2);
            if (found){
                break;
            }
        }
        blockSize = x;
        if (found){
            break;
        }
    }

    printf("\n\n");
    printf("Number of runs: %d\n", run);
    printf("Block size found: %d", blockSize);
    printf("\n");
    printf("Permutations: ");
    int *perm = permset[i];
    for (i = 0; i < blockSize; i++){
        printf("%d ", perm[i]);
    }
    printf("\n\n");
    strcpy(str, str2);
    return permset[i];
}


void permute(int * per, int i, int length, int fac){
    int j;
    if (length == i) {
        //Store to permset
        for (i = 0; i < length; i++) {
            if (k < fac)
                permset[k][i] = per[i];
        }
        k++;
    } else {
        j = i;
        for (j = i; j < length; j++)
        {
            swap((per+j), (per+i));
            permute(per, i+1, length, fac);
            swap((per+i), (per+j)); //Backtrack
        }
    }
}

void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
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


