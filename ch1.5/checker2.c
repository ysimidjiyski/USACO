/*
ID:  ysimidjiy1
LANG: C
TASK: checker
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE_TOT 13
#define MAXSOLS 50000 //50 kb

int check_cols(char cur_col, char cur_row, char sols[MAXSIZE_TOT]){

  char i;

  for (i = 0; i < cur_col; i++){
    if (cur_row == sols[i]) {
      return 0;
    }
  }

  //printf("col_check passed!\n");
  return 1;

}

char check_diags(char cur_col, char cur_row, char board[MAXSIZE_TOT][MAXSIZE_TOT],
		 char max_size){

  char i = cur_col-1, j = cur_row-1;
  while(i >= 0 && j >=0){
    //printf("board[%d][%d] = %d\n", i, j, board[i][j]);
    if (board[i][j]) {
      //printf("f.\n");
      return 0;
    }
    i--; j--;
  } 

  i = cur_col-1; j = cur_row+1;
  while (i >=0 && j < max_size){
    //printf("board[%d][%d] = %d\n", i, j, board[i][j]);
    if (board[i][j]){
      //printf("ailure."); 
      return 0;
    }

    i--; j++;

  }
  //printf("diag_check passed!\n");
  return 1;

}

void scan_pos(char cur_col, char board[MAXSIZE_TOT][MAXSIZE_TOT],
	      char sols[MAXSIZE_TOT], char max_size, FILE *fout, int *flag,
	      char f_sols[3][MAXSIZE_TOT]){

  char i;

  if (cur_col == max_size){
    //print sols
    //printf("Incing flag from %d", *flag);
    (*flag) = (*flag) + 1;
    //printf(" to %d\n", *flag);
    if (*flag <= 3){
      //fprintf(fout, "flagged\n");
      for (i = 0; i < max_size; i++){
	if (i == max_size-1) fprintf(fout, "%d\n", sols[i]+1);
	else fprintf(fout, "%d ", sols[i]+1);
      }  
    }
    return;
  }

  
  for (i = 0; i<max_size; i++){
    //printf("Scanning (%d, %d)\n", cur_col, i);
    if ( check_cols(cur_col, i, sols) ){
      if ( check_diags(cur_col, i, board, max_size) ){
	
	board[cur_col][i] = 1;
	sols[cur_col] = i;
	scan_pos(cur_col+1, board, sols, max_size, fout, flag, f_sols);
	board[cur_col][i] = 0;
      }
      
    }
  
  }



}


int main(){

  char board[MAXSIZE_TOT][MAXSIZE_TOT] = {0};
  char sols[MAXSIZE_TOT] = {0};
  char f_sols[3][MAXSIZE_TOT];
  
  char max_size;
  int flag = 0; int temp = 0;
  char i;

  FILE *fin = fopen("checker.in", "r");
  FILE *fout = fopen("checker.out", "w");

  fscanf(fin, "%hhd\n", &max_size);

  for (i =0; i<(max_size - (max_size %2))/2; i++) {
    
    board[0][i] = 1;
    sols[0] = i;
    scan_pos(1, board, sols, max_size, fout, &flag, f_sols);
    board[0][i] = 0;
  }
  if (flag < 3){
    temp = 3-flag;
    //printf("temp: %d, flag: %d\n", temp, flag);
    while (temp > 0 && (flag -temp>= 0) && (flag-1 >= flag - temp) ) {
      //printf("temp: %d, flag: %d\n", temp, flag);
      for (i = 0; i <max_size; i++){
	if (i == max_size-1) fprintf(fout, "%d\n",
				     max_size - f_sols[flag - temp][i]+1);
	else {
	  fprintf(fout, "%d ", max_size - f_sols[flag - temp][i]+1);
	}
      }
      temp--;
    }
  }

  flag = flag*2;
  //printf("post doubling, flag is: %d\n", flag);
  //printf("i is %d\n", i);
  
  if (max_size %2){
    board[0][i] = 1;
    sols[0] = i;
    scan_pos(1, board, sols, max_size, fout, &flag, f_sols);
  }

  fprintf(fout, "%d\n", flag);
  exit(0);

}
