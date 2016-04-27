#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define STRSIZE 4096
#define SHIFT 21
#define INSIZE 50
#define BLOCK 8
#define FAC8 40320

char * encryptSub(char *, int);
char * decryptSub(char *, int);
int decryptSubNoKey(char *); // frequency analysis
int decryptSubNoKey2(char *); // exhaustive search
char * encryptTransp(char *, int[]);
char * decryptTransp(char *, int[]);
int * decryptTranspNoKey(char *, int[]);
char * decryptProdNoKey (char *, int[]);
void permute(int *, int, int);
void swap(int *, int *);
void allEncrypt(char *, int[]);
void readFile(const char *, char *);
void dispMenu();



int permset[FAC8][BLOCK];
int k = 0;

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

    if (strcmp(opt, "SE") == 0) { //Sub encrypt
        readFile(filename, str);
        char encryptText[strlen(str)];
        strcpy(encryptText, encryptSub(str, SHIFT));
        printf("\n");
        printf("Encrypted text:\n%s\n\n", encryptText);
        printf("Decrypted text:\n%s\n\n", decryptSub(encryptText, SHIFT));
    } else if (strcmp(opt,"SD") == 0) { //Sub decrypt without key
        readFile(filename, str);
        char string [strlen(str)];
        strcpy(string,str);

        //Frequency analysis attack
        printf("\n");
        printf("Run with frequency analysis attack:\n");
        char decryptText1[strlen(str)];
        int key1 = decryptSubNoKey(string);
        strcpy(decryptText1, decryptSub(str,key1));
        printf("Key: %d\n\nDecrypted text:\n%s\n\n",key1, decryptText1);

        //Exhaustive search attack
        printf("Run with exhaustive search attack:\n");
        char decryptText2[strlen(str)];
        int key2 = decryptSubNoKey2(string);
        strcpy(decryptText2, string);
        printf("Key: %d\n\nDecrypted text:\n%s\n\n",key2, decryptText2);
    } else if (strcmp(opt,"TE") == 0) { //Transp encrypt
        readFile(filename, str);
        char encryptText[strlen(str)];
        int permutations[BLOCK];
        strcpy(encryptText, encryptTransp(str, permutations));
        printf("\n");
        printf("Encrypted text:\n%s\n\n", encryptText);
        printf("Decrypted text:\n%s\n\n", decryptTransp(encryptText, permutations));
    } else if (strcmp(opt,"TD") == 0) { //Transp decrypt without key
        readFile(filename, str);
        char decryptText[strlen(str)];
        int permutations[BLOCK];
        int * key = decryptTranspNoKey(str, permutations);
        strcpy(decryptText, str);
        int i;
        printf("\n");
        printf("Permutations: ");
        for (i = 0; i < BLOCK; i++){
            printf("%d ", key[i]);
        }
        printf("\n\n");
        printf("Decrypted text:\n%s\n", decryptText);
    } else if (strcmp(opt, "PE") == 0) { //Prod encrypt
        readFile(filename, str);
        char encryptText[strlen(str)];
        int permutations[BLOCK];
        strcpy(encryptText, encryptTransp(encryptSub(str, SHIFT), permutations));
        printf("\n");
        printf("Encrypted text:\n%s\n\n", encryptText);
        printf("Decrypted text:\n%s\n\n", decryptSub(decryptTransp(encryptText, permutations), SHIFT));
    } else if (strcmp(opt, "PD") == 0) { //Prod decrypt without key
        readFile(filename, str);
        char decryptText[strlen(str)];
        int permutations[BLOCK];
        strcpy(decryptText, decryptProdNoKey(str, permutations));
        printf("\n");
        printf("Decrypted text:\n%s\n\n", decryptText);

    } else if (strcmp(opt, "AE") == 0) { //All encrypt & decrypt with key
        readFile(filename, str);
        int permutations[BLOCK];
        allEncrypt(str, permutations);
    } else {
        printf("Invalid argument.\n");
        exit(1);
    }
    return 0;
}

