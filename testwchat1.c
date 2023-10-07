#include <stdio.h>
#include <string.h>

int main () {
   const char str[] = "https://www.tutorialspoint.com";
   const char *ch = "tps:";
   char *ret;

   ret = strchr(str, ch);

   printf("String after |%c| is - |%s|\n", ch, ret);
   
   return(0);
}