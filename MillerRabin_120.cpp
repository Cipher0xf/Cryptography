#include <iostream>
#include <cmath>
#include <string.h>
using namespace std;

typedef struct {
	unsigned long long int H;
	unsigned long long int L;
} int128;

void char_to_int128(char* x, int128* y){//二进制字符串->128位整型
    (*y).L=(*y).H=0;
    for(int i=0;i<strlen(x);i++){
        if(x[i]=='1'){
            if(i<64) (*y).L+=((unsigned long long int)1<<i);
            else (*y).H+=((unsigned long long int)1<<(i-64));  
        }
    }
}

void print_int128(int128 x){
    printf("2^64 × %llu + %llu\n", x.H, x.L);
}

int128 left(int128 x, int biase){//二进制左移
    if(biase==0) return x;
    int128 y;
    y.L=y.H=0;
    y.L=x.L<<biase;
    y.H=x.H<<biase;
    y.H+=x.L>>(64-biase);
    return y;
}

int128 right(int128 x, int biase){//二进制左移
    if(biase==0) return x;
    int128 y;
    y.L=y.H=0;
    y.L=x.L>>biase;
    y.H=x.H>>biase;
    y.L+=x.H<<(64-biase);
    return y;
}

int compare(int128 x, int128 y){//大整数比较
    if(x.H==y.H){
        if(x.L==y.L){
            return 0;
        } else if(x.L>y.L){
            return 1;
        } else {
            return -1;
        }
    } else if(x.H>y.H){
        return 1;
    } else {
        return -1;
    }
}

int128 add(int128 x, int128 y){//大整数加法
	int128 z;
    z.L=z.H=0;
	z.L = x.L + y.L;
	z.H = x.H + y.H;
	if(z.L<x.L){
		z.H += 1;
	}
	return z;
}

int128 minu(int128 x, int128 y){//大整数减法(x>=y)
	int128 z;
    z.L=z.H=0;
	z.L = x.L - y.L;
	z.H = x.H - y.H;
	if(z.L>x.L){
		z.H -= 1;
	}
	return z;
}

int128 mult(int128 x, int128 y){//大整数乘法
	int128 z;
    z.L=z.H=0;
    unsigned long long int L=y.L;
    unsigned long long int H=y.H;
    for(int i=0;i<64;i++){
        if(L%2==1){
            z=add(z, left(x, i));
        }
        L=L>>1;
    }
    for(int i=0;i<64;i++){
        if(H%2==1){
            z.H+=x.L<<i;
        }
        H=H>>1;
    }
	return z;
}

int128 modulu(int128 a, int128 p){//a模p
	int128 b=a;
    while(compare(b,p)>=0){
        b=minu(b,p);
    }
    return b;
}

int128 anp(int128 a, int128 n, int128 p){//a的n次方模p
    int128 _1;
    _1.H=0,_1.L=1;
    int128 b=_1;
    a=modulu(a,p);
    while(n.H>0||(n.H==0&&n.L>0)){
        b=mult(b,a);
        b=modulu(b,p);
        n=minu(n,_1);
    }
    return b;
}


int main(){
//120比特大整数，用二进制字符串表示
    char b[]="111100000001100010111";
    if(b[strlen(b)-1]=='0'){
        cout<<"偶数n是合数,请输入奇数\n";
        system("pause");
        return 0;
    }
    char* p1=b;
    char* p2=b+strlen(b)-1;
    while(p1<p2){
        char t=*p1;
        *p1=*p2;
        *p2=t;
        p1++;
        p2--;
    }

    int128 _0,_1;
    _0.H=0,_0.L=0;
    _1.H=0,_1.L=1;
    int128 n;
    char_to_int128(b, &n);
    if(compare(n,_1)==0){
        cout<<"1不是素数,请输入其他奇数\n";
        system("pause");
        return 0;
    }
    cout<<"大奇整数n=";print_int128(n);
    int128 temp = minu(n, _1);

//n=2^s*k+1
    int s=0;
    for(int i=1;i<strlen(b);i++){
        if(b[i]=='1'){
            s=i;
            break;
        }
    }
    cout<<"s="<<s<<endl;

    int128 k=right(n,s);
    cout<<"k=";print_int128(k);
    cout<<"n=2^s*k+1\n";

//安全参数t
    int t,T;
    T=t=3;
    int flag=0;
    cout<<"安全参数t="<<t<<endl;
    while(t--){
        cout<<"\ntest"<<T-t<<":\n";
        int128 random;
        random.L=random.H=0;
//生成随机数random
        for(int i=0;i<16;i++){
            random.L+=(unsigned long long int)(rand()%2)<<i;
        }
        random=modulu(random,n);
        cout<<"random mod n="<<random.L<<endl;
//MillerRabin素性检验
        int128 r;
        r=anp(random, k, n);
        cout<<"r=";print_int128(r);
        if((compare(modulu(add(r,temp),n),_0)==0||compare(modulu(add(r,_1),n),_0)==0)){
            flag=1;
            cout<<"phase1 pass (r≡1 or n-1 mod n)\n";
            continue;
        }
        flag=0;
        int i=0;
        while(i<s-1){
            i++;
            r=modulu(mult(r,r),n);
            if(compare(modulu(add(r,_1),n),_0)==0){
                flag=1;
                cout<<"phase2 pass (r≡n-1 mod n)\n";
                break;
            }
        }
        if(flag==0)break;
    }
    if(flag==1) {cout<<"\nn是素数\n";}
    else {cout<<"\nn是合数\n";}

    system("pause");
    return 0;
}