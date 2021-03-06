/*
ID:  ysimidjiy1
LANG: C
TASK: checker
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE_TOT 13

int check_cols(int cur_col, int cur_row, int sols[MAXSIZE_TOT]){

  int i;

  for (i = 0; i < cur_col; i++){
    if (cur_row == sols[i]) {
      return 0;
    }
  }

  //printf("col_check passed!\n");
  return 1;

}

int check_diags(int cur_col, int cur_row, int board[MAXSIZE_TOT][MAXSIZE_TOT],
		int max_size){

  int i = cur_col-1, j = cur_row-1;
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

void scan_pos(int cur_col, int board[MAXSIZE_TOT][MAXSIZE_TOT],
	      int sols[MAXSIZE_TOT], int max_size, FILE *fout, int *flag){

  int i;

  if (cur_col == max_size){
    //print sols
    (*flag)++;
    if (*flag <= 3){
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
	scan_pos(cur_col+1, board, sols, max_size, fout, flag);
	board[cur_col][i] = 0;
      }
      
    }
  
  }



}


int main(){

  int board[MAXSIZE_TOT][MAXSIZE_TOT] = {0};
  int sols[MAXSIZE_TOT] = {0};
  
  int max_size;
  int flag = 0;

  FILE *fin = fopen("checker.in", "r");
  FILE *fout = fopen("checker.out", "w");

  fscanf(fin, "%d\n", &max_size);

  scan_pos(0, board, sols, max_size, fout, &flag);

  fprintf(fout, "%d\n", flag);
  exit(0);

}
