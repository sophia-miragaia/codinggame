#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define N_MAX 2000

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int index_max_element (int arr[], int n){
	int j=0;
	int max = arr[j];
	for (int i = 1; i < n; i++) {
		if (arr[i] > max) {
			max = arr[i];
			j=i;
		}
	}
	return j;
}

int main()
{
	int N;
	scanf("%d", &N);
	int C;
	scanf("%d", &C);

	int Budget [N_MAX];
	int TotalBudget = 0;

	for (int i = 0; i < N; i++) {
		int B;
		scanf("%d", &B);
		Budget[i] = B;
		TotalBudget += B;
	}

	if (TotalBudget < C) {
		printf("IMPOSSIBLE\n");
	}
	else{
		int j;
		while (TotalBudget > C) {
			j = index_max_element(Budget, N);
			Budget[j]--;
			TotalBudget--;
		}

		qsort(Budget, N, sizeof(int), cmpfunc);
		for (int i = 0; i < N; i++) {
			printf("%d\n",Budget[i]);
		}
	}
	return 0;
}