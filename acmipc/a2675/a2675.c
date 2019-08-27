#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	int test, i, iterative[1000] = { -1 };
	char* arr_str[1000] = { NULL };

	scanf("%d", &test);

	for (i = 0; i < test; ++i) {
		char str[20] = "";
		scanf("%d %s", &iterative[i], str);
		char* mem = (char*)malloc(sizeof(char) * 20);
		strcpy(mem, str);
		arr_str[i] = mem;
	}

	char* start = NULL;
	int loop;
	for (i = 0; i < test; ++i) {
		for (start = arr_str[i]; *start != '\0'; ++start) {
			for (loop = 0; loop < iterative[i]; ++loop) {
				printf("%c", *start);
			}
		}
		printf("\n");
	}

	for (i = 0; i < test; ++i) {
		free(arr_str[i]);
		arr_str[i] = NULL;
	}

	return 0;
}
