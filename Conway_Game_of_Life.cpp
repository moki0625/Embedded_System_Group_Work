#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <math.h>



/*team work strategy
Our project is mainly divided into 2 parts: the core of game (programming the cell 
will be live or dead each state) and the UI & memory allocation.

Chengli Liu did the first part, using  the function 'counter' to count the number 
of live neighbors and the function 'LoD' to define the chosen cell will be live or 
dead in the next state

Yiyi Wang did the second part, using the ncurses library to create a UI which enables
users to move the cursor and insert or eliminate the cell. Once the program is started,
the cells will change the state automatically. For the menmory allocation, we used 'malloc' 
to creat 2 pointers for current state and the next state. And the next state will be freed 
after it is copied by the current one.

*/


/* system inroduction
Once the code is run, a UI is opened. The cursor will be shown in the center of the screen.
Since the pause is flagged at the beginning, the system will repeat reading the keyboard input.
At that time, user can move the cursor with arrows and change the state of current cell with space.
Users can press 'p' to start or pause the game, and 'q' to quit the game. 

Once the game is started, the function 'game' keeps judging the cells will be live or dead in the next generation.
The function 'visualization' keeps printing the current state of the cells stored in the memory.

The game can be paused or quit at any time.

*/


/* Programming state */
bool running = 1;
bool pause_ = 1;

/*pointer*/
bool** state; // pointer of the current state

/*current cursor position */
int y, x; 






//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/
//Coder1: Chengli Liu





/*create the function to count the surrounding alive cell*/
int counter(int y_, int x_, bool **state) {
	int i, j; // declare the variable for current row:i and column:j
	int count = 0; // the number of alive cells
	for (i = y_ - 1; i <= y_ + 1; i++) {
		for (j = x_ - 1; j <= x_ + 1; j++) {
			// if the cell is cell[r][c], or i or j less than 0, or i >= row or j>= col
			// then continue
			if ((i == y_ && j == x_) || (i < 0 || j < 0) || (i >= LINES-1) || (j >= COLS-1)) {
				continue;
			}
			else if (state[i][j] == 1) {
				count++; // the counter +1 when the cell value is 1
			}
		}
	}
	return count;
}

/*check the cell will be live or dead in the next state*/
bool LoD(int y_, int x_, bool **state) {
	int live_cell_num = counter(y_, x_, state);
	if (state[y_][x_]) {
		//any live cell with live neighbours fewer than 2 or more than 3 will be dead in the next generation.
		// any live cell with two or three live neighbours lives on to the next generation.
		if (live_cell_num < 2 || live_cell_num > 3)
			return false;
		return true;
	}
	//any dead cell with exactly three live neighbours becomes a live cell
	else if (live_cell_num == 3) {
		return true;
	}
	else {
		return false;
	};
	return true;
};
	



//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/









//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/
//Coder2: Yiyi Wang

/* the game */
void game(bool **state)
{
	bool **next_state = NULL; //pointer of the next state
	if (pause_)
		return;

	/*create the menmory space for the next state*/
	next_state = (bool**)malloc(LINES * sizeof(bool *));  //bool* -> array of pointers
	for (int i = 0; i < LINES; ++i)
		next_state[i] = (bool*)malloc(COLS * sizeof(bool));

	/*create the menmory space for the next state*/
	for (int i = 0; i < LINES; ++i)
		for (int j = 0; j < COLS; ++j)
			if (LoD(i, j, state))
				next_state[i][j] = true;
			else
				next_state[i][j] = false;	

	/* copy next state to the current*/
	for (int i = 0; i < LINES; ++i)
		for (int j = 0; j < COLS; ++j)
			state[i][j] = next_state[i][j];

	//free the menmory of the next state
	for (int i = 0; i < LINES; ++i)
		free(next_state[i]);
	free(next_state);
	next_state = NULL;
	
	return;
}

	
/*initialize the screen and allocate the memory*/
void initialization(void)
{
	/*Initialization of the screen*/
	initscr(); // start curses mode 
	cbreak(); // no buff 
	noecho();  //the input of the keyboard will not be shown on the screen
	nonl();  //carriage return is translated to newline, but won't be the input.
	intrflush(stdscr, FALSE);  // no flushing of the buffer will occur when an interrupt key is pressed 
	keypad(stdscr, TRUE); //enables the reading of function keys like arrow keys 
	nodelay(stdscr, TRUE); // getch to be a non-blocking call
	clear();

	/* initialize the use of color */
	if (has_colors()) {
		start_color();
		init_pair(0, COLOR_BLACK, COLOR_WHITE); 
		init_pair(1, COLOR_WHITE, COLOR_WHITE);

	}

	/*initial cursor position--> center of the screen*/
	y = LINES / 2; // LINES and COLS are the number of rows and columns of stdscr
	x = COLS / 2;
	move(y, x);
	refresh(); // refresh the scrceen

	/* memory allocaction */
	state = (bool**)malloc(LINES * sizeof(bool *));  //bool* -> array of pointers
	for (int i = 0; i < LINES; ++i)
		state[i] = (bool*)malloc(COLS * sizeof(bool));

	// state[10][10] = true; state[11][11] = true; state[12][10] = true; state[11][9] = true;
	// state[15][15+10] = true; state[16][16+10] = true;state[17][16+10] = true;state[17][15+10] = true;
	// state[17][14+10] = true;
	return;
}


void visualization(bool **state)
{
	/* Clean the screen */
	refresh();
	erase(); // erase the screen so that we just need to flag the live cell
	getsyx( y, x ); // get the current cursor position
	
	// set * where the cell is live
	for (int i = 0; i < LINES; ++i) {
		for (int j = 0; j < COLS; ++j) {
			if (state[i][j]){
				 mvaddch(i,j,'*');
			}
		}
	}
	move(y,x); // move the cursor to the previous position
	refresh();
}




/* read the input of keyboard*/
void keyboard(void) {
	int c; //input of the keyboard
	getsyx( y, x);// get the current cursor position
	c = getch();  //get the user input
	switch (c) {
		//cursor movement
	case KEY_UP:
		move(--y, x);
		break;
	case KEY_DOWN:
		move(++y, x);
		break;
	case KEY_LEFT:
		move(y, --x);
		break;
	case KEY_RIGHT:
		move(y, ++x);
		break;
		
		//reverse the state of the chosen cell  
	case ' ':
		state[y][x] = !state[y][x];
		break;
		//pause or start the game
	case 'p':
		pause_ = !pause_;
		break;
		// quit the game
	case 'q':
		running = 0;
		break;
	}
	refresh();
	return;
}



int main(){
  initialization();
  visualization(state);

  while (running) {  
	  game(state);
	  keyboard();  
	  while (pause_) // when the game is paused, keep reading the keyboard input and refresh the screen
	  {
		keyboard();
	  	visualization(state);
		usleep(100000);
		if( not running) break;   // if the user presses 'q'--> quit the game, stop reading
	  }
	  visualization(state);
	  usleep(1000000); 

  }

  /* free the menmory*/
  	for (int i = 0; i < LINES; ++i)
  		free(state[i]);
  	free(state);

  	endwin();// End curses mode 
  	return 0;


}


//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/
//----------------------------------------------------------------------------/



