#include <bits/stdc++.h>
using namespace std;
int main(){
	freopen("a.txt","w",stdout);
	printf("uint8_t code Volume[][256] = {\n  ");
	for(int i = 0; i <= 20 ; i++){
		printf("{\n");
		for(int j = 0 ; j <256 ; j ++){
			int tmp =  min (255,(int)(j * 0.1*(i)));
			printf("%-3d", tmp);
			if(j != 255) printf(",");
			if(j&&j%32 == 0) printf("\n");
		}
		printf("\n},\n");
	}
}
