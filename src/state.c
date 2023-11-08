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
        brd[i] = (char*)malloc(21 * sizeof(char));
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
  free(arr);
  state->snakes = NULL;
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
    for (int i = 0; i < 17; i++) {
  //for (int j = 0; j < 20; j++) {
            fputs(arr[i], fp);
      //  }
    //  printf("%s",arr[i]);
    }
        //fputs("\n", fp);
       fputs( arr[17],fp);
      // printf("%s",arr[17]);
   //    fprintf( fp, "\n");
// fprintf( fp, "%s",arr[i] );
     fclose(fp);
 return;
    }

// void print_board(game_state_t* state, FILE* fp) {
//     if (fp == NULL) {
//         printf("Error opening file\n");
//         return;
//     }

//     char **arr = state->board;
//     for (int i = 0; i < 18; i++) {
//         fputs(arr[i], fp);
//         if (i < 17) { // Don't add a newline character after the last row
//             fputc('\n', fp);
//         }
//         // fputc('\n', fp);
//     }
//      int lastRowLength = strlen(arr[17]);
//     if (arr[17][lastRowLength - 1] != '\n') {
//         fputc('\n', fp);
//     }
//     //fputc('\n', fp);
// }
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
  // TODO: Implement this function.
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
  // TODO: Implement this function.
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  return;
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
  // TODO: Implement this function.
  return NULL;
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
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  return NULL;
}
