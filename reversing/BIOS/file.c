
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* make_flag() {
    char* flag = malloc(20); //memleak but who cares ¯\_(ツ)_/¯
    char flag_c = 'a';
    int i;
    for(i = 0; i<20; i++) {
        flag[i] = flag_c++;
    }
    flag[19] = 0;
    return flag;
}

int main(void) {
    char* filename = "make_me";
    FILE* f = fopen(filename, "r+");
    if(f == 0) {
        abort();
    }
    fprintf(f, "flag: TG17{%s}\n", make_flag());
    fclose(f);
}
