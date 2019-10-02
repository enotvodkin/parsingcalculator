#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#define OP_ADD '+'		//43		
#define OP_SUB '-'		//45
#define OP_DIV '/'		//47
#define OP_MULT '*'		//42
#define OP_OBKT '('		//40
#define OP_CBKT ')'		//41

struct stack
{
	int e[64];
	int top;
};

void inline init(struct stack *st)
{	st->top = 0;	}

void inline push(struct stack *st, int f)
{
	st->e[st->top] = f;
	st->top++;
}

int pop(struct stack *st)
{
	int e;
	if ((st->top) > 0)
	{
		st->e[st->top] = -842150451;
		st->top--;
		e = st->e[st->top];
		return e;
	}
	else
	{
		Beep(750, 300);
		return 0;
	}
}

int inline gettop(struct stack *st)		
{	return st->e[st->top - 1];	}

int inline getcount(struct stack *st)
{	return st->top;		}

int inline isempty(struct stack *st)
{	return !(st->top) ? 1 : 0;		}

void inline append(char *s, char c)
{
	int l = strlen(s);
	s[l] = c;
	s[l + 1] = '\0';
}

void compExpression(struct stack *st_num, struct stack *st_op)
{
	int y = pop(st_num),
		x = pop(st_num);
	switch (pop(st_op))
	{
	case OP_MULT:
		push(st_num, x * y);
		break;
	case OP_DIV:
		if (y) push(st_num, x / y);
		else Beep(350, 300);
		break;
	case OP_ADD:
		push(st_num, x + y);
		break;
	case OP_SUB:
		push(st_num, x - y);
		break;
	default:
		Beep(500, 300);
		break;
	}
}

void findExpression(struct stack *st_num, struct stack *st_op, const char *s)
{
	char t[10] = { 0 };
	for (;; *s++)
	{
		if (!(*s))
		{
			if (*(s - 1) == OP_CBKT) break;
			push(st_num, atoi(t));
			break;
		}
		if (*s == OP_MULT || *s == OP_DIV)
		{
			if (*(s - 1) != OP_CBKT) push(st_num, atoi(t));
			memset(&t, 0, 10);
			if (gettop(st_op) == OP_MULT || gettop(st_op) == OP_DIV || 
				(gettop(st_op) == OP_SUB && getcount(st_num) == 2))	
				compExpression(st_num, st_op);		
			push(st_op, *s);
		}
		else if (*s == OP_ADD || *s == OP_SUB)
		{
			if (*(s - 1) != OP_CBKT) push(st_num, atoi(t));
			memset(&t, 0, 10);
			if (gettop(st_op) == OP_MULT || gettop(st_op) == OP_DIV ||
				gettop(st_op) == OP_ADD  || gettop(st_op) == OP_SUB) 
				compExpression(st_num, st_op);
			push(st_op, *s);
		}
		else if (*s == OP_OBKT) push(st_op, *s);
		else if (*s == OP_CBKT)
		{
			if (*t) push(st_num, atoi(t));
			memset(&t, 0, 10);
			while (gettop(st_op) != OP_OBKT) compExpression(st_num, st_op);
			st_op->top--;
		}
		else append(t, *s);
	}
	while (!isempty(st_op))
		compExpression(st_num, st_op);
}

int main()
{
	const char *exp = "-5*(((10/2)))/(2+4-1)+3";	//"-((51-40)-(-1+3))*((7-5)+2)/2";
	struct stack *st_num = (struct stack *) malloc(sizeof(struct stack)),
		*st_op = (struct stack *) malloc(sizeof(struct stack));
	init(st_num); init(st_op);

	findExpression(st_num, st_op, exp);
	printf("%s = %d\n", exp, gettop(st_num));

	free(st_num);
	return 0;
}
