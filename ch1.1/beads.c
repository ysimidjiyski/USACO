/*
ID:  ysimidjiy1
LANG: C
TASK: beads
*/

#include <stdio.h>
#include <string.h>

typedef enum state {
  RED,
  BLUE
} state;

//returns 1 on string continue, 0 on string break.
int processbead(char *pos, state the_state, int *whites, int *count){

  //  printf("in procbead, pos is %c\n", *pos);
  //  printf("whitecount is %d\n", *whites);

  switch (*pos) {
  case 'w':
    //printf("proc white\n");
    (*count)++;
    (*whites)++;
    return 1;
    break;
  case 'b':
    //printf("proc blue\n");
    if (the_state == BLUE) {
      (*count)++;
      (*whites)=0;
      return 1;
    }
    else return 0;
    break;
  case 'r':
    //printf("proc red\n");
    if (the_state == RED) {
      (*count)++;
      (*whites)=0;
      return 1;
    }
    else return 0;
    break;
  }
}

int main(){

  int offset=0; //offset from the beginning of our buffer
  int start=0; //offset from the beginning of our buffer designted "start"
  int a=0, b=0; //lengths of the last two bead chains
  int count; //length of the bead chain
  int whitecount=0; //current number of whites at the end of the bead chain
  int max=0; //longest pair of chains yet encountered
  state the_state; //current state
  int wasteful = 0; //this will make sure we do an accurate count.
  int whiteflag = 0;  //Did we use whites at the tail end of our count?
  int lastswitchpos = 0; int lastswitchpos2;

  char buffer[352];

  FILE *input = fopen("beads.in", "r");
  FILE *output = fopen("beads.out", "w");

  fscanf(input, "%d\n", &count);
  fgets(buffer, 352, input);

  if(*buffer == 'b') {
    the_state=BLUE;
  }
  else the_state=RED;

  //align our beginning to a boundary
  while(processbead(buffer+start, the_state, &whitecount, &b)) {
    if (offset < count) {
      // printf("offset reached %d\n", offset);
      offset++;
      start++;
      whitecount = 0;
    }
    else {
      fprintf(output, "%d\n", count);
      exit(0);
    }
  }

  //  printf("-------------------------------\n");
  a = 0; b=0; whitecount=0; whiteflag=0; lastswitchpos = start;

  while(wasteful < 3 || offset != start) {


    //If we can continue the string
    if(processbead(buffer+offset, the_state, &whitecount, &b)) {
      offset++;
      //printf("offset increment!\n");
    }
    //otherwise...
    else {
      if (a+b > max) {
	printf("offset: %d, lastswitchpos: %d\n", offset, lastswitchpos);
	if (offset == lastswitchpos2) {
	  max = a+b-whiteflag-whitecount;
	}
	else max = a+b-whiteflag;   
      }
      //printf("on switch:  a = %d, b=%d, wflag=%d, wcount = %d\n",
      //	     a,b,whiteflag, whitecount);
      a = b;
      b = whitecount;
      whiteflag = whitecount;
      whitecount = 0;
      the_state = !the_state;
      lastswitchpos2 = lastswitchpos;
      lastswitchpos=offset;
      //offset++;  //We don't want this... Ah, because count starts at 0.
    }
    if (offset == count) {
      offset = 0;
      //printf("wasteful inc\n");
      wasteful++;
    }
  }

  fprintf(output, "%d\n", max);

  exit(0);

}
