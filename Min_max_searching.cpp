#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#define ROWS 30000
#define COLUMN 25000
#define NUM 4

int main() {

	double t1, t2; 
	t1 = omp_get_wtime();
	srand(time(NULL));
	omp_set_num_threads(NUM);
	int i, k;
	double min, max;
	double ** arr_a;
	//**-указатель на массив указателей
	arr_a = (double**)calloc(ROWS, sizeof(double*));
	if (arr_a == NULL) { fprintf(stderr, "Lack of memory\n"); exit(1); }
	//выделяем память под массив указателей
	for (i = 0; i < ROWS; i += 1) {
		//выделяем память под каждый элемент массива arr_a 
		arr_a[i] = (double*)calloc(COLUMN, sizeof(double));
		if (arr_a[i] == NULL) { fprintf(stderr, "Lack of memory\n"); exit(1); }
	}
	//-----------------------------------------
#pragma omp parallel for private (i,k) collapse(2)
	//-----------------------------------------
	for (i = 0; i < ROWS; i += 1) {
		for (k = 0; k < COLUMN; k += 1) {
			arr_a[i][k] = sqrt((double)rand()) + 2;
			//приводим к double так как функция rand возвращает int значение
		}
	}
	max = arr_a[0][0];
	min = arr_a[0][0];
	//-----------------------------------------
#pragma omp parallel for private(i,k) reduction(min: min) reduction(max: max) collapse(2)
	//-----------------------------------------
	for (int i = 0; i < ROWS; i += 1) {
		for (int k = 0; k < COLUMN; k += 1) {
			(arr_a[i][k] > max) ? max = arr_a[i][k] : max;
			(arr_a[i][k] < min) ? min = arr_a[i][k] : min;
		}
	}
	printf("Element %f\n", arr_a[100][100]);
	printf("Max element is %f\n", max);
	printf("Min element is %f\n", min);
	for (int i = 0; i < ROWS; i += 1) {
		free(arr_a[i]);
	}
	free(arr_a);
	t2 = omp_get_wtime();
	printf("Spent time is %f\n", t2 - t1);
	system("pause");
	return 0;
}





