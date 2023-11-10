#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

/*
 * Read file / Open file and read continously
 * */

typedef enum
{
  PLUS, MINUS,
  SLASH, STAR,
  UNDEF
} tokentype_e;

typedef struct
{
  tokentype_e type;
  char *value;
} token_t;

typedef struct 
{
  FILE *fp;
  char *buf; 
  token_t *tokens;
  int tokens_len;
} tokenizer_t;

void print_usage()
{
  printf("Usage:\n");
  printf("\t<c> <path> [<flags>]");
}

int init_tokenizer(char *path, tokenizer_t *tokenizer)
{
  if((tokenizer->fp = fopen(path, "rb")) == NULL)
    return 1;
  
  tokenizer->tokens = calloc(1, sizeof(token_t));
  return 0;
}

void quit(tokenizer_t *tokenizer)
{
  // basic memory cleaning 
  // (Nothing fancy, very prone to leak memory)
  fclose(tokenizer->fp);
  for (int i = 0; i < tokenizer->tokens_len; i++)
  {
    free(tokenizer->tokens[i].value);
  }
  free(tokenizer->tokens);
  free(tokenizer);
}

int main(int argc, char *argv[])
{
  // Checks if the arguments
  // are sufficient
  if (argc < 2)
  {
    print_usage();
    return EXIT_FAILURE;
  }

  // Initialization of tokenizer
  // (throwing assertion if it fails)
  tokenizer_t *tokenizer = malloc(sizeof(tokenizer_t));
  assert(tokenizer != NULL);

  init_tokenizer(argv[1], tokenizer);

  // While loop, incrementing the file pointer
  // and storing the char at every iteration
  char c;
  int len = 0;
  tokentype_e type;
  while((c = fgetc(tokenizer->fp)) != EOF)
  {
    if (isspace(c))
      continue;

    switch(c)
    {
      case '+':
        type = PLUS;
        break;
      case '-':
        type = MINUS;
        break;
      case '/':
        type = SLASH;
        break;
      case '*':
        type = STAR;
        break;
      default:
        type = UNDEF;
        break;
    }

    if (type == UNDEF) 
      continue;

    // Throw assertion if the reallocation fails
    tokenizer->tokens = realloc(
        tokenizer->tokens,
        sizeof(token_t) * (len+1)
      );
    
    tokenizer->tokens[len].type = type;
    tokenizer->tokens[len].value = calloc(1, 1);
    tokenizer->tokens[len].value[0] = c;

    printf("[VALUE: %s, TYPE: %d]\n",
      tokenizer->tokens[len].value,
      tokenizer->tokens[len].type
    );

    len++;
  }
  tokenizer->tokens_len = len;

  quit(tokenizer);

  return 0;
}
