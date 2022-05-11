#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <time.h>
#define MAX 100001
using namespace std;
long long N;
vector<int> arr; //actual memorizing array
int isPrime[MAX];

int findGCD(int x, int y) {
	while(1) {
		int r = x%y;
		if(r == 0) return y;
		x = y;
		y = r;
	}
}

void primeNumber() { //find prime number and memorize into 'isPrime' vector
	for(int i=2; i<MAX; i++) isPrime[i] = i;
	for(int i=2; i<=sqrt(MAX); i++) {
		if(isPrime[i] == 0) continue;
		for(int j= i+i; j<MAX; j+=i) isPrime[j] = 0;
	}
}

int main() {
	ios::sync_with_stdio(false);
	int flag = 0;
	long long temp;
	clock_t start, end;
	while(1) {
		flag = 0;
		cout << "Input Size : ";
		cin >> N;
		if(N >= 31 || N <= 1) {
			cout << "your input data is too big or too small\n";
			continue;
		}
		cout << "Input Numbers : ";
		
		map <int, int> Map;
		for(int i=0; i<N; i++) {
			cin >> temp;
			if(temp >= MAX) {
				flag = 1;
				break;
			}
			Map[temp] = 1;
		}
		
		if(flag == 1) {
			cout << "your input data is too big!\n";
			continue;	
		}
		else if(Map.size() == 1) {
			cout << "your input data is overlapped, so actual number of input data is 1\n";
			continue;
		}
		start = clock();
		
		int size = Map.size();
		arr.resize(size, 0);
		int i=0;
		for(map<int,int>::iterator it = Map.begin(); it != Map.end(); it++) {
			arr[i++] = it->first;
		}
		
		int GCD = findGCD(arr[1], arr[0]);
		for(int i=2; i<size; i++) {
			GCD = findGCD(arr[i], GCD);
		}
		cout << "GCD of input is : " << GCD << '\n';
		primeNumber();
		for(int i=0; i<size-1; i++) {
			int numbers=0;
			for(int j=arr[i]; j <= arr[i+1]; j++) {
				if(isPrime[j] != 0) numbers++;
			}
			cout << "prime numbers between " << arr[i] << " and " << arr[i+1] << " is " << numbers << '\n';
		}
		break;
	}
	end = clock();
	double result = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "time left : " << result << "sec\n";
	return 0;
}
