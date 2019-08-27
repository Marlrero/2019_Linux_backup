#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double rounding(double x, int digit) {
	return ( floor( (x) * pow( 10, digit ) + 0.5f ) / pow( 10, digit ) );
}

int main() {
	int test_case, i, j, num_students, avg_over_students;
	double sum, avg;
	int* students_score;

	scanf("%d", &test_case);
	
	double* result = (double*) malloc(sizeof(double) * test_case);

	for (i = 0; i < test_case; ++i) {
		scanf("%d", &num_students);

		students_score = (int*) malloc(sizeof(int) * num_students);		
		sum = 0.0;
		avg_over_students = 0;

		for (j = 0; j < num_students; ++j) {
			scanf("%d", &students_score[j]);
			sum += students_score[j];
		}

		avg = sum / (double)num_students;	
	
		for (j = 0; j < num_students; ++j) {
			if (avg < (double)students_score[j]) {
				avg_over_students++;
			}
		}

		result[i] = rounding((double)avg_over_students / (double)num_students * 100, 3);
		
		// memory allocation free
		free(students_score);
		students_score = NULL;
	}

	for (i = 0; i < test_case; ++i) {
		printf("%.3lf%\n", result[i]);
	}

	free(result);
	result = NULL;

	return 0;
}
