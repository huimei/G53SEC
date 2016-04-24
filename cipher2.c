#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXSIZE 1024
#define ALPHA 26
#define NUM 10
#define BLOCK 8

void encrypt(char*);
void decrypt(char*, int []);

int main(void){
	char c, choice[1], str[MAXSIZE];

	int columns = BLOCK;
    int permutations[columns];
    int i;

	while(1){
		puts("To encrypt, input e or E\n");
 		puts("To decrypt, input d or D\n");
 		puts("To exit, input any other letter\n");
 		puts("Your choice ->");

		gets(choice);

		if((choice[0]=='e')||(choice[0]=='E')){
			puts("Please input text for encryption ->");
			gets(str);
			encrypt(str);
		}else if((choice[0]=='d')||(choice[0]=='D')){
			puts("Please input encrypted text for decryption ->");
			gets(str);
			puts("Please key in your permutations sequence starting from 0 to 7:");
			for(i = 0;i < 8; i++){
                int temp;
                scanf("%d",&temp);
                printf("Gotten: %d", temp);
                printf("\n");
                permutations[i] = temp;
            }
            for (i = 0; i < 8; i++){
                printf("%d ",permutations[i]);
            }
			//decrypt(str, permutations);
		}else{
			break;
		}
	}
	return 0;
}

void encrypt(char*str){
	int length = strlen(str);
	int columns = BLOCK;
	int rows = (int)(ceil((float)length/(float)columns));

	int permutations[columns];
	int permutationSequence[columns];

	char cipherBlock[columns][rows];

	int x,y,i,j,zero;

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

        for (x = 0; x < columns; x++){
            if (i < length){
                cipherBlock[x][y] = str[i];
            } else{
                cipherBlock[x][y] = '@';
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
	printf("Encrypted cipher text: %s\n", str);
	printf("\n");
	decrypt(str,permutations);
}

void decrypt(char*str, int permutations[BLOCK]){
	int length = strlen(str);
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
	printf("Decrypted plain text: %s\n", str);
	printf("\n");
}
