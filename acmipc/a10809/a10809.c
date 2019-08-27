#include <stdio.h>
#include <ctype.h>

int main() {
	int alpha[26][2];
	char word[100] = { '0' };
	int i;

	for (i = 0; i < 26; ++i) {
		alpha[i][0] = -1; // 위치를 저장
		alpha[i][1] = 0; // 이미 검사한 것을 1로 표시
	}

	scanf("%s", word);
	
	for (i = 0; i < 100; ++i) {
		if (isalpha(word[i]) && alpha[word[i] - 'a'][1] == 0) {
			alpha[word[i] - 'a'][0] = i;
			alpha[word[i] - 'a'][1] = 1;
		}
	}

	for (i = 0; i < 26; ++i) {
		printf("%d ", alpha[i][0]);
	}

	return 0;
}
