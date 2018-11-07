#include "stack.h"
#include <stdio.h>

int main ()
{
	stack *stk1 = NULL;
	char *canary = NULL;

	stk1 = CreateStack (1.2);
	stk1 = Push (1.3, stk1);
	stk1 = Push (234.32, stk1);
//	canary = (char *) stk1->data - 1;
//	*canary = 0x67;
	DATA_TYPE x = Pop (&stk1);
	printf ("%lg\n", x);
	DeleteStack (&stk1);
	return 0;
}

