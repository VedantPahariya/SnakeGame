#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

int main(int argc, char* argv[]) {
  bool io_stdin = false;
  char* in_filename = NULL;
  char* out_filename = NULL;
  game_state_t* state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  
  if (in_filename != NULL) {
    // TODO: Load the board from in_filename
    FILE* fp = fopen(in_filename,"r");
  //  printf("hellolllgdsd\n\n\n");
    state = load_board(fp); 
    // TODO: If the file doesn't exist, return -1
    if(state == NULL){
      return -1;
    
    }
    // TODO: Then call initialize_snakes on the state you made
    initialize_snakes(state);
    // printf("hellolllgdsd\n\n\n");
    //  if(state==NULL){
    //   printf("YES\n");
    // }
    //  if(state->snakes==NULL){
    //   printf(" DEFINETJFjs YES\n");
    // }
    fclose(fp);
  } else if (io_stdin) {
    // TODO: Load the board from stdin
     state = load_board(stdin);
    // TODO: Then call initialize_snakes on the state you made
    state = initialize_snakes(state);
    //  if(state==NULL){
    //   printf("YES\n");
    // }
    
  } else {
    // TODO: Create default state
    state = create_default_state();
  }

  // TODO: Update state. Use the deterministic_food function
  // (already implemented in snake_utils.h) to add food.
  // Write updated board to file or stdout
 // printf("passed1\n\n ");
   update_state(state, (&deterministic_food));
 //printf("passed2\n\n ");
  if (out_filename != NULL) {
    // TODO: Save the board to out_filename
    save_board(state, out_filename );
  } else {
    // TODO: Print the board to stdout
    print_board(state, stdout);
  }

  // TODO: Free the state
  free_state(state);
  return 0;
}
