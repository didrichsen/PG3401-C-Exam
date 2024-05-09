#include <stdio.h>
int main(int argc, char* argv[]){

   int number,biggerNumber = 100, a = 20;
   char *name;
   char *LastName;

   number = 1000;
   while (number>biggerNumber){
   printf("Dette skal ikke fjernes fra body\n");
   printf("%d\n", number--); //Vi kommer forbi number?
   //Greier vi å komme oss forbi denne }
   number--;
}

   a = 10;
   while (a<biggerNumber){
   printf("Dette skal ikke fjernes fra body\n");
   printf("%d\n", a);
   /*
   * Kommer vi oss også forbi denne? }}}}}
   */
   a+=2;
}

return 0;

}