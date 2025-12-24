#include <bits/stdc++.h>
#include <conio.h>
#include <Windows.h>
using namespace std;
const int maxn=20;
struct pos{
	int x,y;
}q[100000],head;
struct w{
	int value;
	pos p;
}award;
int f[maxn*2][maxn*2];
int vis[maxn*2][maxn*2];
pos blocks[maxn*40];
int cnt=1;
int front=0,rear=0;
int op,d,dire,gamemode;
int alpha=0;
int pts=0;
int level;
int rcd=0;
int dx[5]={-1,1,0,0};
int dy[5]={0,0,-1,1};
struct cmp{
	bool operator()(const pos& a,const pos& b){
		if(a.x!=b.y) return a.x<b.x;
		else return a.y<b.y;
	}
};
map<pos,int,cmp>snake;
void read_rcd_file(){
	freopen("rcd_memory.data","r",stdin);
	cin>>rcd;
	fclose(stdin);
	return;
}
void write_rcd_file(){
	freopen("rcd_memory.data","w",stdout);
	rcd=max(rcd,pts);
	cout<<rcd;
	fclose(stdout);
}
void outit(){
	cout<<"your points:"<<pts<<endl;
	cout<<"record:"<<rcd<<endl;
	for(int i=1;i<=maxn;i++){
		for(int j=1;j<=maxn;j++){
			if(f[i][j]==2) putchar('#');
			else if(f[i][j]==3) putchar('!');
			else{
				if(f[i][j]==1) putchar(' ');
				else putchar('*');	
				f[i][j]=0;				
			}
		}
		cout<<endl;
	}
}
int dire_control(int k){
	if(k==119) return 0;
	if(k==115) return 1;
	if(k==97) return 2;
	if(k==100) return 3;
	else return 1;
}
void set_award(){
	award.p.x=(rand())%(maxn-1)+1;
	award.p.y=(rand())%(maxn-1)+1;
	award.value=(rand()%10);
	pts+=award.value;
	alpha=1;
}
void instruction_C(){
	cout<<"NCY:欢迎游玩！"<<endl;
	cout<<">>游戏时用W,S,A,D控制上下左右"<<endl;
	cout<<">>开始前请输入两个数字（中间空一格）选择等级与模式"<<endl;
	cout<<">>等级1~5,越大越快"<<endl;
	cout<<">>模式1~4, 分别为 有界模式 无限模式 截断模式 障碍模式"<<endl;
	cout<<"-------------------------"<<endl;
}
void instruction_E(){
	cout<<"NCY:welcome to play!"<<endl;
	cout<<">>during the game,W,S,A,D are the avaluable key only"<<endl;
	cout<<"pressing another keys would cause some unknown errors"<<endl;
	cout<<">>press"<<"W,S,A,D"<<"for up,down,left,right"<<endl;
	cout<<">>the game ends if you hit the edge,or yourself body"<<endl;
	cout<<">>eat the # to score"<<endl;
	cout<<">>choose the level first and start by pressing these "<<endl;
	cout<<"avaluable keys!"<<endl;
	cout<<"-------------------------"<<endl;
}
void prepare(){
	srand(time(0));	
	head.x=7+(rand())%5;
	head.y=7+(rand())%5;
	q[front].x=head.x;
	q[front].y=head.y;
	vis[head.x][head.y]=1;
	front++;
	instruction_C();
	cin>>level>>gamemode;
	cout<<">>按下W,S,A,D中任意键开始"<<endl;
	int dire=getch();
}
void delay(int level){
	if(level==1) Sleep(300);
	else if(level==2) Sleep(150);
	else if(level==3) Sleep(50);
	else if(level==4) Sleep(5);	
	else if(level==5) 1;//Sleep(1);	
	else Sleep(1000);
}
void endless(bool open){
	if(open==false) return;
	if(head.x>20) head.x=head.x-20;
	if(head.x<1) head.x=20+head.x;
	if(head.y>20) head.y=head.y-20;
	if(head.y<1) head.y=20+head.y;	
}
void end_fail(){
	cout<<"FAILED WITH "<<pts<<" PTS!";
	getchar();
	write_rcd_file();
	getchar();
	exit(0);
}
void create_blocks(int level){
	for(int i=1;i<=(rand()%level)*level/2;i++){
		int bx=rand()%21+1;
		int by=rand()%21+1;
		if(bx!=award.p.x&&by!=award.p.y){
			vis[bx][by]=1;
			blocks[cnt].x=bx;
			blocks[cnt].y=by;
			cnt++;			
		}else continue;
	}
}
int main(){
	prepare();
	read_rcd_file(); 
	while(1){		
	bool flg=1;
		if(alpha==0){
			set_award();
			f[award.p.x][award.p.y]=2;			
		}
		cout<<award.p.x<<" "<<award.p.y<<endl;
		outit();
		delay(level);
		system("cls");
		if(_kbhit()) dire=getch();
		d=dire_control(dire);
		if(op+d==1||op+d==5) d=op;			
		op=d;
		head.x=head.x+dx[d];
		head.y=head.y+dy[d]; 		
		if(head.x==award.p.x&&head.y==award.p.y){
			alpha=0;
			f[award.p.x][award.p.y]=0;	
			rear--;
		}
		
		if(gamemode==1){
			if(vis[head.x][head.y]||head.x>20||head.x<1||head.y>20||head.y<1) end_fail();
		}else if(gamemode==2){
			endless(gamemode);
			if(vis[head.x][head.y]==1) end_fail();
		}else if(gamemode==3){
			if(head.x>20||head.x<1||head.y>20||head.y<1) end_fail();
			if(vis[head.x][head.y]==1)  
				for(int i=rear;i<=front;i++) if(q[i].x==head.x&&q[i].y==head.y){rear=i;break;}
		}else if(gamemode==4){
			if(alpha==0) create_blocks(level);;
			if(vis[head.x][head.y]||head.x>20||head.x<1||head.y>20||head.y<1||f[head.x][head.y]==3) end_fail();
		}else{
			if(vis[head.x][head.y]||head.x>20||head.x<1||head.y>20||head.y<1) end_fail();
		}
		
		vis[head.x][head.y]=1;
		q[front].x=head.x;
		q[front].y=head.y;
		front++;
		vis[q[rear].x][q[rear].y]=0;
		rear++;
		for(int i=rear;i<=front;i++){
			f[q[i].x][q[i].y]=1;
		}
		for(int i=1;i<cnt;i++){
			f[blocks[i].x][blocks[i].y]=3;
		}
	}
	getchar();
	return 0;
}
