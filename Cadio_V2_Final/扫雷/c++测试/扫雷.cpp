#include <bits/stdc++.h>
#include <conio.h>>
#include <Windows.h>
int maxn=10,maxb=10;
//#define show_booms
using namespace std;
int a[100][100],b[100][100];
const int dx[]={-1,-1,-1,0,1,1,1,0};
const int dy[]={-1,0,1,1,1,0,-1,-1};
int flg,boom;
void instruction(){
	system("title 扫雷");
	cout<<"输入正方形边长和雷数"<<endl; 
	cin>>maxn>>maxb;
	cout<<"输入形式：(x,y,op);分别是坐标和操作。"<<endl;
	cout<<"坐标从1开始；操作0是踩，1是标记。"<<endl;
	cout<<"对于一个已经标记过的格子再次标记，是取消"<<endl;
	cout<<"#是可踩区域，*是无效区域，！是标记，调试模式里@是雷"<<endl;
	cout<<"按任意键开始，玩的愉快！";
	while(!_kbhit());
}
void prepare(int num,int bm_num){
	flg=bm_num;
	boom=bm_num;
	srand(time(0));
	for(int i=1;i<=bm_num;i++){
		int px=rand()%maxn+1;
		int py=rand()%maxn+1;
		if(a[px][py]==-1) bm_num++;//去重复 
		else a[px][py]=-1;	
	} 
	for(int i=1;i<=maxn;i++){
		for(int j=1;j<=maxn;j++){
			if(a[i][j]!=-1){
				int cnt=0;	
				for(int k=0;k<8;k++){
					int nx=i+dx[k];
					int ny=j+dy[k];
					if(nx>=1&&nx<=maxn&&ny>=1&&ny<=maxn&&a[nx][ny]==-1) cnt++;
				}
				a[i][j]=cnt;
			}
		}
	} 
}
void work(int si,int sj){
	if(a[si][sj]>=1&&a[si][sj]<=8){
		b[si][sj]=a[si][sj];
		return;
	}else if(b[si][sj]==-2){
		return;
	}else{
		b[si][sj]=-1;
		for(int k=0;k<8;k++){
			int nx=si+dx[k];
			int ny=sj+dy[k];
			if(nx>=1&&nx<=maxn&&ny>=1&&ny<=maxn&&b[nx][ny]==0){
				work(nx,ny);
			}
		}
	}
}
void update(bool showbm){
	system("cls");
	for(int i=1;i<=maxn;i++){
		for(int j=1;j<=maxn;j++){
			if(b[i][j]==-2) cout<<"!";
			else if(a[i][j]==-1&&showbm==1) cout<<"@"; //show booms
			else if(b[i][j]==-1) cout<<"*";
			else if(b[i][j]==0) cout<<"#";
			else if(b[i][j]>=1&&b[i][j]<=8) cout<<b[i][j];	
		} 
		cout<<endl;
	}
} 
int main(){
	instruction();
	prepare(maxn,maxb);
	update(0);
	while(1){
		int ix,iy,op=0;
		cin>>ix>>iy>>op;
		if(op==0){
			if(a[ix][iy]==-1){
				update(1);
				cout<<"FAIL!";
				break;
			}else{
				work(ix,iy);
				update(0);
			}
		}else if(op==1&&flg!=0){
			if(b[ix][iy]==0){
				b[ix][iy]=-2;
				update(0);
				flg--;
				if(a[ix][iy]==-1) boom--;
			}else if(b[ix][iy]==-2){
				b[ix][iy]=0;
				update(0);
				flg++;
				if(a[ix][iy]==-1) boom++;
			}
		}
		if(boom==0){
			Sleep(500);
			cout<<"WIN!";
			break;	
		}
	}
	getchar();
	return 0;
} 
