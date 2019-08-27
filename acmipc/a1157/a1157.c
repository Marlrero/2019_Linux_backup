#include <stdio.h>

void strupr(char* word) {
	for (; *word != '\0'; ++word) {
		if (*word >= 'a' && *word <= 'z') {
			*word = *word - 32;
		}
	}
}

int main() {
	char word[1000000];
	int freq[26];
	char* chptr = NULL;
	int i;

	// init freq -> 0과 0은 같게 처리됨
	for (i = 0; i < 26; ++i) {
		freq[i] = i - 26;
	}

	scanf("%s", word);
	strupr(word);

	for (chptr = &word[0]; *chptr != '\0'; ++chptr) {
		printf("debug >> %d \n", *chptr - 'A');
		if (freq[*chptr - 'A'] < 0)
			freq[*chptr - 'A'] = 1;
		else
			freq[*chptr - 'A']++;
	}

	int max_idx = 0,  max = freq[0], isOverlap = 0;
	for (i = 1; i < 26; ++i) {
		if (freq[i] > max) {
			max_idx = i;
			max = freq[i];
			isOverlap = 0;
		} else if (freq[i] == max) {
			isOverlap = 1;
		}
	}

	if (isOverlap) {
		printf("?");
	} else {
		printf("%c", 'A' + max_idx);
	}

	return 0;
}
