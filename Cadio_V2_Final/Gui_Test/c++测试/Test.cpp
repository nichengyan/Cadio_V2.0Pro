#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#define rt(a,b) (a*b);
using namespace std;
unsigned char Gui_IdMap[2]; 
uint8_t Gui_GetPageId(){ //0~15;  0:Unused 1:Used
    uint8_t i,j,BitCheck,Cnt=0;
    for(i=0;i<2;i++){
        BitCheck=0x80;  
        for(j=0;j<8;j++){
            if(((~Gui_IdMap[i])&BitCheck)){ //Unused    1101 1111 
                Gui_IdMap[i]|=BitCheck;
                return i*8+j;
            }
            BitCheck>>=1;
        }
    }
    return 0;  //不可能返回0,因为0号页面一定被使用了
}   
uint8_t  Gui_FreePageId(unsigned char n){
    uint8_t i,BitCheck=0x80;
    if(n==0) return 0xff;//invalid
    if((~Gui_IdMap[n/8])&(BitCheck>>(n%8))) return 0xff;
    Gui_IdMap[n/8]&=(~(BitCheck>>(n%8)));
    return 0;
} 
int main(){
	int s,num;
	cout<<rt(2,4);
	for(int i=0;i<2;i++) Gui_IdMap[i]=0x00;
	while(1){
		for(int i=0;i<2;i++){
			unsigned char tmp=0x80;
			for(int j=0;j<8;j++){
			    if(tmp&Gui_IdMap[i]) cout<<1;
				else cout<<0;
				tmp>>=1;	
			}
		}
		cout<<endl;
		cin>>s;
		if(s){
            cout<<(int)Gui_GetPageId();			
		}else{
			cin>>num;
			cout<<(int)Gui_FreePageId(num);
		}	
		
		
		cout<<endl;
	}
    
}
