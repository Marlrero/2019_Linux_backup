#include <stdio.h>
#include <string.h>

int main(void)
{
	char buf[30];
	
	buf[0] = (char)strlen("Hello\n") + 1;
	strcpy(buf + 1, "Hello\n");
	printf("%s", buf);

	return 0;
}
