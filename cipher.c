
#include <stdio.h>
#include <ctype.h>

#define MAXSIZE 1024
#define SHIFT 3

void encrypt(char*);
void decrypt(char*);

int main(void){
	char c, choice[2], str[MAXSIZE];

	while(1){
		puts("To encrypt, input e or E\n");
 		puts("To decrypt, input d or D\n");
 		puts("To exit, input any other letter\n");
 		puts("Your choice ->\n");

		gets(choice);

		if((choice[0]=='e')||(choice[0]=='E')){
			puts("Please input text for encryption ->");
			gets(str);
			encrypt(str);
		}else if((choice[0]=='d')||(choice[0]=='D')){
			puts("Please input encrypted text for decryption ->");
			gets(str);
			decrypt(str);
		}else{
			break;
		}
	}
	return 0;
}

void encrypt(char*str){
	int length = strlen(str);
	int shift = SHIFT;
	int i;

	for(i = 0; i < length; i++){
		if (str[i] == " "){
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
	printf("Encrypted: %s\n\n", str);
	decrypt(str);
}

void decrypt(char*str){
	int length = strlen(str);
	int shift = SHIFT;
	int i;

	for(i = 0; i < length; i++){
		if (str[i] == " "){
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







