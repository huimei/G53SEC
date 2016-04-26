#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXSIZE 1024

void decryptSubs(char*, int);

int main(){

    char str[MAXSIZE];
    int length = 0;

    printf("Input a string\n");
    gets(str);

    length = strlen(str);

    int l;
    for (l=0; l<length; l++){
        str[l] = tolower(str[l]);
    }

    printf("Length: %d\n", length);
    printf("String: %s\n", str);

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
    int highest, position;
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

    printf("Highest frequency: %d\n\n", highest);
    printf("Highest frequency letter: %c\n\n", highestAlph);

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
    int lengthFive = 0;

    //Get how many words have length of 8
    for (j = 0; j < wordCount; j++){
        if (strlen(array[j])==8){
            lengthFive ++;
        }
    }

    char *stringFive [lengthFive];
    i = 0;

    //Copy word with length of 8 to new array
    for (j = 0; j < wordCount; j++){
        if (strlen(array[j])==8){
            stringFive[i] = array[j];
            i++;
        }
    }

    char alphabet[26] = {'e','t','a','o','i','n','s','h','r','d','l','c','u','m','w','f','g','y','p','b','v','k','j','x','q','z'};
    int key = 0;
    char tmp[8];
    bool flag = false;

    for (i=0; i<26; i++){
        key = (int)highestAlph - alphabet[i];

        if (key < 0){
            key = key + 26;
        }else{
            key = key;
        }

        for (j=0; j<lengthFive; j++){
            strcpy(tmp, stringFive[j]);

            int z;
            for (z=0; z<8; z++){
                tmp[z] = tmp[z] - key;
                if(tmp[z]<97){
                    tmp[z] = 123 - (97 - tmp[z]);
                }else{
                    tmp[z] = tmp[z];
                }
            }

            if ((strcmp("computer",tmp)==0) || (strcmp("security",tmp)==0)){
                key = key;
                flag = true;
                break;
            }
        }
        if (flag){
            break;
        }
    }

    printf("Key found: %d\n", key);

    decryptSubs(str, key);

   return 0;
}

void decryptSubs(char *str, int shift){
    int length = strlen(str);
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

    printf("\nDecrypted String: %s", str);
}
