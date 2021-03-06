/*
ID:  ysimidjiy1
LANG: C
TASK: sprime
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXSUPERPRIMES 5000

//a^b
int s_exp(int a, int b){
  
  int i;
  int result = 1;

  for (i=0; i<b; i++){
    result *=a;
  }
  return result;
}

int is_prime(int a){

  int i;

  for (i = 2; i*i <= a; i++){
    if ( !(a%i)) 
      return 0;
  }

  return 1;
  
}

int scan_sprimes(int cur_scan, int cur_len, int max_len, FILE *fout){

  int i;
  int temp;
  //printf("scanning %d, cur_len = %d\n", cur_scan, cur_len);

  if (cur_len == max_len){
    fprintf(fout, "%d\n", cur_scan);
    return;
  }

  for (i = 1; i<10; i++){
    temp = i+cur_scan*10;
    if ( is_prime(temp) ){
      scan_sprimes(temp, cur_len+1, max_len, fout);
    }
  }

  
  
}

int main(){

  FILE *fin = fopen("sprime.in", "r");
  FILE *fout = fopen("sprime.out", "w");

  int *results = calloc(MAXSUPERPRIMES, sizeof(int));
  int *start_results = results;
  int max_len;
  int i;
  
  fscanf(fin, "%d\n", &max_len);

  //  printf("maxlen is: %d\n", max_len);

  scan_sprimes(2, 1, max_len, fout);
  scan_sprimes(3, 1, max_len, fout);
  scan_sprimes(5, 1, max_len, fout);
  scan_sprimes(7, 1, max_len, fout);

  //printf("start_results is %ld,",(long)results);
  //printf(" results is %d\n", (long)results);

  //for (i = 0; start_results[i] != 0; i++){
  //printf("%d\n", start_results[i]);
  //}

  exit(0);

}
