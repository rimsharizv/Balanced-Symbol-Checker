// 
// CS 211 - Project 2: Writing a C program that will determine whether input is given with properly balanced symbols
// Rimsha Rizvi (659249234)
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE  1
#define FALSE 0

int DebugMode;

typedef struct Stack
{
  char *darr;  // the pointer to the dynamic array that actually holds the stack
  int size;  // the integer variable specifying the current size of the dynamic array
  int topStack;  // the integer variable specifying the top of the stack
} Stack;

void init(struct Stack *s);
int is_empty(struct Stack *s);
void push(struct Stack *s, char val);
void pop(struct Stack *s);
char top(struct Stack *s);
void clear(struct Stack *s);
int IsItBalanced(Stack *s, char *input, int *index);
char error1(Stack *s);
char error2(Stack *s, char *input, int index);

// init
// initializing the stack
void init(Stack *s)
{
  s->size = 2;
  s->darr = (char*) malloc(sizeof(char) * s->size);
  s->topStack = 0;
}

// is_empty
// checking if the stack is empty
int is_empty(Stack *s)
{
  if (s->topStack == 0){
    return TRUE;
  }
  else {
    return FALSE;
  }
}

// push
// pushing/adding an element onto the stack
void push(Stack *s, char val)
{
  // grow stack
  if (s->topStack >= s->size) {
    char *temp;
    int old_val, new_val, elements, i; // variables for debugging (except i)
    temp = s->darr;
    s->darr = (char *)malloc(sizeof(char)*2*s->size);

    for (i = 0; i < s->topStack; i++) {
        s->darr[i] = temp[i];
    }
    free(temp);

    old_val = s->size;
    elements = s->topStack;

    s->size = s->size * 2;
    new_val = s->size;

    if (DebugMode == TRUE) { // debug for grow
        printf("The stack was full. It has doubled now, from %d to %d, where a total of %d values were copied into the larger array.\n", old_val, new_val, elements);
    }
  }

  if (DebugMode==TRUE){ // debug for push
    printf("\nPushing %c to stack ", val);
  }
  s->darr[s->topStack] = val;
  s->topStack = s->topStack + 1;
}

// pop
// popping/removing an element off of the stack
void pop(Stack *s)
{
  //print debugging info
  if (DebugMode==TRUE){
    printf("\nPopping %c from stack ", top(s));
  }
  s->topStack = s->topStack - 1;
}

// top
// accessing/returning the top element on the stack
char top(Stack *s)
{
  return (s->darr[s->topStack - 1]);
}

// clear
// clear the stack so that it is empty and ready to be used again
void clear(Stack *s)
{
  free(s->darr);
  init(s);
}


// IsItBalanced
//    It checks if the expression is balanced. 
//    If not, it finds the errors involved in the expression
int IsItBalanced(Stack *s, char *input, int *index) {
  char current_val = '\0';
  char top_val;
  int error = 0;
  int i = 0;

  while(input[i] != '\0') {
    current_val = input[i];

    // When an opening symbol is encountered, this symbol is pushed onto the stack
    if (current_val=='(' || current_val=='{' || current_val=='[' || current_val=='<')
      push(s, current_val);

    // When a closing symbol is encountered, check the symbol at the top of the stack
    else if (current_val==')' || current_val=='}' || current_val==']' || current_val=='>') {
      // Error #2
      //  is unbalanced because it is expecting a different closing symbol (the wrong closing symbol is on the top of the stack)
      if(is_empty(s)) {
          error = 2;
          break;
      }
      top_val = top(s);
      
      // if closing symbol matches opening symbol at top, pop the opening symbol
      if((top_val=='(' && current_val==')') || (top_val=='{' && current_val=='}') ||
        (top_val=='[' && current_val==']') || (top_val=='<' && current_val=='>')) {
          pop(s);
      }

      // Error #1
      // is unbalanced because it is missing an opening symbol (stack is empty when a closing symbol is encountered)
      if(!((top_val=='(' && current_val==')') || (top_val=='{' && current_val=='}') || (top_val=='[' && current_val==']') || (top_val=='<' && current_val=='>'))) {
        error = 1;
        break;
      }
    }
    i++;
  }

  // Error #3
  // is unbalanced because it is missing a closing symbol (line ends while the stack is not empty)
  if(error == 0 && is_empty(s) == FALSE) {
    error = 3;
  }
  if(error != 0 && index != NULL) {
    *index = i + 1;
  }
  return error;
}


// error1
// This function is called when expression is unbalanced because:
//    Error #1: it is missing an opening symbol
//    Error #3: it is missing a closing symbol
char error1(Stack *s){
    char check_error = ' ';
    char top_val = top(s);
    if(top_val == '(')
        check_error = ')';
    else if(top_val == '{')
        check_error = '}';
    else if(top_val == '[')
        check_error = ']';
    else if(top_val == '<')
        check_error = '>';

    return check_error;
}


// error2
// This function is called when expression is unbalanced because:
//    Error #2: it is expecting a different closing symbol
char error2(Stack *s, char *input, int index){
    char check_error = ' ';
    char prev_val = input[index-1];
    if(prev_val == ')')
        check_error = '(';
    else if(prev_val == '}')
        check_error = '{';
    else if(prev_val == ']')
        check_error = '[';
    else if(prev_val == '>')
        check_error = '<';

    return check_error;
}

int main (int argc, char** argv)
{
 char input[301];
 
 Stack my_stack;
 init(&my_stack);

 /* check for command line argument of -d */
 DebugMode = FALSE;
 int i;
 for ( i = 0; i < argc; i++)
    if ( strcmp (argv[i], "-d") == 0)
       DebugMode = TRUE;

 /* set up an infinite loop */
 while (1)
 {
   /* get line of input from standard input */
   printf ("\nEnter input to check or q to quit\n");
   fgets(input, 300, stdin);

   /* remove the newline character from the input */
   int i = 0;
   while (input[i] != '\n' && input[i] != '\0')
   {
      i++;
   }
   input[i] = '\0';

   /* check if user enter q or Q to quit program */
   if ( (strcmp (input, "q") == 0) || (strcmp (input, "Q") == 0) ) {
     break;
   }
   printf("\n");
   printf ("%s\n", input);

   /* run the algorithm to determine is input is balanced */
   int ind;
   int error = IsItBalanced(&my_stack, input, &ind);
   char check_error;

   if(DebugMode==TRUE) {
      printf("\n\n%s\n", input);
   }

   if(error == 0){
    printf("Expression is balanced\n");
   } else {
    int j;
    for(j=1; j<ind; j++)
        printf(" ");

    if(error == 1) {
        check_error = error1(&my_stack);
        printf("^ expecting %c\n", check_error);
    }
    else if(error == 2) {
        check_error = error2(&my_stack, input, ind);
        printf("^ missing %c\n", check_error);
    }
    else {
        check_error = error1(&my_stack);
        printf("^ missing %c\n", check_error);
    }
   }
   clear(&my_stack); // for new line
  }
 printf ("\nGoodbye\n");
 return 0;
}