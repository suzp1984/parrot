#ifndef OPRATION_H
#define OPRATION_H

typedef struct _Operation Operation;

typedef int (*OperationAdd)(int a, int b);
typedef void (*OperationHello)(void);

struct _Operation {
	OperationAdd add;
	OperationHello hello;
};

#endif //OPRATION_H