char * encryptSub(char * str, int shift) {
    size_t length = strlen(str);
    int i;
    shift = shift % 26; //Make sure key is smaller than 26

    for(i = 0; i < length; i++){
        if (str[i] == ' '){ //Keep space
            str[i] = str[i];
        } else if(str[i]>=48 && str[i]<=57){ //0 - 9
            int shf = shift % 10; //Reduce the number of shift for number to 10 (total number of number characters)
            str[i] = str[i] + shf;
            if(str[i]>57){
                str[i] = (str[i] - 58) + 48;
            }
        } else if(str[i]>=65 && str[i]<=90){ //A - Z
            str[i] = str[i] + shift;
            if(str[i]>90){
               str[i] = (str[i] - 91) + 65;
            }else{
                str[i] = str[i];
            }
        } else if(str[i]>=97 && str[i]<=122){ //a - z
            int temp;
            temp = str[i] + shift; //Temp is used to prevent character fall into extended character after shift
            if(temp > 122){
                str[i] = (temp - 123) + 97;
            } else {
                str[i] = temp;
            }
        }
    }
    return str;
}

char * decryptSub(char * str, int shift){
    size_t length = strlen(str);
    int i;
    for(i = 0; i < length; i++){
        if (str[i] == ' '){ //Keep space
            str[i] = str[i];
        } else if(str[i]>=48 && str[i]<=57){ //0 - 9
            int shf = shift % 10; //Reduce number of shift to <10
            str[i] = str[i] - shf;
            if(str[i]<48){
                str[i] = 58 - (48 - str[i]);
            }
        } else if(str[i]>=65 && str[i]<=90){ //A - Z
            str[i] = str[i] - shift;
            if(str[i]<65){
                str[i] = 91 - (65 - str[i]);
            }
        } else if(str[i]>=97 && str[i]<=122){ //a - z
            str[i] = str[i] - shift;
            if(str[i]<97){
                str[i] = 123 - (97 - str[i]);
            }
        }
    }
    return str;
}

int decryptSubNoKey(char * string) {
    char str[STRSIZE];
    size_t length = strlen(string);
    int l;

    //Change all character to lower case for easier search
    for (l=0; l<length; l++){
        str[l] = tolower(string[l]);
    }

    //Calculate frequency of each character
    int count[26] = {0};
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z' ){
            count[str[i]-'a']++;
        }
        i++;
    }

    //Get the highest count character
    int highest, position = -1;
    char highestAlph;
    i = 0;

    highest = count[i];

    for (i = 0; i < 26; i++){
        if (count[i] > highest){
            highest = count[i];
            position = i;
        }
    }

    highestAlph = position + 'a';

    printf("Highest frequency: %d\n", highest);
    printf("Highest frequency letter: %c\n", highestAlph);

    //Calculate number of words in string
    int wordCount = 0;

    for (i = 0; str[i] != '\0'; i++){
        if (str[i] == ' '){
            wordCount++;
        }
    }
    wordCount = wordCount + 1;
    printf("Number of words in given string are: %d\n", wordCount);

    //Break string into array of words
    char *array[wordCount];
    char s[length];

    strcpy(s, str);
    i = 0;

    array[i] = strtok(s," ");

    while(array[i]!=NULL){
        array[++i] = strtok(NULL," ");
    }

    int j;
    int lengthEight = 0;

    //Get how many words have length of 8
    for (j = 0; j < wordCount; j++){
        if (strlen(array[j])==8){
            lengthEight ++;
        }
    }

    char *stringEight [lengthEight];
    i = 0;

    //Copy word with length of 8 to new array
    for (j = 0; j < wordCount; j++){
        if (strlen(array[j])==8){
            stringEight[i] = array[j];
            i++;
        }
    }

    char alphabet[26] = {'e','t','a','o','i','n','s','h','r','d','l','c','u','m','w','f','g','y','p','b','v','k','j','x','q','z'};
    int key = 0;
    char tmp[8];
    int flag = 0;

    for (i=0; i<26; i++){

        //Start shift guessing from the most frequent used character to the least
        key = (int)highestAlph - alphabet[i];

        if (key < 0){
            key = key + 26;
        }else{
            key = key;
        }

        for (j=0; j<lengthEight; j++){
            strcpy(tmp, stringEight[j]);

            int z;
            //Perform shift on word with length of 8
            for (z=0; z<8; z++){
                tmp[z] = tmp[z] - key;
                if(tmp[z]<97){
                    tmp[z] = 123 - (97 - tmp[z]);
                }else{
                    tmp[z] = tmp[z];
                }
            }

            //Check if keyword 'computer' or 'security' is found
            if ((strcmp("computer",tmp)==0) || (strcmp("security",tmp)==0)){
                //If key word is found, break the loop
                key = key;
                flag = 1;
                break;
            }
        }
        if (flag){
            break;
        }
    }

    printf("Number of runs: %d\n", i+1);

    return key;
}

