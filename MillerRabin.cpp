#include <iostream>
#include <cmath>
using namespace std;


unsigned long long int anp(unsigned long long int a, unsigned long long int n, unsigned long long int p) {
	unsigned long long int b = 1;
	a%=p;
	while (n--) {
		b = (b * a) % p;
	}
	return b;
}


int main() {
	unsigned long long int n, s, k; //n=2^s*k+1
	s = 0;
	// cin >> n;
	n=114553;
	if (n % 2 == 0 && n > 2) {
		cout << "n是合数";
	}
	unsigned long long int temp = n - 1;
	while (temp % 2 == 0) {
		temp /= 2;
		s++;
	}
	k = temp;
	cout << "s=" << s << ", k=" << k << endl;

	unsigned long long int t;
	// cin >> t; 
	t=10;
	unsigned long long int flag=0;
	while (t--) {
		unsigned long long int random = rand() % (n - 3) + 2;
		cout << "random=" << random << endl;
		unsigned long long int r;
		r = anp(random, k, n);
		cout << "r=" << r << endl;
		if ((r+n-1)%n==0||(r+1)%n==0) {
			flag=1;
			cout<<"phase1:r="<<r<<"\n";
			continue;
		} 
		flag = 0;
		unsigned long long int i = 0;	
		while (i < s - 1) {
			i++;
			r = (r*r)%n;
			cout<<"phase2:r="<<r<<"\n";
			if ((r+1)%n==0) {
				flag = 1;
				cout<<"phase2:i="<<i<<"\n";
				break;
			}
		}
		if (flag==0)break;
	}
    if(flag==1) {cout<<"n是素数\n";}
    else {cout<<"n是合数\n";}
	system("pause");
	return 0;
}

