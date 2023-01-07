
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
     if (argc != 2) {
	  printf("Usage: regexp_review [regexp]\n");
	  return 1;
     }

     char *re = argv[1];
     printf("Regexp: %s\n", re);
}
