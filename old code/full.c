#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAXSIZE 1024
#define ALPHA 26
#define NUM 10
#define BLOCK 8
#define SHIFT 3

char* encryptSubs(char*, int);
char* decryptSubs(char*, int);
char* encryptTransp(char*, int, int[]);
char* decryptTransp(char*, int, int []);

int main(void){
	char c, choice[2], str[MAXSIZE];

    int i;

	while(1){
        printf("\n");
		puts("To encrypt with substitution cipher, input se \n");
 		puts("To decrypt with substitution cipher, input sd \n");
 		puts("To encrypt with transposition cipher, input te \n");
 		puts("To decrypt with transposition cipher, input td \n");
 		puts("To encrypt with product cipher, input pe \n");
 		puts("To decrypt with product cipher, input pd \n");
 		puts("To exit, input any other letter\n");
 		puts("Your choice ->");

		gets(choice);

		choice[0] = tolower(choice[0]);
		choice[1] = tolower(choice[1]);

		if((choice[0]=='s') && (choice[1]=='e')){
		    printf("\n");
			puts("Please input text for encryption ->");
			gets(str);

			int shift = SHIFT;

			char encryptText[strlen(str)];
			strcpy(encryptText, encryptSubs(str, shift));
			printf("\n");
			printf("Encrypted text: %s", encryptText);
			printf("\n");

			char decryptText[strlen(str)];
			strcpy(decryptText, decryptSubs(encryptText, shift));
			printf("Decrypted text: %s", decryptText);
			printf("\n");

		}else if((choice[0]=='s') && (choice[1]=='d')){
		    printf("\n");
			puts("Please input text for decryption ->");
			gets(str);

			int shift = SHIFT;

			char decryptText[strlen(str)];
			strcpy(decryptText, decryptSubs(str, shift));
			printf("Decrypted text: %s", decryptText);
			printf("\n");

		}else if((choice[0]=='t') && (choice[1]=='e')){
		    int blockSize = BLOCK;

		    printf("\n");
			puts("Please input text for encryption ->");
			gets(str);

			int permutations[blockSize];
            char encryptText[strlen(str)];

			strcpy(encryptText, encryptTransp(str, blockSize, permutations));
			printf("\n");
			printf("Encrypted text: %s", encryptText);
			printf("\n");

			int size = blockSize * (ceil((float)strlen(str)/(float)blockSize));
			char decryptText[size];

			strcpy(decryptText, decryptTransp(encryptText, blockSize, permutations));
			printf("\n");
			printf("Decrypted text: %s", decryptText);
			printf("\n");
		} else if((choice[0]=='t') && (choice[1]=='d')){
		    int blockSize = BLOCK;

		    printf("\n");
			puts("Please input text for decryption ->");
			gets(str);

            int permutations[blockSize];

			puts("Please key in your permutations sequence starting from 0 to 7:");
			for(i = 0;i < 8; i++){
                int temp;
                scanf("%d",&temp);
                permutations[i] = temp;
            }

            int size = blockSize * (ceil((float)strlen(str)/(float)blockSize));
            char decryptText[size];

			strcpy(decryptText, decryptTransp(str, blockSize, permutations));
			printf("Decrypted text: %s", decryptText);
			printf("\n");

		} else if((choice[0]=='p') && (choice[1]=='e')){
		    int shift = SHIFT;
		    int blockSize = BLOCK;
		    int permutations [blockSize];

		    printf("\n");
			puts("Please input text for encryption ->");
			gets(str);

            int size = blockSize * (ceil((float)strlen(str)/(float)blockSize));
            char encryptSubst[strlen(str)];
            char encryptText[size];

            strcpy(encryptSubst, encryptSubs(str, shift));
			strcpy(encryptText, encryptTransp(encryptSubst, blockSize, permutations));
			printf("\n");
			printf("Encrypted text: %s", encryptText);
			printf("\n");

			char decryptTrans[strlen(str)];
            char decryptText[size];

            strcpy(decryptTrans, decryptTransp(encryptText, blockSize, permutations));
			strcpy(decryptText, decryptSubs(decryptTrans, shift));
			printf("\n");
			printf("Decrypted text: %s", decryptText);
			printf("\n");
		} else if((choice[0]=='p') && (choice[1]=='d')){
		    int shift = SHIFT;
		    int blockSize = BLOCK;

		    printf("\n");
			puts("Please input text for decryption ->");
			gets(str);

            int permutations[blockSize];

			puts("Please key in your permutations sequence starting from 0 to 7:");
			for(i = 0;i < 8; i++){
                int temp;
                scanf("%d",&temp);
                permutations[i] = temp;
            }

            int size = blockSize * (ceil((float)strlen(str)/(float)blockSize));
            char decryptTrans[strlen(str)];
            char decryptText[size];

            strcpy(decryptTrans, decryptTransp(str, blockSize, permutations));
			strcpy(decryptText, decryptSubs(decryptTrans, shift));
			printf("\n");
			printf("Decrypted text: %s", decryptText);
			printf("\n");

		}else{
			break;
		}
	}
	return 0;
}

char* encryptSubs(char *str, int shift) {
    size_t length = strlen(str);
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

    return str;
}

char* decryptSubs(char *str, int shift){
    size_t length = strlen(str);
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

    return str;
}

char* encryptTransp(char*str, int blockSize, int permutations [BLOCK]){

	int length = strlen(str);
	int columns = blockSize;
	int rows = (int)(ceil((float)length/(float)columns));

	int permutationSequence[columns];

	char cipherBlock[columns][rows];

	int x,y,i,j;

	printf("\nLength = %d\n", length);
	printf("Columns = %d\n", columns);
	printf("Rows = %d\n", rows);

	i = 0;
	y = 0;

	printf("\nPlain text in matrix:\n");
	while(1){
        if (y == rows){
            break;
        }

        int n = 'a';

        for (x = 0; x < columns; x++){
            if (i < length){
                cipherBlock[x][y] = str[i];
            } else{
                cipherBlock[x][y] = n++;
            }
            i++;
            printf("%c", cipherBlock[x][y]);
        }
        y++;
        printf("\n");
	}

	i = 0;
	j = 0;

	printf("\nKey sequence:\n");
	for(i = 0; i < columns; i++){
		int duplicate;
		int permutation;

		while(1){

			duplicate=0;
			permutation = rand() % columns;

			for(j = 0; j < columns; j++){

                if(permutation == permutations[j]){
                    duplicate = 1;
                    break;
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

	printf("In function encrypt text: %s\n", str);
	return str;
}

char* decryptTransp(char*str, int blockSize, int permutations[BLOCK]){
	int length = strlen(str);
	int columns = blockSize;
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

    printf("In function decrypt text: %s\n", str);
	return str;
}


