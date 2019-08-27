#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define STACK_SIZE 10000

int stack[STACK_SIZE];
int pos = -1;

void push(int value);
int pop();
int size();
int empty();
int top();

int main() {
	int command, i;
	char input[20];
	int* result;

	scanf("%d\n", &command);
	result = (int*) malloc(sizeof(int) * command);

	for (i = 0; i < command; ++i) {
		scanf("%[^\n]s", input);
		
		char* token = strtok(input, " \n");
		char input_command[20];
		int input_number;
		strcpy(input_command, token); // command
		
		while (1) {
			token = strtok(NULL, " \n");
			if (token == NULL)
				break;
			input_number = atoi(token);
		}
		
		if (!strcmp(input_command, "push")) {
			push(input_number);
			result[i] = INT_MAX;
		} else if (!strcmp(input_command, "top")) {
			result[i] = top();
		} else if (!strcmp(input_command, "size")) {
			result[i] =  size();
		} else if (!strcmp(input_command, "empty")) {
			result[i] = empty();
		} else if (!strcmp(input_command, "pop")) {
			result[i] = pop();
		}
		getchar();
	}

	for (i = 0; i < command; ++i) {
		if (result[i] != INT_MAX) {
			printf("%d\n", result[i]);
		}
	}

	free(result);
	result = NULL;

	return 0;
}

void push(int value) {
	if (pos <= STACK_SIZE - 1) {
		stack[++pos] = value;	
	}
}

int pop() {
	if (!empty()) {
		return stack[pos--];
	} else {
		return -1;
	}
}

int size() {
	return pos + 1;
}

int empty() {
	return (pos == -1) ? 1 : 0;
}

int top() {
	if (!empty()) {
		return stack[pos];
	} else {
		return -1;
	}
}
