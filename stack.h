#define DATA_TYPE double
#define HASH_SUM_TYPE long int
#define CANARY_SYMB 0x69

#define ASSERT(CONDITION) 																	\
	if(!(CONDITION))																		\
	{																						\
		fprintf (stderr, "Assertation failed, file %s, function %s, line %d\n",				\
				__FILE__, __PRETTY_FUNCTION__, __LINE__);									\
		exit (EXIT_SUCCESS);																\
	}


typedef struct Stack
{
	DATA_TYPE *data;
	struct Stack *next;
} stack;

stack *CreateStack (DATA_TYPE x);
stack *Push (DATA_TYPE x, stack *stk);
DATA_TYPE Pop (stack **stk);
void DeleteStack (stack **stk);
int Verify (stack *stk);
int DumpElem (stack *elem);

#include "Stack.c"

