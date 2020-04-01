#include <stdio.h>

#define ROWS 17
#define COLS 21
#define KERNEL_SIZE 3

void printImage(int image[][COLS]);
void getCNN(int image[][COLS], int kernel[][KERNEL_SIZE]);
void initMatchArr(int arr[][KERNEL_SIZE]);

int main(int argc, char* argv[]) {
	int image[ROWS][COLS] = { // 0 덧대기
		//1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 1	
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 2
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 6, 0, 0, 0, 0, 0 }, // 3
		{ 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 8, 8, 5, 0, 0, 0, 0, 0 }, // 4
		{ 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 3, 4, 5, 3, 0, 0, 0, 0, 0 }, // 5	
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 6
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 7
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0 }, // 8
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 7, 0, 0, 8, 8, 7, 0, 0, 0, 0 }, // 9
		{ 0, 0, 0, 0, 0, 0, 0, 7, 9, 7, 7, 6, 8, 8, 9, 0, 7, 6, 0, 0, 0 }, // 10
		{ 0, 0, 0, 0, 0, 7, 8, 7, 6, 0, 0, 0, 7, 7, 0, 0, 6, 8, 6, 0, 0 }, // 11
		{ 0, 0, 0, 0, 7, 8, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 0, 0, 0 }, // 12
		{ 0, 0, 0, 7, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 0, 0 }, // 13
		{ 0, 0, 7, 9, 7, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 7, 9, 5, 0, 0 }, // 14
		{ 0, 7, 8, 6, 0, 0, 0, 0, 0, 0, 6, 8, 0, 3, 8, 0, 7, 8, 6, 0, 0 }, // 15
		{ 0, 8, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 0 }, // 16
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }  // 17
	};

	int horizontal_kernel[KERNEL_SIZE][KERNEL_SIZE] = { { -1, 0, 1 },
							    { -1, 0, 1 },
							    { -1, 0, 1 } };

	int vertical_kernel[KERNEL_SIZE][KERNEL_SIZE] = { {  1,  1,  1 },
							  {  0,  0,  0 },
							  { -1, -1, -1 } };

	int diagonal_kernel[KERNEL_SIZE][KERNEL_SIZE] = { { 0, -1, -1 },
		      				          { 1,  0, -1 },
						          { 1,  1,  0 } };

	int custom_kernel[KERNEL_SIZE][KERNEL_SIZE] = { { 3, 3, 3 },
							{ 4, 4, 4 },
							{ 3, 3, 3 } };
	printf("Original Image\n");
	printImage(image);

	printf("1. convolution operation using horizontal kernel \n");
	getCNN(image, horizontal_kernel);

	printf("2. convolution operation using vertical kernel \n");
	getCNN(image, vertical_kernel);

	printf("3. convolution operation using diagonal kernel \n");
	getCNN(image, diagonal_kernel);

	printf("4. convolution operation using any kernel \n");
	getCNN(image, custom_kernel);

	return 0;
}

void printImage(int image[][COLS]) {
	int row, col;
	
	for (row = 0; row < ROWS; ++row) {
		for (col = 0; col < COLS; ++col) {
			printf("%3d ", image[row][col]);
		}
		printf("\n");
	}
	printf("\n");
}

void getCNN(int image[][COLS], int kernel[][KERNEL_SIZE]) {
	int row, col, i, j, sum;
	int match_kernel_pos[KERNEL_SIZE][KERNEL_SIZE];
	int result[ROWS][COLS];

	for (row = 1; row < ROWS - 1; ++row) { // 끝부분은 하지 않기 위해 1 빼기
		for (col = 1; col < COLS - 1; ++col) { // 끝부분은 하지 않기 위해 1 빼기
			initMatchArr(match_kernel_pos);

			// 현재 위치에서 매치되는 영역의 주변 검사
			match_kernel_pos[1][1] = image[row][col];

			if (row != 0 && col != 0) {
				match_kernel_pos[0][0] = image[row - 1][col - 1]; // 왼쪽 위
			}

			if (row != 0) {
				match_kernel_pos[0][1] = image[row - 1][col]; // 중앙 위
			}

			if (row != 0 && col != COLS - 1) {
				match_kernel_pos[0][2] = image[row - 1][col + 1]; // 오른쪽 위
			}

			if (col != 0) {
				match_kernel_pos[1][0] = image[row][col - 1]; // 왼쪽 중앙
			}

			if (col != COLS - 1) {
				match_kernel_pos[1][2] = image[row][col + 1]; // 오른쪽 중앙
			}

			if (row != ROWS - 1 && col != 0) {
				match_kernel_pos[2][0] = image[row + 1][col - 1]; // 왼쪽 아래	
			}

			if (row != ROWS - 1) {
				match_kernel_pos[2][1] = image[row + 1][col]; // 중앙 아래
			}

			if (row != ROWS - 1 && col != COLS - 1) {
				match_kernel_pos[2][2] = image[row + 1][col + 1]; // 오른쪽 아래
			}

			// convolution operation
			sum = 0;
			for (i = 0; i < KERNEL_SIZE; ++i) {
				for (j = 0; j < KERNEL_SIZE; ++j) {
					sum += match_kernel_pos[i][j] * kernel[i][j];
				}
			}
			result[row][col] = sum;
		}
	}

	printImage(result);
}

void initMatchArr(int arr[][KERNEL_SIZE]) {
	int row, col;
	
	for (row = 0; row < KERNEL_SIZE; ++row) {
		for (col = 0; col < KERNEL_SIZE; ++col) {
			arr[row][col] = 0;
		}
	}
}
