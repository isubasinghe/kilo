#ifndef STATE_H 
#define STATE_H 

enum editor_mode {
  NORMAL, 
  INSERT
};

enum state_actions {
  INSERT_CHAR, 
  NOOP,
  QUIT, 
  ENTER_INSERT, 
  ENTER_NORMAL
};

struct state_t {
  enum editor_mode mode; 
  int row;
  int col;
};

struct state_t* state_create();

struct state_action_t {
  enum state_actions action;
  int chr;
};

int state_handle_input(struct state_t *state, int chr, struct state_action_t *action);


#endif // STATE_H
