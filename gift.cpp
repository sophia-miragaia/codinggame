#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#define N_MAX 2000

int index_max_element(int arr[], int n)
{
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
	cin >> N; cin.ignore();

	int C;
	cin >> C; cin.ignore();

	int Budget [N_MAX];
	int TotalBudget = 0;

	for (int i = 0; i < N; i++) {
		int B;
		cin >> B; cin.ignore();
		Budget[i] = B;
		TotalBudget += B;
	}

	if(TotalBudget < C){
		cout << "IMPOSSIBLE" << endl;
	}
	else{

		while (TotalBudget > C){
			int j = index_max_element(Budget, N);
			Budget[j]--;
			TotalBudget--;
		}
		vector<int> Payment(Budget, Budget + N);
		sort(Payment.begin(), Payment.end());

		for (auto a : Payment) {
			cout << a << endl;
		}
	}
}