
#define DEBUG 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum atomType {
     META,
     REG
};

struct atom {
     enum atomType type;
     int start;
     int end;
     struct atom *next;
};

/* Convert a regular expression to an atomic */
/* Where an atomic is a linked list of atoms */
struct atom *reToAtomic(char *re) {
     int start = 0;
     int end = start + 1;

     struct atom *first = NULL;
     struct atom *current = NULL;

     /* Metacharacters recognized outside of a character class */
     char extMeta[] = {'^', '$', '.', '[', '|', '(', ')', '*', '+', '?', '{', '\0'};
     
     /* Metacharacters recognized inside of a character class */
     char intMeta[] = {'^', '-', '[', ']', '\0'};
     
     bool inClass = false;
     for (; *re; re++) {
	  bool isMeta = false;

	  char *meta = (inClass) ? intMeta : extMeta;
	  for (; *meta; meta++) {
	       if (*re == *meta) {
		    isMeta = true;
		    if (*re == '[')
			 inClass = true;
		    else if (*re == ']')
			 inClass = false;
		    break;
	       }
	  }
	  
	  struct atom *temp = malloc(sizeof(struct atom));
	  if (temp == NULL)
	       return NULL;
	  temp->type = (isMeta) ? META : REG;
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
     }
     return first;
}

/* Compare an atomic against expected values */
void atomcmp(enum atomType *types, int *starts, int *ends, struct atom *actual) {
     enum atomType *t = types;
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

/* Test on different regular expressions */
void test() {
     char* re1 = "^foo$";
     enum atomType types1[] = {META, REG, REG, REG, META};
     int starts1[] = {0, 1, 2, 3, 4};
     int ends1[] = {1, 2, 3, 4, 5};
     atomcmp(types1, starts1, ends1, reToAtomic(re1));
     printf("Passed test for \"%s\"\n", re1);

     char* re2 = "-[a|b]c";
     enum atomType types2[] = {REG, META, REG, REG, REG, META, REG};
     int starts2[] = {0, 1, 2, 3, 4, 5, 6};
     int ends2[] = {1, 2, 3, 4, 5, 6, 7};
     atomcmp(types2, starts2, ends2, reToAtomic(re2));
     printf("Passed test for \"%s\"\n", re2);
}

/* Print the contents of an atomic */
void printAtomic(struct atom *atomic) {
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
     struct atom *atomic = reToAtomic(re);
     if (atomic == NULL)
	  return 1;

#ifdef DEBUG
     printAtomic(atomic);
#endif
}
