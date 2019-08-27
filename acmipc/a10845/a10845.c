#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define QUEUE_SIZE 10000

int queue[QUEUE_SIZE];
int front_pos = 0, back_pos = -1;

void push(int value);
int pop();
int size();
int empty();
int front();
int back();

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
		} else if (!strcmp(input_command, "pop")) {
			result[i] = pop();
		} else if (!strcmp(input_command, "size")) {
			result[i] =  size();
		} else if (!strcmp(input_command, "empty")) {
			result[i] = empty();
		} else if (!strcmp(input_command, "front")) {
			result[i] = front();
		} else if (!strcmp(input_command, "back")) {
			result[i] = back();
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
	if (front_pos <= QUEUE_SIZE - 1) {
		queue[++back_pos] = value;	
	}
}

int pop() {
	if (!empty()) {
		return queue[front_pos++];
	} else {
		return -1;
	}
}

int size() {
	return back_pos - front_pos + 1;
}

int empty() {
	return (front_pos > back_pos) ? 1 : 0;
}

int front() {
	if (!empty()) {
		return queue[front_pos];
	} else {
		return -1;
	}
}

int back() {
	if (!empty()) {
		return queue[back_pos];
	} else {
		return -1;
	}
}
