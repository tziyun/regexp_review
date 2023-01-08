
/* #define DEBUG 1 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum atomtype {
     META,
     REG
};

struct atom {
     enum atomtype type;
     int start;
     int end;
     struct atom *next;
};

struct atom *re2atomic(char *re) {
     int start = 0;
     int end = start + 1;

     struct atom *first = NULL;
     struct atom *current = NULL;

     char extchars[] = {'^', '$', '.', '[', '|', '(', ')', '*', '+', '?', '{'};
     char *extchar = extchars;

     for (; *re; re++) {
	  int ismeta = 0;
	  
	  for (; *extchar; extchar++) {
	       if (*re == *extchar) {
		    ismeta = 1;
		    break;
	       }
	  }
	  
	  struct atom *temp = malloc(sizeof(struct atom));
	  if (temp == NULL)
	       return NULL;
	  temp->type = (ismeta) ? META : REG;
	  temp->start = start;
	  temp->end = end;
	  temp->next = NULL;
	  if (first == NULL)
	       first = temp;
	  else
	       current->next = temp;
	  current = temp;

	  start++;
	  end = start + 1;
	  extchar = extchars;
     }
     return first;
}

void atomcmp(enum atomtype *types, int *starts, int *ends, struct atom *actual) {
     enum atomtype *t = types;
     int *s = starts;
     int *e = ends;
     struct atom *act = actual;
     while (act) {
	  assert(act->type == *t);
	  t++;
	  assert(act->start == *s);
	  s++;
	  assert(act->end == *e);
	  e++;
	  act = act->next;
     }
}

void test() {
     enum atomtype types1[] = {META, REG, REG, REG, META};
     int starts1[] = {0, 1, 2, 3, 4};
     int ends1[] = {1, 2, 3, 4, 5};

     atomcmp(types1, starts1, ends1, re2atomic("^foo$"));

     printf("All tests passed\n");
}

void printatomic(struct atom *atomic) {
     printf("Atomic of command-line input:\n");
     struct atom *current = atomic;
     while (current) {
	  printf("\ttype: %s, start %d, end %d\n",
		 (current->type == META) ? "meta" : "reg",
		 current->start,
		 current->end);
	  current = current->next;
     }
}

int main(int argc, char **argv) {
#ifdef DEBUG
     test();
#endif
     
     if (argc != 2) {
	  printf("Usage: regexp_review [regexp]\n");
	  return 1;
     }

     char *re = argv[1];
     struct atom *atomic = re2atomic(re);
     if (atomic == NULL)
	  return 1;

#ifdef DEBUG
     printatomic(atomic);
#endif
}
