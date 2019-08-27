#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main() {
	int num, result = 0;
	char* str, *ch;

	scanf("%d", &num);
	str = (char*) malloc(sizeof(char) * num);
	memset(str, 0, sizeof(str));	

	scanf("%s", str);
	
	for (ch = str; *ch != '\0'; ++ch) {
		if (isdigit(*ch)) {
			result += *ch - '0';
		}
	} 

	printf("%d", result);

	return 0;
}
