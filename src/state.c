#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  game_state_t* D= (game_state_t*)malloc(sizeof(game_state_t));
     D->num_rows = 18;
  char** brd = (char**) malloc(sizeof(char*)*18);
    for (int i = 0; i < 18; i++) {
        brd[i] = (char*)malloc(25 * sizeof(char));
    }
    for(int i=0;i<18;i++){
      for(int j=0;j<21;j++){
        if(i==0 || j==0 || i==17 || j==19){
          brd[i][j] ='#';
        }
        else if ( i == 2 && j == 9 ) {
          brd[i][j] ='*';
        }
       else if ( i == 2 && j == 2) {
           brd[i][j] ='d';
      } else if (i == 2 && j == 3) {
           brd[i][j] ='>';
      } else if (i == 2 && j == 4) {
           brd[i][j] ='D';
      } else {
           brd[i][j] =' ';
      }
    }
      brd[i][20]='\n';
      brd[i][21]='\0';
      }
   D->board = brd;
   D-> num_snakes = 1;
     // snake_t snk[(D->num_snakes)];
     // snake_t snk[1];
      snake_t* B = (snake_t*) malloc(sizeof(snake_t));
      B->tail_row = 2;
      B->tail_col = 2;
      B->head_col = 4;
      B->head_row = 2;
      B->live = true;
      //snk[0]= *B;
      // snake_t* snk = D->snakes;
      D->snakes = B ;
      //  D->snakes = NULL;
  return D;
}

/* Task 2 */
void free_state(game_state_t* state) {
  char **arr = state->board;
  for (int i = 0; i < 18; i++) {
    free(arr[i]);
    }
  // for (int i = 0; i < 18; i++) {
  //    for(int j=0;j<25;j++){
  //      free(arr[i][j]);
  //    } 
  
  free(arr);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    char **arr = state->board;
    for (int i = 0; i < state->num_rows; i++) {
    for (int j = 0; j < 20; j++) {
      if(arr[i][j]=='\n'){
        break;
      }
      fputc(arr[i][j], fp);
        }
     fputs("\n", fp);
    }
    return;
    }
    