int decryptSubNoKey2(char * str) {
    size_t length = strlen(str);
    int i,found = 0;
    char str2[length];

    //Loop through all possible shift to find key word
    for (i = 0; i < 26; i++) {
        strcpy(str2, str);

        //Decrypt cipher text based on shift used in loop
        decryptSub(str2, i);

        int j;
        //Check through decrypted text if keyword 'computer' is found
        for (j = 0; j < length-8; j++) {
            //If keyword 'computer' is found, break the loop
            if (str2[j]=='c' && str2[j+1]=='o' && str2[j+2]=='m' && str2[j+3]=='p'
                && str2[j+4]=='u' && str2[j+5]=='t' && str2[j+6]=='e' && str2[j+7]=='r')
                found = 1;
        }
        if (found){
            break;
        };
    }
    strcpy(str, str2);
    printf("Number of runs: %d\n", i+1);
    return i;
}

char * encryptTransp(char * str, int permutations[BLOCK]){
    int length = (int) strlen(str);
    int columns = BLOCK;
    //Calculate the number of rows needed
    int rows = (int)(ceil((float)length/(float)columns));

    int permutationSequence[columns];

    char cipherBlock[columns][rows];

    int x,y,i,j,zero;

    printf("\n");
    printf("Length = %d\n", length);
    printf("Columns = %d\n", columns);
    printf("Rows = %d\n", rows);

    i = 0;
    y = 0;

    printf("\n");
    printf("Plain text in matrix:\n");
    while(1){ //Loop by row
        if (y == rows){
            break;
        }
        int n = 'a';
        for (x = 0; x < columns; x++){ //Filling row by row
            if (i < length){
                cipherBlock[x][y] = str[i];
            } else{
                cipherBlock[x][y] = n++; //Filler, abcde..
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

    printf("\n");
    printf("Key sequence:\n");
    for(i = 0; i < columns; i++){
        int duplicate;
        int permutation;
        //Check for duplicates
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

    //Reassign string based on permutations of columns
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

int * decryptTranspNoKey(char * str, int permutations[BLOCK]) {

    int i, found = 0;
    for (i = 0; i < BLOCK; i++) { //Fill with 0-7
        permutations[i] = i;
    }
    size_t length = strlen(str);
    char str2[length];

    //Find n! permutations
    permute(permutations, 0, BLOCK);

    //Loop through all possible combinations
    for (i = 0; i < FAC8; i++) {
        strcpy(str2, str);
        //Decrypt cipher text based on selected combination in this run
        decryptTransp(str2, permset[i]);

        int j;
        for (j = 0; j < length-8; j++) { //Find 'computer' or 'security'
            if ((str2[j]=='c' && str2[j+1]=='o' && str2[j+2]=='m' && str2[j+3]=='p'
                 && str2[j+4]=='u' && str2[j+5]=='t' && str2[j+6]=='e' && str2[j+7]=='r' ) ||
                (str2[j]=='s' && str2[j+1]=='e' && str2[j+2]=='c' && str2[j+3]=='u'
                 && str2[j+4]=='r' && str2[j+5]=='i' && str2[j+6]=='t' && str2[j+7]=='y'))
                found = 1;
        }
        if (found){
            break;
        }
    }
    printf("\n");
    printf("Number of runs: %d\n", i);
    strcpy(str, str2);
    return permset[i];
}

char* decryptProdNoKey (char * string, int permutations[BLOCK]){
    size_t length = strlen(string);
    char str[STRSIZE];
    int l;
    int row = 0;
    int run = 0;

    //Lower case all character for easier attack
    for (l=0; l<length; l++){
        str[l] = tolower(string[l]);
    }
    str[l] = '\0';

    //Calculate frequency of each character
    int count[26] = {0};
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z' ){
            count[str[i]-'a']++;
        }
        i++;
    }

    //Get the highest count character
    int highest, position = -1;
    char highestAlph;
    i = 0;

    highest = count[i];

    for (i = 0; i < 26; i++){
        if (count[i] > highest){
            highest = count[i];
            position = i;
        }
    }

    highestAlph = position + 'a';

    printf("\n");
    printf("Highest frequency: %d\n", highest);
    printf("Highest frequency letter: %c\n", highestAlph);

    char alphabet[26] = {'e','t','a','o','i','n','s','h','r','d','l','c','u','m','w','f','g','y','p','b','v','k','j','x','q','z'};
    int key = 0;
    int j;
    char tmp[strlen(str)];
    int flag = 0;

    for (i = 0; i < 26; i++){

        //Shift guessing start from the most frequent used character to the least
        key = (int)highestAlph - alphabet[i];

        if (key < 0){
            key = key + 26; //If highest frequency character is smaller than target character, loop back with +26
        }else{
            key = key;
        }

        char string_1[] = "computer";
        char string_2[] = "security";
        int z;

        //Encrypt keywords with selected shift in this run
        for (z = 0; z < 8; z++){
            int tmpo1;
            tmpo1 = string_1[z] + key;
            if(tmpo1 > 122){
                string_1[z] = (tmpo1 - 123) + 97;
            }else{
                string_1[z] = tmpo1;
            }
        }
        for (z = 0; z < 8; z++){
            int tmpo2;
            tmpo2 = string_2[z] + key;
            if(tmpo2 > 122){
                string_2[z] = (tmpo2 - 123) + 97;
            }else{
                string_2[z] = tmpo2;
            }
        }


        for (z = 0; z < BLOCK; z++) { //Fill with 0-7
            permutations[z] = z;
        }

        //Find n! permutations
        permute(permutations, 0, BLOCK);

        //Loop through all possible combinations
        for (z = 0; z < FAC8; z++) {
            strcpy(tmp, str);

            //Decrypt cipher text using transposition with selected combination in this run
            decryptTransp(tmp, permset[z]);

            for (j = 0; j < length-8; j++) { //Find encrypted 'computer' or 'security' in text
                if ((tmp[j]==string_1[0] && tmp[j+1]==string_1[1] && tmp[j+2]==string_1[2] && tmp[j+3]==string_1[3]
                     && tmp[j+4]==string_1[4] && tmp[j+5]==string_1[5] && tmp[j+6]==string_1[6] && tmp[j+7]==string_1[7]) ||
                    (tmp[j]==string_2[0] && tmp[j+1]==string_2[1] && tmp[j+2]==string_2[2] && tmp[j+3]==string_2[3]
                     && tmp[j+4]==string_2[4] && tmp[j+5]==string_2[5] && tmp[j+6]==string_2[6] && tmp[j+7]==string_2[7])){
                        key = key;
                        flag = 1;
                        break;
                    }
            }
            if (flag){
                break;
            }
        }
        row = z;
        run = z * (i + 1);
        int *perm = permset[z];
        printf("Permutations found: ");
        for (j = 0; j < BLOCK; j++){
            printf("%d ", perm[j]);
        }
        printf("\n");
        if (flag){
            break;
        }
    }
    strcpy(string,decryptTransp(string,permset[row]));
    decryptSub(string, key);
    printf("Key found: %d\n", key);
    printf("Number of runs: %d\n", run);
    return string;
}


void permute(int * per, int i, int length){
    int j;
    if (length == i) {
        //Store to permset
        for (i = 0; i < length; i++) {
            if (k < FAC8)
                permset[k][i] = per[i];
        }
        k++;
    } else {
        j = i;
        for (j = i; j < length; j++)
        {
            swap((per+j), (per+i));
            permute(per, i+1, length);
            swap((per+i), (per+j)); //Backtrack
        }
    }
}

void swap(int *x, int *y){
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void allEncrypt(char * str, int permutations[BLOCK]) {
    char str1[strlen(str)];
    char str2[strlen(str)];
    char str3[strlen(str)];

    strcpy(str1, str);
    strcpy(str2, str);
    strcpy(str3, str);

    printf("\n");
    //Substitution cipher
    printf("------------SUBSTITUTION CIPHER-------------\n\nEncrypted text:\n%s\n", encryptSub(str1, SHIFT));
    printf("\nDecrypted text:\n%s\n\n", decryptSub(str1, SHIFT));
    printf("------------TRANSPOSITION CIPHER------------\n");
    //Transposition cipher
    int permutations1[BLOCK];
    printf("\nEncrypted text:\n%s\n", encryptTransp(str2, permutations1));
    printf("\nDecrypted text:\n%s\n\n", decryptTransp(str2, permutations1));
    printf("---------------PRODUCT CIPHER---------------\n");
    //Product cipher
    int permutations2[BLOCK];
    printf("\nEncrypted text:\n%s\n", encryptTransp(encryptSub(str3, SHIFT), permutations2));
    printf("\nDecrypted text:\n%s\n\n", decryptSub(decryptTransp(str3, permutations2), SHIFT));
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

    //Allocate memory
    buffer = calloc( 1, length+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc failed",stderr),exit(1);

    //Copy file into buffer
    if( 1!=fread( buffer , length, 1 , fp) )
        fclose(fp),free(buffer),fputs("read failed",stderr),exit(1);
    fclose(fp);
    strcpy(str, buffer);
    free(buffer);
}

