#include <stdio.h>
#include <stdlib.h>

int row, col; // declare the size of the whole map

// create the function to count the alive cell
int counter(int cell[row][col], int r, int c){
  int i, j; // declare the variable for current row:i and column:j
  int count = 0; // the number of alive cells
  for (i=r-1; i<=r+1; i++){
    for (j=c-1; j<=c+1; j++){
      // if the cell is cell[r][c], or i or j less than 0, or i >= row or j>= col
      // then continue
      if ((i==r && j==c) || (i<0 || j<0) || (i>=row || j>=col)){
        continue;
      }
      if (cell[i][j]==1){
        count++; // the counter +1 when the cell value is 1
      }
    }
  }
  return count;
}

// define the function to decide the nextgeneration
int next_gen(int current_generation[row][col]){
  int i,j;
  int next_generation[row][col];
  int live_cell_num;

  for (i=0; i<row; i++){
    for (j=0; j<col; j++){
      live_cell_num = counter(current_generation[row][col], i, j);
      // any live cell with two or three live neighbours lives on to the next
      // generation.
      if(current_generation[i][j]==1 && (live_cell_num==2 || live_cell_num==3)){
        next_generation[i][j] = 1;
      }
      // any dead cell with exactly three live neighbours becomes a live cell
      else if(current_generation[i][j]==0 && live_cell_num==3){
        next_generation[i][j] = 1;
      }
      // any live cell with live neighbours fewer than 2 or more than 3
      // will be dead in the next generation.
      else{
        next_generation[i][j] = 0;
      }
    }
  }
  return next_generation[row][col];
}


int main(){
  int iteration = 10; // declare the variable for times of iteration
  int i;
  int current_generation[row][col];
  int next_generation[row][col];

  /*
  Here to generate the initial state of the map
  */

  for (i=0; i<=iteration; i++){
    next_generation[row][col] = next_gen(current_generation[row][col]);
    
    /*
    Here to visualize the game
    */
  }

}
