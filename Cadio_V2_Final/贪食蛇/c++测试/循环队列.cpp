#include <bits/stdc++.h>
using namespace std;
const int maxn=10;
int q[maxn];
int front=0,rear=0;
bool is_empty1(){
    return front==rear;
}
bool is_full(){
    return (front+1)%maxn==rear;
}
void En(int element){
	if(is_full()){
		cout<<"Error:Full£¡"<<endl;
		return;
	}
	q[front]=element;
	front=(front+1)%maxn;
	return;
}
int De(){
	int tmp=0;
	if(is_empty1()){
		cout<<"Error:Empty£¡"<<endl;
		return -1;
	}
	tmp=q[rear];
	rear=(rear+1)%maxn;
	return tmp;		
}
int main(){
	int op,num;
	while(1){

		cin>>op;
		if(op==1){
		    cin>>num;
			En(num);    	
		}
		if(op==2){
			cout<<De()<<endl;
		}		
		int i=rear;
		while(i!=front){
		    cout<<q[i]<<" ";
		    i=(i+1)%maxn;
       }
		cout<<endl<<front<<" "<<rear<<" "<<q[(front+maxn-1)%maxn]<<endl;
		cout<<endl;
	} 
	return 0;
} 
