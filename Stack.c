#ifndef STACK
#define STACK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


HASH_SUM_TYPE HashSum (const DATA_TYPE x);
int CheckData (DATA_TYPE *data);

stack *CreateStack (DATA_TYPE x)
{
	int check_stack = 0;
	char *canary = NULL;
	HASH_SUM_TYPE *hash_sum = NULL;

	stack *new_stk = (stack *) calloc (1, sizeof (*new_stk));
	if (new_stk == NULL)
	{
		fprintf (stderr, "Can't allocate memory\n");
		exit (EXIT_FAILURE);
	}

	canary = (char *) calloc (1, sizeof (x) + sizeof (*hash_sum) + 2 * sizeof (*canary));
	if (canary == NULL)
	{
		fprintf (stderr, "Can't allocate memory\n");
		exit (EXIT_FAILURE);
	}
	
	*canary = CANARY_SYMB;
	new_stk->data = (DATA_TYPE *) (canary + sizeof (*canary));
	memcpy (new_stk->data, &x, sizeof (x));

	hash_sum = (HASH_SUM_TYPE *) (canary + sizeof (x) + sizeof (*canary));
	*hash_sum = HashSum (x);
	
	canary = (char *) hash_sum + sizeof (*hash_sum);
	*canary = CANARY_SYMB;

	new_stk->next = NULL;
	
	check_stack = Verify (new_stk);
	if (check_stack)
	{
		fprintf (stderr, "ERROR: Verification failed, \"CreateStack\" function, code %d\n", check_stack);
		DumpElem (new_stk);
		exit (EXIT_FAILURE);
	}

	return new_stk;
}

stack *Push (DATA_TYPE x, stack *stk)
{
	int check_stack = 0;
	char *canary = NULL;
	HASH_SUM_TYPE *hash_sum = NULL;
	stack *new_elem = NULL;

	check_stack = Verify (stk);
	if (check_stack)
	{
		fprintf (stderr, "ERROR: Verification failed\n\"Push\" function: incorrect stack as argument, code %d\n", check_stack);
		DumpElem (stk);
		exit (EXIT_FAILURE);
	}

    new_elem = (stack *) calloc (1, sizeof (*new_elem));
	if (new_elem == NULL)
	{
		fprintf (stderr, "Can't allocate memory\n");
		exit (EXIT_FAILURE);
	}
	
	canary = (char *) calloc (1, sizeof (x) + sizeof (*hash_sum) + 2 * sizeof (*canary));
	if (canary == NULL)
	{
		fprintf (stderr, "Can't allocate memory\n");
		exit (EXIT_FAILURE);
	}

	*canary = CANARY_SYMB;
	new_elem->data = (DATA_TYPE *) (canary + sizeof (*canary));
	memcpy (new_elem->data, &x, sizeof (x));
	
	hash_sum = (HASH_SUM_TYPE *) (canary + sizeof (x) + sizeof (*canary));
	*hash_sum = HashSum (x);
	
	canary = (char *) hash_sum + sizeof (*hash_sum);
	*canary = CANARY_SYMB;

	new_elem->next = stk;

	check_stack = Verify (new_elem);
	if (check_stack)
	{
		fprintf (stderr, "ERROR: Verification failed\n\"Push\" function: error occured during pushing new element, code %d\n", check_stack);
		DumpElem (new_elem);
		exit (EXIT_FAILURE);
	}

	return new_elem;
}

DATA_TYPE Pop (stack **stk)
{
	ASSERT (stk);

	int check_stack = 0;
	DATA_TYPE x = 0;
	stack *cur_stk = *stk;
	
	check_stack = Verify (cur_stk);
	if (check_stack)
	{
		fprintf (stderr, "ERROR: Verification failed\n\"Pop\" function: incorrect stack as argument, code %d\n", check_stack);
		DumpElem (cur_stk);
		exit (EXIT_FAILURE);
	}
	
	if (cur_stk == NULL)
	{
		fprintf (stderr, "Attempt to read from empty stack\n");
		exit (EXIT_FAILURE);
	}
	
	x = *cur_stk->data;
	*stk = cur_stk->next;

	free ((char *) cur_stk->data - 1);
	free (cur_stk);
	
	if (*stk)
	{
		check_stack = Verify (*stk);
		if (check_stack)
		{
			fprintf (stderr, "ERROR: Verification failed\n\"Pop\" function: incorrect stack as argument, code %d\n", check_stack);
			DumpElem (*stk);
			exit (EXIT_FAILURE);
		}
	}

	return x;
}

void DeleteStack (stack **stk)
{
	ASSERT (stk);
	int check_stack = 0;

	check_stack = Verify (*stk);
	if (check_stack)
	{
		fprintf (stderr, "ERROR: Verification failed\n\"DeleteStack\" function: incorrect stack as argument, code %d\n", check_stack);
		DumpElem (*stk);
		exit (EXIT_FAILURE);
	}

	if (*stk)
	{
		if ((*stk)->next)
			DeleteStack (&((*stk)->next));
		free ((char *) (*stk)->data - 1);
		free (*stk);
	}
}

int Verify (stack *stk)
{
	ASSERT (stk);

	int res = 0;

	if (stk->data)
	{
		res = CheckData (stk->data);
	}
	else
	{
		fprintf (stderr, "ERROR:\nVerify function: NULL ptr instead of data\n");
		res = 1;
	}

	return (stk->next) ? Verify (stk->next) || res : res;
}

HASH_SUM_TYPE HashSum (const DATA_TYPE x)
{
	int i = 0;
	HASH_SUM_TYPE hash = 0;
	char cur_symb = 0;
	int positive = 0;
	
	for (i = 0; i < sizeof (DATA_TYPE); i++)
	{
		cur_symb = *(((char *) &x) + i);
		positive = ((int) cur_symb >= 0) ? (int) cur_symb : 128 - (int) cur_symb ;
		hash += (i + 1) * (HASH_SUM_TYPE) positive;
	}
	
	return hash;
}

int DumpElem (stack *elem)
{
	ASSERT (elem);

	if (elem->data)
	{
		printf ("------------------------\n\
				Stack element dump:\n\
				canary 1: %c\n\
				canary 2: %c\n\
				specified hash sum: %ld\n\
				real hash sum: %ld\n\
				-------------------------\n", *((char *) elem->data - 1), *((char *) elem->data + sizeof (DATA_TYPE) + sizeof (HASH_SUM_TYPE)), * (HASH_SUM_TYPE *) ((char *) elem->data + sizeof (DATA_TYPE)), HashSum (*(elem->data)));

		return 0;
	}

	else
		return 1;
}

int CheckData (DATA_TYPE *data)
{
	int res = 0;
	char *canary = NULL;
	HASH_SUM_TYPE hash_sum = 0;

	canary = (char *) data - 1;
	
	if (*canary != CANARY_SYMB)
	{
		fprintf (stderr, "ERROR:\nCheckData function: wrong value of first canary\n");
		res = 1;
	}

	if (HashSum (*data) != * (HASH_SUM_TYPE *) ((char *) data + sizeof (DATA_TYPE)))
	{
		fprintf (stderr, "ERROR:\nCheckData function: wrong hash sum\n");
		res = 1;
	}

	canary = (char *) data + sizeof (DATA_TYPE) + sizeof (HASH_SUM_TYPE);

	if (*canary != CANARY_SYMB)
	{
		fprintf (stderr, "ERROR:\nCheckData function: wrong value of second canary\n");
		res = 1;
	}

	return res;
}

#endif

