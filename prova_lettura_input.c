#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(){
    char str[50];
    const char s[2] = ",";
    char* token;
    int num;
    int n, k;
    char key_word[50];

    if(scanf("%d %d\n", &n, &k)){}
    printf("n e k: %d, %d\n", n, k);

    if(fgets(key_word, 50, stdin)){}
    printf("%s\n", key_word);
    if (key_word[0] == 'A'){
        if(fgets(str, 50, stdin)){}
        printf("riga matrice: %s\n", str);
        token = strtok(str, s);

        while(token != NULL){
            num = atoi(token);
            printf("%d\n", num);

            token = strtok(NULL, s);

        }
    }

    return 0;
}
