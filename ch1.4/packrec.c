/*
ID:  ysimidjiy1
LANG: C
TASK: packrec
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define RECNUM 5
#define DUMMY_RECS 1
#define SOLSIZE 1000

typedef struct pair{
  int x;
  int y;
} pair;

typedef struct rect{

  int placed; //0 means not placed, 1 means placed
  int dimensions[2];
  int orient; //0 means regular, 1 means inverted
  pair pos;
  int order;
} rect;

//Tests if two intervals, [a_1, a_2] and [b_1, b_2], intersect.
int intersect(int a_1, int a_2, int b_1, int b_2){
  //printf("Entered Intersect!\n");
  assert(b_1 <= b_2);
  assert(a_1 <= a_2);

  //printf("[%d %d] to [%d %d]\n", a_1, a_2, b_1, b_2);

  if ( (a_1 <= b_1) && (b_1 < a_2) ) {
    //printf("coll 1\n");
    return 1;
  }
  if ( (a_1 < b_2) && (b_2 <= a_2) ) {
    //printf("coll 2\n");
    return 1;
  }
  if ( (b_1 <= a_1) && (a_1 < b_2) ) {
    //printf("coll 3\n");
    return 1;
  }
  if ( (b_1 < a_2) && (a_2 <= b_2) ) {
    // printf("coll 4\n");
    return 1;
  }

  return 0;

}

//Tests if two rectangles intersect by calling interval intersect
int rects_intersect(rect *a, rect *b){
  //printf("Entered Rects_intersect\n");
  assert(a != b);
  assert(a->placed ==1);
  assert(b->placed ==1);

  if (intersect(a->pos.x, a->pos.x + a->dimensions[a->orient],
		b->pos.x, b->pos.x + b->dimensions[b->orient])) {
    if (intersect(a->pos.y, a->pos.y + a->dimensions[(a->orient + 1) % 2],
		  b->pos.y, b->pos.y + b->dimensions[(b->orient + 1) % 2])) {
      return 1;
    }
  }
  return 0;
}

//Tests if rect_num intersects with any other rectangles.
//Return values:  1 on intersection.  0 otherwise.
int test_all_collision(int rect_num, rect rects[RECNUM]){
  
  int i;
  int temp;
  int plus_x = rects[rect_num].dimensions[0];
  int plus_y = rects[rect_num].dimensions[1];
  int a1 = rects[rect_num].pos.x;
  int b1 = rects[rect_num].pos.y;
  //printf("allright, who wants some?  In test_all_collision\n");
  //flip orientation if we need to.
  if (rects[rect_num].orient) {
    temp = plus_x;
    plus_x = plus_y;
    plus_y = temp;
  }
  //printf("Crai Mohr Nub.  In test_all_collision\n");
  for (i = 0; i<RECNUM-DUMMY_RECS; i++){
    if (i == rect_num) continue;
    if (rects[i].placed == 0) continue;
    if (rects_intersect(&rects[i], &rects[rect_num])) {
      //printf("Collides with %d\n", i);
      return 1;
    }
  }
  //printf("Exiting test_all_collision\n");
  return 0;
  
}

int place_rectangle(int rect_num, rect rects[RECNUM], pair pos, int orient,
		    int motdir, int order){
  
  rects[rect_num].pos = pos;
  rects[rect_num].orient = orient;
  rects[rect_num].placed = 1;
  rects[rect_num].order = order;
  
  //printf("Testing first collision\n");
  //did we get placed in an impossible spot?  If so, placement fails.
  if (test_all_collision(rect_num, rects)) {
    //printf("Placement failed!\n");
    rects[rect_num].placed = 0;
    return 0;
  }

  //printf("Entering While Loop\n");

  //While we are not colliding, keep moving in motdir.
  while(rects[rect_num].pos.x >0 &&
	rects[rect_num].pos.y >0){
    if (!motdir) {
      rects[rect_num].pos.x--;
    }
    else {
      rects[rect_num].pos.y--; 
    }

    if(test_all_collision(rect_num, rects)) {
      printf("Collision detected.  Revert movement!\n");
      if (!motdir) {
	rects[rect_num].pos.x++;
      }
      else {
	rects[rect_num].pos.y++; 
      }
      break;
    }
    printf("This little rectangle moved!\n");
    
  }
  //printf("Placed rectangle with dimensions %d %d at %d %d\n",
  //	 rects[rect_num].dimensions[rects[rect_num].orient],
  //	 rects[rect_num].dimensions[(rects[rect_num].orient + 1) %2],
  //	 rects[rect_num].pos.x,
  //	 rects[rect_num].pos.y);

  return 1;

}

int calculate_bounding_box(rect rects[RECNUM], pair sols[SOLSIZE], int *min,
			    int *next_sol_index){

  int i, j;
  int flag = 0;
  int temp;
  pair new_sol;
  new_sol.x = 0; new_sol.y = 0;

  for (i = 0; i<RECNUM; i++){

    
    printf("Scanned a rectangle with dimensions %d %d at %d %d.  Order: %d, collision: %d\n",
	   rects[i].dimensions[rects[i].orient],
	   rects[i].dimensions[(rects[i].orient + 1) %2],
	   rects[i].pos.x,
	   rects[i].pos.y,
	   rects[i].order,
	   test_all_collision(i, rects));

  //  if (test_all_collision(i, rects)) return 0;

    if(rects[i].pos.x + rects[i].dimensions[rects[i].orient] > new_sol.x)
      new_sol.x = rects[i].pos.x+rects[i].dimensions[rects[i].orient];

    if(rects[i].pos.y + rects[i].dimensions[(rects[i].orient+1)%2] > new_sol.y)
      new_sol.y = rects[i].pos.y+rects[i].dimensions[(rects[i].orient+1)%2];

    //printf("n_s.x: %d, n_s.y: %d\n", new_sol.x, new_sol.y);

  }

  if (new_sol.x > new_sol.y) {
    temp = new_sol.x;
    new_sol.x = new_sol.y;
    new_sol.y = temp;
  }

  printf("Generated bounding box: %d %d\n", new_sol.x, new_sol.y);

  if (new_sol.y * new_sol.x == *min){

    for(j = 0; j<*next_sol_index; j++){
      if (new_sol.x == sols[j].x &&
	  new_sol.y == sols[j].y ){
	flag = 1;
	break;
      }
    }
    //    printf("Found one with size %d\n", *min);

    if (flag == 0){
      sols[*next_sol_index] = new_sol;
      (*next_sol_index)++;
      //printf("Add 'er to the list!\n");
    }
    //    printf("-----------------------------------\n");
    return 1;
  }

  if (new_sol.y * new_sol.x < *min){
    *min = new_sol.y * new_sol.x;
    sols[0] = new_sol;
    *next_sol_index = 1;
    //    printf("A new record!: %d * %d = %d\n", new_sol.y, new_sol.x, *min);
    //    printf("-----------------------------------\n");
    return 1;
  }


  //  printf("-----------------------------------\n");
  //No new solution
  return 0;

}


int test_rects(rect rects[RECNUM], int total_placed, pair sols[SOLSIZE],
	       int *min, int *next_sol_index) {
  //  printf("Entered test_rects!\n");
  //  printf("total_placed = %d\n", total_placed);

  int i, j, k;
  pair target_placement;

  if (total_placed == 4) {
    //    printf("++++++++++All Rects Placed+++++++++++++\n");
    calculate_bounding_box(rects, sols, min, next_sol_index);
    return 0;
  }
  
  for (i = 0; i<RECNUM; i++){
    if (rects[i].placed == 0) continue;

    target_placement.x = rects[i].pos.x + rects[i].dimensions[rects[i].orient];
    target_placement.y = rects[i].pos.y;
    
    //Now place some rectangles moving left
    for (j = 0; j<RECNUM; j++){
      if (rects[j].placed == 1) continue;
      for (k = 0; k<2; k++){
	
	if (place_rectangle(j, rects, target_placement, k, 1, total_placed) ){
	  test_rects(rects, total_placed + 1, sols, min, next_sol_index);
	  rects[j].placed = 0;
	}
      }
    }
    
    target_placement.x = rects[i].pos.x;
    target_placement.y=rects[i].pos.y+rects[i].dimensions[(rects[i].orient+1)%2];
    
    //Now place some rectangles moving down
    for (j = 0; j<RECNUM; j++){
      if (rects[j].placed == 1) continue;
      for (k = 0; k<2; k++){

	if (place_rectangle(j, rects, target_placement, k, 0, total_placed)) {
	  test_rects(rects, total_placed + 1, sols, min, next_sol_index);
	  rects[j].placed = 0;
	}
      }
    }
  }
  //printf("Ok, done with test_rects!\n\n\n\n\n");

}


void print_sols(pair sols[SOLSIZE], int next_sol_index, FILE *fout){

  int i;

  for (i = 0; i<next_sol_index; i++){
    fprintf(fout, "%d %d\n", sols[i].x, sols[i].y);
  }


}

int compare_sols(pair *a, pair *b){

  return (a->x - b->x);

}

int main(){

  //printf("here!\n");
  int i;
  rect rects[RECNUM];
  pair sols[SOLSIZE];
  int min = 10000;
  int next_sol_index = 0;
  
  FILE *fin = fopen("packrec.in", "r");
  FILE *fout = fopen("packrec.out", "w");
  


  //initialize our rectangles
  
  for (i=0; i<4; i++){ 
    fscanf(fin, "%d %d\n", &rects[i].dimensions[0], &rects[i].dimensions[1]);
    rects[i].placed = 0;
    }

  //Rects 4 is a dummy rectangle.  Always has been, always will be.
  rects[4].dimensions[0] = 0;
  rects[4].dimensions[1] = 0;
  rects[4].pos.x = 0; rects[4].pos.y = 0;
  rects[4].placed = 1; rects[4].orient = 0;
  

  //printf("here2!\n");

  test_rects(rects, 0, sols, &min, &next_sol_index);
  //  printf("Done 4 realz\n");
  qsort(sols, next_sol_index, sizeof(pair), compare_sols);
  fprintf(fout, "%d\n", min);
  print_sols(sols, next_sol_index, fout);


  exit(0);

}