/*void print_board(game_state_t* state, FILE* fp) {
if (fp == NULL) {
    printf("Error opening file\n");
    exit(1);
}
    int row = state->num_rows;
    char **brd = state->board;
    char ch;
    int i=0;
    int j=0;
    do{
        ch = brd[i][j];
        fputc(ch,fp);
        printf("%c", ch);
         if(ch == '\n' ){
            i++; 
              j=0;
            }
        else {
           j++;
            }
      } while(i<row);
    return;
}*/

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  if(c=='w' || c=='a'|| c=='s' || c=='d'){
    return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
   if(c=='W' || c=='A'|| c=='S' || c=='D' || c=='x'){
    return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
   if(is_head(c) || is_tail(c) || c=='^' || c=='<'|| c=='v' || c=='>'){
    return true;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
 if(c=='^'){
    c = 'w';
    return c;
  }
  else if(c=='<'){
    c = 'a';
    return c;
  }
  else if(c=='v'){
    c = 's';
    return c;
  }
  else if(c=='>'){
    c = 'd';
    return c;
  }
  else return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  if(c=='W'){
    c = '^';
    return c;
  }
  else if(c=='A'){
    c = '<';
    return c;
  }
  else if(c=='S'){
    c = 'v';
    return c;
  }
  else if(c=='D'){
    c = '>';
    return c;
  }
  else return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  if(c=='v' || c=='s'|| c=='S'){
    return cur_row+1;
  }
  else if(c=='^' || c=='w'|| c=='W'){
    return cur_row-1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  if(c=='>' || c=='d'|| c=='D'){
    return cur_col+1;
  }
  else if(c=='<' || c=='a'|| c=='A'){
    return cur_col-1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
 snake_t snk = state->snakes[snum];
 unsigned int x= snk.head_row;
 unsigned int y= snk.head_col;
 char prst = state->board[x][y] ;
 char fut;
  if(prst=='D'){
    fut = state->board[x][y+1];
    return fut;
  }
  if(prst=='W'){
    fut = state->board[x-1][y];
    return fut;
  }
  if(prst=='A'){
    fut = state->board[x][y-1];
    return fut;
  }
  if(prst=='S'){
    fut = state->board[x+1][y];
    return fut;
  }
  return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  snake_t* snk = state->snakes;
  char **arr = state->board;
 unsigned int x= snk->head_row;
 unsigned int y= snk->head_col;
 char prst = state->board[x][y];
 if(prst=='D'){
    arr[x][y] = '>';
    snk->head_row = x;
    snk->head_col = y+1;
    y= y+1;
    arr[x][y] = 'D';
  }
  else if(prst=='W'){
    arr[x][y] = '^';
    snk->head_row = x-1;
    snk->head_col = y;
    x= x-1;
    arr[x][y] = 'W';
  }
  else if(prst=='A'){
    arr[x][y] = '<';
    snk->head_row = x;
    snk->head_col = y-1; 
    y= y-1;
    arr[x][y]='A';
  }
  else if(prst=='S'){
    arr[x][y] = 'v';
    snk->head_row = x+1;
    snk->head_col = y; 
    x= x+1;
    arr[x][y]='S';
  }
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
 snake_t* snk = state->snakes;
  char **arr = state->board;
 unsigned int x= snk->tail_row;
 unsigned int y= snk->tail_col;
 char prst = state->board[x][y];
 if(prst=='d'){
    arr[x][y] = ' ';
    snk->tail_row = x;
    snk->tail_col = y+1;
    y= y+1;
  }
  else if(prst=='w'){
    arr[x][y] = ' ';
    snk->tail_row = x-1;
    snk->tail_col = y;
    x= x-1;
  }
  else if(prst=='a'){
    arr[x][y] = ' ';
    snk->tail_row = x;
    snk->tail_col = y-1; 
    y= y-1;
  }
  else if(prst=='s'){
    arr[x][y] = ' ';
    snk->tail_row = x+1;
    snk->tail_col = y; 
    x= x+1;
  }
  if(arr[x][y]=='<'){
    arr[x][y] = 'a';
  }
  else if(arr[x][y]=='^'){
    arr[x][y] = 'w';
  }
  else if(arr[x][y]=='v'){
    arr[x][y] = 's';
  }
  else if(arr[x][y]=='>'){
    arr[x][y] = 'd';
  }
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
   snake_t* snk = state->snakes;
    unsigned int x= snk->head_row ;
    unsigned int y= snk->head_col ;
  char fut = next_square(state,0);
  if(fut==' '){
    update_head(state,0);
    update_tail(state,0); 
  }
  else if(fut=='*'){
    update_head(state,0);
    add_food(state);
  }
  else if(fut=='#' || is_snake(fut)){
    state->snakes[0].live = false;
    state->board[x][y] = 'x';
  }
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
  // TODO: Implement this function.
  game_state_t* D= (game_state_t*)malloc(sizeof(game_state_t));
  char ch;
  char** brd = (char**) malloc(sizeof(char*)*18);
    for (int i = 0; i < 18; i++) {
        brd[i] = (char*)malloc(25 * sizeof(char));
    }
    unsigned int i=0;
    unsigned int j=0;
    do{
      ch = (char)fgetc(fp);
      if(ch!=EOF){
      brd[i][j]=ch;
     // printf("%c",ch );
      }
      if(ch == '\n' ){
      brd[i] = (char*)realloc( brd[i], (j+1) * sizeof(char));
      i++; 
      j=0;
      }
      else {
        j++;
      }
    } while(ch!=EOF);
   D->board= brd;
   D->num_rows = i;  
   D->num_snakes =0;
   D->snakes = NULL;
  return D;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  char **arr= state->board;
 unsigned int i= state->snakes[snum].tail_row;
 unsigned int j= state->snakes[snum].tail_col;
  char ch= arr[i][j];

  while(!(is_head(ch))){
    i= get_next_row(i,ch);
    j= get_next_col(j,ch);
    ch=arr[i][j];
  }
   state->snakes[snum].head_row = i;
   state->snakes[snum].head_col = j;
   return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
   char **arr= state->board;
   unsigned int ro= state->num_rows;
  unsigned int i =1;
  unsigned int j =1;
  char ch;
  state->num_snakes=0;
 while(i <=(ro-2)){
   ch= arr[i][j];
   if(is_tail(ch)){
      snake_t* B = (snake_t*) malloc(sizeof(snake_t));
      unsigned int n= (state->num_snakes)++;
      B->tail_row=i;
      B->tail_col=j;
      B->live= true;
      state->snakes = B;
      find_head(state, n);
       j++;
    }
    else if(ch=='#'){
      i++;
      j=1;
   }
    else j++;
 }
  return state;
}
