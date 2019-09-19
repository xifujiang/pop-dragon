#include<stdio.h>
#include<windows.h>
#include <graphics.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include <mmsystem.h>  //音乐
#pragma comment(lib,"WINMM.LIB")
typedef struct position
{
    int x;
    int y;
    int bubble;
    int flag;
    int flag1;///消除泡泡
    int flag2;///掉落泡泡
    int x1;
    int y1;
} position;

typedef struct emission
{
    int bubble;
    int x;
    int y;
    int x1;
    int y1;
} emi;


void *thr_fn(void *args);
void  mainloop();
void *key_loop(void *arg);
void *up_loop(void *arg);
void *play_music(void *ret);//发射音效
int AttachmentJudge(position pos[12][9],int i,int j);///判断泡泡是否附着
pthread_t uploop;
pthread_t keyloop;

time_t start_time, end_time, start_time_1, end_time_1;
pthread_t ppid;//创建音效线程
int pos_bubble[12][9];
position pos[12][9],pos1[12][9];///pos[12][9]是右边，pos1[12][9]是左边；
int b_bubble[20000];
emi b[20000],b1[20000];
emi add_b[7],small_b;///add_b[]是增加泡泡
PIMAGE wheel[33],back1,back2[2],a[8],lose,win,baozha[7],Spop[2];
PIMAGE bluebmp,redbmp;
PIMAGE bmp[3];
PIMAGE timex,time1[6];
PIMAGE moun,explain;
int score0 = 0,score1 = 0;
int amount[2] = {0};///两边特殊泡泡数量，amount[0]是右边,amount[1]是左边:
int n1 = 500, m1 = 450;///右边要发射的泡泡位置
int n2 = 406, m2 = 473;///右边预发射泡泡位置
int n3 = 455, m3 = 380;///右边弓箭位置
int n11 = 133, m11 = 450;///左边要发射的泡泡位置
int n22 = 39, m22 = 473;///左边预发射泡泡位置
int n33 = 88, m33 = 380;///左边弓箭位置
int p = 16, q = 0;///右边弓箭方向
int p1 = 16,q1 = 0;///左边弓箭方向
int emil[102][2] = {0},z = 0;
int KO = 1;///是否死亡
int angle[33][2] = {            -16,-3,        ///-80
                                -16,-4,        ///-75
                                -16,-5,        ///-70
                                -16,-6,        ///-65
                                -17,-10,       ///-60
                                -13,-10,       ///-55
                                -11,-10,       ///-50
                                -10,-10,       ///-45
                                -14,-17,       ///-40
                                -11,-17,       ///-35
                                -10,-17,       ///-30
                                -7,-17,        ///-25
                                -6,-16,        ///-20
                                -5,-16,        ///-15
                                -4,-16,        ///-10
                                -3,-16,        ///-5
                                0,-20,         ///0
                   };
int add_angle[7][2]= { -9,-10, ///横坐标减7，纵坐标减10
                       -8,-11,
                       -7,-12,
                       -6,-12,
                       -3,-12,
                       -4,-13,
                       -2,-13,
                     };
void Lanuch_Angle()///正向角度
{
    int i;
    for(i = 17; i<33; i++)
    {
        angle[i][0] = angle[32-i][0]*(-1);
        angle[i][1] = angle[32-i][1];
    }
}

void Init_Pop()///初始化泡泡
{
    int i;
    for(i = 0; i<8; i++)
        a[i] = newimage();
    getimage(a[0],"black.png");
    getimage(a[1],"red.png");
    getimage(a[2],"blue.png");
    getimage(a[3],"orange.png");
    getimage(a[4],"green.png");
    getimage(a[5],"white.png");
    getimage(a[6],"yellow.png");
    getimage(a[7],"purple.png");
}

void Init_drap_Pop()///爆炸动画
{
    int i;
    for(i = 0; i<7; i++)
        baozha[i] = newimage();
    getimage(baozha[0],"1.png");
    getimage(baozha[1],"2.png");
    getimage(baozha[2],"3.png");
    getimage(baozha[3],"4.png");
    getimage(baozha[4],"555.png");
    getimage(baozha[5],"6.png");
    getimage(baozha[6],"7.png");
}

void Init_time()
{
    int i;
    timex = newimage();
    for(i = 1; i<6; i++)
        time1[i] = newimage();
    getimage(timex,"time.png");
    getimage(time1[1],timex,2,2,37,57);
    getimage(time1[2],timex,38,1,50,58);
    getimage(time1[3],timex,85,0,37,58);
    getimage(time1[4],timex,126,1,37,58);
    getimage(time1[5],timex,170,1,37,58);
}

void Init_Wheel()
{
    int i;
    for(i= 0; i<33; i++)
        wheel[i] = newimage();
    getimage(wheel[0],"-80.png");
    getimage(wheel[1],"-75.png");
    getimage(wheel[2],"-70.png");
    getimage(wheel[3],"-65.png");
    getimage(wheel[4],"-60.png");
    getimage(wheel[5],"-55.png");
    getimage(wheel[6],"-50.png");
    getimage(wheel[7],"-45.png");
    getimage(wheel[8],"-40.png");
    getimage(wheel[9],"-35.png");
    getimage(wheel[10],"-30.png");
    getimage(wheel[11],"-25.png");
    getimage(wheel[12],"-20.png");
    getimage(wheel[13],"-15.png");
    getimage(wheel[14],"-10.png");
    getimage(wheel[15],"-5.png");
    getimage(wheel[16],"0.png");
    getimage(wheel[17],"5.png");
    getimage(wheel[18],"10.png");
    getimage(wheel[19],"15.png");
    getimage(wheel[20],"20.png");
    getimage(wheel[21],"25.png");
    getimage(wheel[22],"30.png");
    getimage(wheel[23],"35.png");
    getimage(wheel[24],"40.png");
    getimage(wheel[25],"45.png");
    getimage(wheel[26],"50.png");
    getimage(wheel[27],"55.png");
    getimage(wheel[28],"60.png");
    getimage(wheel[29],"65.png");
    getimage(wheel[30],"70.png");
    getimage(wheel[31],"75.png");
    getimage(wheel[32],"80.png");
}

void Init_SpecialPop()
{
    int i;
    for(i =0; i<2; i++)
        Spop[i] = newimage();
    getimage(Spop[0],"big.png");
    getimage(Spop[1],"small.png");
}

void Init_Bmp()
{
    redbmp = newimage();
    bluebmp = newimage();
    getimage(redbmp,"red2.png");
    getimage(bluebmp,"blue2.png");
    int i;
    for(i =0; i<3; i++)
        bmp[i] = newimage();
    getimage(bmp[0],redbmp,5,5,27,29);
    getimage(bmp[1],redbmp,238,6,29,29);
    getimage(bmp[2],bluebmp,238,4,29,29);
}
void Init_flag2(position pos[12][9])
{
    int i,j;
    for(i = 0; i<12; i++)
        for(j = 0; j<9; j++)
            pos[i][j].flag2 = 0;
    int k,flag= 0;
    for(k = 0; k<8; k++)
    {
        if(pos[1][k].flag == 1)///第二排有泡泡
        {
            flag = 1;
            break;
        }
    }

    if(flag==0)
    {
        for(k = 0; k<9; k++)
        {
            if(pos[0][k].flag==1)
                pos[0][k].flag2 = 1;
        }
    }
    else
    {
        for(i =0 ; i<9; i++)
            if(pos[0][i].flag==1 && pos[0][i].flag2==0)
            {
                pos[0][i].flag2 = 1;
                AttachmentJudge(pos,0,i);
            }
    }
}

void Init_Add_b(int s)///给对手放泡后随机生成的泡泡
{
    srand(time(0));
    int i;
    for(i = 0; i<7; i++)
    {
        add_b[i].bubble = rand()%8;
        if(s == 2 )
        {
            add_b[i].x = 649;
            add_b[i].y = 513;
        }
        if(s == 7)
        {
            add_b[i].x = 288;
            add_b[i].y = 513;
        }
    }
}

void Init_bubble()///初始化flag,坐标和四行泡泡
{
    srand(time(0));
    int i,j,w,n = 363,m = 64;
    for(i = 0; i<12; i++)
    {
        for(j = 0; j<9; j++)
        {
            pos[i][j].flag = 0;
            pos1[i][j].flag = 0;
        }
    }
    for(i = 0; i<12; i++)
    {
        if(i%2 == 0)
        {
            w = 9;
            n = 363;
        }
        else
        {
            w = 8;
            n = 379;
        }
        for(j = 0; j<w; j++)
        {
            pos[i][j].x = n;
            pos[i][j].y = m;
            pos[i][j].x1 = n+16;
            pos[i][j].y1 = m+16;///右边
            pos1[i][j].x = n-363;
            pos1[i][j].y = m;
            pos1[i][j].x1 = n-363+16;
            pos1[i][j].y1 = m+16;///左边
            n+=32;
        }
        m+=30;
    }
    for(i = 0; i<4; i++) ///打印四行泡泡
    {
        if(i%2==0) w = 9;
        else w = 8;
        for(j = 0; j<w; j++)
        {
            pos_bubble[i][j] = rand()%8;
            pos[i][j].bubble=pos_bubble[i][j];
            pos1[i][j].bubble=pos_bubble[i][j];
            pos[i][j].flag = 1;
            pos1[i][j].flag = 1;
        }
    }
}

void Init_Emission()///初始化20000个泡泡
{
    srand(time(0));
    int i;
    for(i = 0; i<20000; i++)
    {
        b_bubble[i] = rand()%8;
        b[i].bubble = b_bubble[i];
        b[i].x = n1;
        b[i].y = m1;
        b1[i].bubble = b_bubble[i];
        b1[i].x = n11;
        b1[i].y = m11;
    }
}

void score_print(int n)
{
    char s[] = "";
    setcolor(EGERGB(0xEA, 0x9A, 0xBB));
    setfont(21,20, "华文隶书");
    if(n==1)
    {
        sprintf(s, "%d", score1);
        outtextxy(75, 38, s);
    }
    else if(n==0)
    {
        sprintf(s, "%d", score0);
        outtextxy(438, 38, s);
    }
}
void play_BK_BGM()
{
    mciSendString(TEXT("open bk.mp3 alias BGM"), NULL, 0, NULL);
    mciSendString(TEXT("play BGM repeat"), NULL, 0, NULL);
}
void close_BK_BGM()
{
    mciSendString(TEXT("stop bk.mp3 alias BGM"), NULL, 0, NULL);
    mciSendString(TEXT("close BGM"), NULL, 0, NULL);
}
void play_BK1_BGM()
{
    mciSendString(TEXT("open bk1.mp3 alias BGM"), NULL, 0, NULL);
    mciSendString(TEXT("play BGM repeat"), NULL, 0, NULL);
}
void close_BK1_BGM()
{
    mciSendString(TEXT("stop bk1.mp3 alias BGM"), NULL, 0, NULL);
    mciSendString(TEXT("close BGM"), NULL, 0, NULL);
}
void *play_music(void *ret)//发射音效
{
    char temp = *(char *)ret;
    if (temp == 't')
    {
        mciSendString(TEXT("open begin.mp3 alias begin"), NULL, 0, NULL);
        mciSendString(TEXT("play begin"), NULL, 0, NULL);
    }
    else if (temp == 'e')
    {
        mciSendString(TEXT("open win.mp3 alias win"), NULL, 0, NULL);
        mciSendString(TEXT("play win"), NULL, 0, NULL);
    }
    else if (temp == 'r')
    {
        mciSendString(TEXT("open fantan.mp3 alias fantan"), NULL, 0, NULL);
        mciSendString(TEXT("play fantan"), NULL, 0, NULL);
    }
    else if (temp == 'y')
    {
        mciSendString(TEXT("open playpao.mp3 alias playpao"), NULL, 0, NULL);
        mciSendString(TEXT("play playpao"), NULL, 0, NULL);
    }
    else if (temp == 'u')
    {
        mciSendString(TEXT("open xiaochu.mp3 alias xiaochu"), NULL, 0, NULL);
        mciSendString(TEXT("play xiaochu"), NULL, 0, NULL);
    }
    else if (temp == 'i')
    {
        mciSendString(TEXT("open playtoenemy.mp3 alias playtoenemy"), NULL, 0, NULL);
        mciSendString(TEXT("play playtoenemy"), NULL, 0, NULL);
    }
    else if (temp == 'o')
    {
        mciSendString(TEXT("open diaoluo.mp3 alias diaoluo"), NULL, 0, NULL);
        mciSendString(TEXT("play diaoluo"), NULL, 0, NULL);
    }

}
void map_1(int n)///右边打印泡泡
{
    int i,j;
    if(n==0)
    {
        for(i = 0; i<12; i++)
            for(j = 0; j<9; j++)
            {
                if(pos[i][j].flag == 1)
                {
                    putimage_transparent(NULL,a[pos[i][j].bubble],pos[i][j].x,pos[i][j].y,BLACK);
                }
            }
        if(amount[0]!=0)
        {
            int x = 336, y = 49;
            //int x1 = 293,y1 = 49;
            for(i = 0; i<amount[0]/7; i++)
            {
                putimage_transparent(NULL,Spop[0],x,y,BLACK);
                y+=20;
            }
            for(i = 0; i<amount[0]%7; i++)
            {
                putimage_transparent(NULL,Spop[1],x-2,y,BLACK);
                if((i+1)%3) x+=8;
                else
                {
                    x= 336;
                    y+=8;
                }
            }
        }
    }
    else if(n==1)
    {
        for(i = 0; i<12; i++)
            for(j = 0; j<9; j++)
            {
                if(pos1[i][j].flag == 1)
                {
                    putimage_transparent(NULL,a[pos1[i][j].bubble],pos1[i][j].x,pos1[i][j].y,BLACK);
                }
            }
        if(amount[1]!=0)
        {
            int x1 = 293,y1 = 49;
            for(i = 0; i<amount[1]/7; i++)
            {
                putimage_transparent(NULL,Spop[0],x1,y1,BLACK);
                y1+=20;
            }
            for(i = 0; i<amount[1]%7; i++)
            {
                putimage_transparent(NULL,Spop[1],x1,y1,BLACK);
                if((i+1)%3) x1+=8;
                else
                {
                    x1= 293;
                    y1+=8;
                }
            }
        }
    }
}

void InitFlag1()///初始化flag1（标记这个泡泡是不是跟放上去的泡泡相同）
{
    int i,j;
    for(i = 0; i<12; i++)
        for(j = 0; j<9; j++)
        {
            pos[i][j].flag1 = 0;
            pos1[i][j].flag1 = 0;
        }
}

void eliminate(int i, int j,emi b[],int q,position pos[12][9])///判断发射的泡泡周围是否有相同颜色的泡泡
{
    emil[z][0] = i;
    emil[z++][1] = j;
    pos[i][j].flag1 = 1;
    if(i%2==0)///奇数排
    {
        if(i>0 && j>0 && pos[i-1][j-1].flag==1 &&pos[i-1][j-1].flag1==0)///左上
            if(pos[i-1][j-1].bubble==b[q].bubble)
            {
                eliminate(i-1,j-1,b,q,pos);
            }
        if(i>0 && j<8 && pos[i-1][j].flag==1 &&pos[i-1][j].flag1==0)///右上
            if(pos[i-1][j].bubble==b[q].bubble)
            {
                eliminate(i-1,j,b,q,pos);
            }
        if(j>0 && pos[i][j-1].flag==1 &&pos[i][j-1].flag1==0)///左边
            if(pos[i][j-1].bubble==b[q].bubble)
            {
                eliminate(i,j-1,b,q,pos);
            }
        if(j<8 && pos[i][j+1].flag==1 &&pos[i][j+1].flag1==0)///右边
            if(pos[i][j+1].bubble==b[q].bubble)
            {
                eliminate(i,j+1,b,q,pos);
            }
        if(i<12 && j>0 && pos[i+1][j-1].flag==1 &&pos[i+1][j-1].flag1==0)///左下
            if(pos[i+1][j-1].bubble==b[q].bubble)
            {
                eliminate(i+1,j-1,b,q,pos);
            }
        if(i<12 && j<8 && pos[i+1][j].flag==1 &&pos[i+1][j].flag1==0)///右下
            if(pos[i+1][j].bubble==b[q].bubble)
            {
                eliminate(i+1,j,b,q,pos);
            }
    }
    else
    {
        if(pos[i-1][j].flag==1 &&pos[i-1][j].flag1==0)///左上
            if(pos[i-1][j].bubble==b[q].bubble)
            {
                eliminate(i-1,j,b,q,pos);
            }
        if(pos[i-1][j+1].flag==1 &&pos[i-1][j+1].flag1==0)///右上
            if(pos[i-1][j+1].bubble==b[q].bubble)
            {
                eliminate(i-1,j+1,b,q,pos);
            }
        if(j>0 && pos[i][j-1].flag==1 &&pos[i][j-1].flag1==0)///左边
            if(pos[i][j-1].bubble==b[q].bubble)
            {
                eliminate(i,j-1,b,q,pos);
            }
        if(j<7 && pos[i][j+1].flag==1 &&pos[i][j+1].flag1==0)///右边
            if(pos[i][j+1].bubble==b[q].bubble)
            {
                eliminate(i,j+1,b,q,pos);
            }
        if(pos[i+1][j].flag==1 &&pos[i+1][j].flag1==0)///左下
            if(pos[i+1][j].bubble==b[q].bubble)
            {
                eliminate(i+1,j,b,q,pos);
            }
        if(pos[i+1][j+1].flag==1 &&pos[i+1][j+1].flag1==0)///右下
            if(pos[i+1][j+1].bubble==b[q].bubble)
            {
                eliminate(i+1,j+1,b,q,pos);
            }
    }
}
void print_2()
{
    putimage(0,0,back1);
    putimage_transparent(NULL,wheel[p],n33,m33,BLACK);
    putimage_transparent(NULL,wheel[p],n3,m3,BLACK);
    score_print(0);
    score_print(1);
}

void GameOver(position pos[12][9])///游戏结束
{
    int  i,j,y1 = 0;
    print_2();
    char t;
    t = 'e';
    KO = 0;
    pthread_create(&ppid,NULL,play_music,&t);
    if(pos[0][0].x ==0)
    {
        score0 += 10;
        score1 -= 10;
        for(i = 0; i<20; i++)
        {
            print_2();
            putimage_transparent(NULL,lose,40,y1,BLACK);
            putimage_transparent(NULL,win,403,y1,BLACK);
            y1+=10;
            Sleep(50);
        }
    }
    else
    {
        score1 += 10;
        score0 -= 10;
        for(i = 0; i<20; i++)
        {
            print_2();
            putimage_transparent(NULL,lose,403,y1,BLACK);
            putimage_transparent(NULL,win,40,y1,BLACK);
            y1 += 10;
            Sleep(50);
        }
    }
    int Prompt;
    Prompt= MessageBox(NULL,"请问要再玩一盘吗？","提示",MB_YESNO);
    if(Prompt==6)
    {
        KO = 1;
        mainloop();
    }
    else
        exit(0);
}

void Location(int n, int m,int i, int j,int n4,position pos[12][9],emi b[],int q)///泡泡定位
{
    if(i%2==0)
    {
        if(n>=16 && m>=0 && pos[i-1][j-1].flag == 0 && n4 >=0)
        {
            pos[i-1][j-1].flag = 1;
            pos[i-1][j-1].bubble = b[q].bubble;
            eliminate(i-1,j-1,b,q,pos);
        }
        else if(n<=-16 &&m>=0 && pos[i-1][j].flag == 0 && n4<=0)
        {
            pos[i-1][j].flag = 1;
            pos[i-1][j].bubble = b[q].bubble;
            eliminate(i-1,j,b,q,pos);
        }
        else if(n>=16 &&j!=0&&pos[i][j-1].flag==0)
        {
            pos[i][j-1].flag = 1;
            pos[i][j-1].bubble = b[q].bubble;
            eliminate(i,j-1,b,q,pos);
        }
        else if(n<=-16 && pos[i][j+1].flag==0)
        {
            pos[i][j+1].flag = 1;
            pos[i][j+1].bubble = b[q].bubble;
            eliminate(i,j+1,b,q,pos);
        }
        else if((0<=n && pos[i+1][j-1].flag==0 &&j!=0) || (m<0 && j==8 && pos[i+1][j-1].flag==0))
        {
            pos[i+1][j-1].flag = 1;
            pos[i+1][j-1].bubble = b[q].bubble;
            if(i+1 >9) GameOver(pos);
            eliminate(i+1,j-1,b,q,pos);
        }
        else if((n<=0 && pos[i+1][j].flag ==0) || (m<0 &&j ==0 && pos[i+1][j].flag==0))
        {
            pos[i+1][j].flag = 1;
            pos[i+1][j].bubble = b[q].bubble;
            if(i+1>9)  GameOver(pos);
            eliminate(i+1,j,b,q,pos);
        }

    }
    if(i%2==1)
    {
        if(n>=16 && m>=0 && pos[i-1][j].flag == 0 && n4 >=0)
        {
            pos[i-1][j].flag = 1;
            pos[i-1][j].bubble = b[q].bubble;
            eliminate(i-1,j,b,q,pos);
        }
        else if(n<=-16 &&m>=0 && pos[i-1][j+1].flag == 0 && n4<=0)
        {
            pos[i-1][j+1].flag = 1;
            pos[i-1][j+1].bubble = b[q].bubble;
            eliminate(i-1,j+1,b,q,pos);
        }
        else if( n>=16 &&pos[i][j-1].flag==0 &&j!=0)
        {
            pos[i][j-1].flag = 1;
            pos[i][j-1].bubble = b[q].bubble;
            eliminate(i,j-1,b,q,pos);
        }
        else if(n<=-16 && pos[i][j+1].flag==0 &&j!=7)
        {
            pos[i][j+1].flag = 1;
            pos[i][j+1].bubble = b[q].bubble;
            eliminate(i,j+1,b,q,pos);
        }
        else if( 0<=n && pos[i+1][j].flag==0)
        {
            pos[i+1][j].flag = 1;
            pos[i+1][j].bubble = b[q].bubble;
            if(i+1>10) GameOver(pos);
            eliminate(i+1,j,b,q,pos);
        }
        else if(n<=0 && pos[i+1][j+1].flag ==0)
        {
            pos[i+1][j+1].flag = 1;
            pos[i+1][j+1].bubble = b[q].bubble;
            if(i+1>10) GameOver(pos);
            eliminate(i+1,j+1,b,q,pos);
        }
    }
}
void Location_add(int n, int m,int i, int j,position pos[12][9],emi b)///泡泡定位2
{
    if(i%2==0)
    {
        if(n<=-16 &&m>=0 && pos[i-1][j].flag == 0)
        {
            pos[i-1][j].flag = 1;
            pos[i-1][j].bubble = b.bubble;
        }
        else if(n>=16 &&j!=0&&pos[i][j-1].flag==0)
        {
            pos[i][j-1].flag = 1;
            pos[i][j-1].bubble = b.bubble;
        }
        else if(n<=-16 && pos[i][j+1].flag==0)
        {
            pos[i][j+1].flag = 1;
            pos[i][j+1].bubble = b.bubble;
        }
        else if((0<=n && pos[i+1][j-1].flag==0 &&j!=0) || (m<0 && j==8 && pos[i+1][j-1].flag==0))
        {
            pos[i+1][j-1].flag = 1;
            pos[i+1][j-1].bubble = b.bubble;
            if(i+1 >9) GameOver(pos);
        }
        else if((n<=0 && pos[i+1][j].flag ==0) || (m<0 &&j ==0 && pos[i+1][j].flag==0))
        {
            pos[i+1][j].flag = 1;
            pos[i+1][j].bubble = b.bubble;
            if(i+1>9)  GameOver(pos);
        }

    }
    if(i%2==1)
    {
        if(n<=-16 &&m>=0 && pos[i-1][j+1].flag == 0)
        {
            pos[i-1][j+1].flag = 1;
            pos[i-1][j+1].bubble = b.bubble;
        }
        else if( n>=16 &&pos[i][j-1].flag==0 &&j!=0)
        {
            pos[i][j-1].flag = 1;
            pos[i][j-1].bubble = b.bubble;
        }
        else if(n<=-16 && pos[i][j+1].flag==0 &&j!=7)
        {
            pos[i][j+1].flag = 1;
            pos[i][j+1].bubble = b.bubble;
        }
        else if( 0<=n && pos[i+1][j].flag==0)
        {
            pos[i+1][j].flag = 1;
            pos[i+1][j].bubble = b.bubble;
            if(i+1>10) GameOver(pos);
        }
        else if(n<=0 && pos[i+1][j+1].flag ==0)
        {
            pos[i+1][j+1].flag = 1;
            pos[i+1][j+1].bubble = b.bubble;
            if(i+1>10) GameOver(pos);
        }
    }
}


void Cover_Back1()///左边覆盖背景
{
    putimage(0,0,back2[1]);
    score_print(1);
    map_1(1);
    putimage_transparent(NULL,wheel[p1],n33,m33,BLACK);
    putimage_transparent(NULL,a[b1[q1].bubble],n11,m11,BLACK);
    putimage_transparent(NULL,a[b1[q1+1].bubble],n22,m22,BLACK);
}
void Cover_Back()///右边覆盖背景
{
    putimage(325,0,back2[0]);
    score_print(0);
    map_1(0);
    putimage_transparent(NULL,wheel[p],n3,m3,BLACK);
    putimage_transparent(NULL,a[b[q].bubble],n1,m1,BLACK);
    putimage_transparent(NULL,a[b[q+1].bubble],n2,m2,BLACK);
}

int AttachmentJudge(position pos[12][9],int i,int j)
{
   // printf("i = %d j = %d\n",i,j);
    if(i%2==0)
    {
        if(i!=0 && j!= 0 && pos[i-1][j-1].flag == 1 && pos[i-1][j-1].flag2 != 1)
        {
            pos[i-1][j-1].flag2 = 1;
            AttachmentJudge(pos,i-1,j-1);
        }
        if(i!=0 && j!= 8&& pos[i-1][j].flag == 1 && pos[i-1][j].flag2 != 1)
        {
            pos[i-1][j].flag2 = 1;
            AttachmentJudge(pos,i-1,j);
        }
        if(j!=0 && pos[i][j-1].flag==1 && pos[i][j-1].flag2 != 1)
        {
            pos[i][j-1].flag2 = 1;
            AttachmentJudge(pos,i,j-1);
        }
        if(j!=8 && pos[i][j+1].flag==1 && pos[i][j+1].flag2 != 1)
        {
            pos[i][j+1].flag2 = 1;
            AttachmentJudge(pos,i,j+1);
        }
        if(j!=0 && pos[i+1][j-1].flag==1 && pos[i+1][j-1].flag2 != 1)
        {
            pos[i+1][j-1].flag2 = 1;
            AttachmentJudge(pos,i+1,j-1);
        }
        if(j!=8 && pos[i+1][j].flag==1 && pos[i+1][j].flag2 != 1)
        {
            pos[i+1][j].flag2 = 1;
            AttachmentJudge(pos,i+1,j);
        }

    }
    if(i%2==1)
    {
        if(pos[i-1][j].flag == 1 && pos[i-1][j].flag2 != 1)
        {
            pos[i-1][j].flag2 = 1;
            AttachmentJudge(pos,i-1,j);
        }
        if(pos[i-1][j+1].flag == 1 && pos[i-1][j+1].flag2 != 1)
        {
            pos[i-1][j+1].flag2 = 1;
            AttachmentJudge(pos,i-1,j+1);
        }
        if(j!=0 &&pos[i][j-1].flag == 1 && pos[i][j-1].flag2 != 1)
        {
            pos[i][j-1].flag2 = 1;
            AttachmentJudge(pos,i,j-1);
        }
        if(j!=7 &&pos[i][j+1].flag == 1 && pos[i][j+1].flag2 != 1)
        {
            pos[i][j+1].flag2 = 1;
            AttachmentJudge(pos,i,j+1);
        }
        if(pos[i+1][j].flag == 1 && pos[i+1][j].flag2 != 1)
        {
            pos[i+1][j].flag2 = 1;
            AttachmentJudge(pos,i+1,j);
        }
        if(pos[i+1][j+1].flag == 1 && pos[i+1][j+1].flag2 != 1)
        {
            pos[i+1][j+1].flag2 = 1;
            AttachmentJudge(pos,i+1,j+1);
        }
    }
}
int Pop_drap(position pos[12][9])///泡泡附着及掉落判断
{
    int i,j;
    int l[102][2] = {0},t = 0;
    for(i = 0; i<11; i++)
        for(j = 0; j<9; j++)
        {
            if(pos[i][j].flag == 1 && pos[i][j].flag2==0)
            {
                printf("i  =%d j = %d\n",i,j);
                l[t][0] = i*10+j;
                l[t++][1] = pos[i][j].bubble;
                pos[i][j].flag = 0;
            }
        }
    if(t!=0)
    {
        if(pos[0][0].x==0)
            amount[1]+=t;
        else amount[0]+=t;
        char k = 'o';
        pthread_create(&ppid,NULL,play_music,&k);
    }
    for(i = 0; i<5; i++)///掉落实现
    {
        if(pos[0][0].x)
        {
            Cover_Back();
        }
        else if(!pos[0][0].x)
        {
            Cover_Back1();
        }
        for(j = 0; j<t; j++)
        {
            putimage_transparent(NULL,a[l[j][1]],pos[l[j][0]/10][l[j][0]%10].x,pos[l[j][0]/10][l[j][0]%10].y+i*50,BLACK);
        }
        Sleep(20);
    }
}

void JudgeMoreThree(position pos[12][9])///是否三个以上，泡泡爆炸
{
    int i,j;
    if(z<3)
    {
        z = 0;
        InitFlag1();
    }
    else
    {
        if(pos[0][0].x==0)
        {
            amount[1]+=(z-3);
        }
        else amount[0]+=(z-3);
        for(i = 0; i<z; i++)
        {
            pos[emil[i][0]][emil[i][1]].flag = 0;
        }
        for(i = 0; i<5; i++)
        {
            if(z==0)
            {
                Cover_Back();
            }
            else if(z==1)
            {
                Cover_Back1();
            }
            for(j = 0; j<z; j++)
            {
                putimage_transparent(NULL,baozha[i],pos[emil[j][0]][emil[j][1]].x,pos[emil[j][0]][emil[j][1]].y,BLACK);
            }
            Sleep(20);
        }
        InitFlag1();
        Init_flag2(pos);
        Pop_drap(pos);
       // printf("%d %d\n",amount[0],amount[1]);

        z = 0;
    }
}

int Judgement(int n4,position pos[12][9],emi b[],int p, int q)///判断是不是碰到泡泡
{
    b[q].x1 = b[q].x + 16;
    b[q].y1 = b[q].y + 16;
    int i,j,w,n,m;
    for(i = 11; i>=0; i--)
        for(j = 9; j>=0; j--)
        {
            n = (pos[i][j].x1-b[q].x1);///已知泡泡的横坐标-发射泡泡的横坐标
            m = (pos[i][j].y1-b[q].y1);///已知泡泡的纵坐标-发射泡泡的纵坐标
            w = n*n + m*m;
            if(pos[i][j].flag==1 && w<=2048)
            {
                InitFlag1();
                Location(n,m,i,j,n4,pos,b,q);///定位
                JudgeMoreThree(pos);///泡泡爆炸
                return 1;
            }
            if(i==0 &&w<=2048)
            {
                InitFlag1();
                pos[0][j].flag=1;
                pos[0][j].bubble=b[q].bubble;
                Cover_Back();
                eliminate(0,j,b,q,pos);
                JudgeMoreThree(pos);///泡泡爆炸
                return 1;
            }
        }
    return 0;
}
int Judgement_add(position pos[12][9],emi b)///判断是不是碰到泡泡
{
    b.x1 = b.x + 16;
    b.y1 = b.y + 16;
    int i,j,z,n,m;
    for(i = 11; i>=0; i--)
        for(j = 8; j>=0; j--)
        {
            n = (pos[i][j].x1-b.x1);///已知泡泡的横坐标-发射泡泡的横坐标
            m = (pos[i][j].y1-b.y1);///已知泡泡的纵坐标-发射泡泡的纵坐标
            z = n*n + m*m;
            if(pos[i][j].flag==1 && z<=2048)
            {
                Location_add(n,m,i,j,pos,b);///定位
                return 1;
            }
            if(i==0 &&z<=2048)
            {
                pos[0][j].flag=1;
                pos[0][j].bubble=b.bubble;
               // Cover_Back();
                return 1;
            }
        }
    return 0;
}

void Init_All()
{
    back1 = newimage();
    back2[0] = newimage();
    back2[1] = newimage();
    lose = newimage();
    win = newimage();
    getimage(back1,"-11.png");
    getimage(back2[0],back1,325,0,325,593);///右边
    getimage(back2[1],back1,0,0,325,593);///左边
    getimage(lose,"lose.png",100,75);
    getimage(win,"win.png",100,75);
    Init_SpecialPop();
    Init_drap_Pop();
    Init_Pop();
    Init_time();
    Init_Wheel();
    Init_bubble();
    Init_Emission();
    Init_Bmp();
    Lanuch_Angle();
    Cover_Back();
    Cover_Back1();
}

void close_mp3()
{
    mciSendString(TEXT("stop bk.mp3"), NULL, 0, NULL);
    mciSendString(TEXT("close BGM"), NULL, 0, NULL);
    mciSendString(TEXT("stop ready.mp3"), NULL, 0, NULL);
    mciSendString(TEXT("close BGM"), NULL, 0, NULL);
    mciSendString(TEXT("stop go.mp3"), NULL, 0, NULL);
    mciSendString(TEXT("close BGM"), NULL, 0, NULL);
}
void fashe_pop(position pos[12][9], emi b[], int p, int q,int v)
{
    int n4,m4,z1,z2;
    n4 = angle[p][0];
    m4 = angle[p][1];
    if(pos[0][0].x==0)
    {
        z1 = 0;
        z2 = 256;
    }
    else
    {
        z1 = 363;
        z2 = 618;
    }
    while(!Judgement(n4,pos,b,p,q))
    {
        if(b[q].x<=z1)
        {
            n4 *= -1;
            b[q].x = z1;
            putimage_transparent(NULL, a[b[q].bubble],b[q].x,b[q].y,BLACK);
        }
        if(b[q].x>=z2)
        {
            n4 *= -1;
            b[q].x = z2;
            putimage_transparent(NULL, a[b[q].bubble],b[q].x,b[q].y,BLACK);

        }
        b[q].x+=n4;
        b[q].y+=m4;
        putimage_transparent(NULL, a[b[q].bubble],b[q].x,b[q].y,BLACK);
        Sleep(20);
        Cover_Back();
    }
    q++;
}
void Add_seven_pop(position pos[12][9],int s)///增加7个泡泡
{
    Init_Add_b(s);

    int n = 0,i = 0,j,temp[7] = {0};

    while(n!=7)
    {
        for(i = 0; i<7; i++)
        {
            if(temp[i] == 0 && Judgement_add(pos,add_b[i]))
            {
                n++;
                temp[i] = 1;
            }
            else if(temp[i]==0 && (!Judgement_add(pos,add_b[i])))
            {
                add_b[i].y += add_angle[i][1];
                if((add_b[i].x>290 && add_b[i].x<363) || add_b[i].x<0)
                {
                    printf("add_b[0].x = %d",add_b[0].x);
                     add_b[i].x -= add_angle[i][0];
                }
                else add_b[i].x += add_angle[i][0];
            }
        }
        for(j = 0; j<7; j++)
        {
            putimage_transparent(NULL,a[add_b[j].bubble],add_b[j].x,add_b[j].y,BLACK);
        }
        Sleep(20);
        if(s==7)
            Cover_Back();
        else if(s==2)
            Cover_Back1();
    }
}
void Add_one_pop(position pos[12][9],int s)///增加1个泡泡
{
    srand(time(0));
    small_b.bubble = rand()%8;
    if(s==2)
    {
        small_b.x = 388+rand()%220;
        small_b.y = 513;
        amount[1]-=1;
    }
    else if(s==7)
    {
        small_b.x = 15+rand()%220;
        small_b.y = 513;
        amount[0]-=1;
    }
    while(!Judgement_add(pos,small_b))
    {
        small_b.y-=15;
        putimage_transparent(NULL,a[small_b.bubble],small_b.x,small_b.y,BLACK);
        Sleep(20);
        if(s==2)
        {
            Cover_Back1();
        }
        if(s==7)
        {
            Cover_Back();
        }
    }
}

void Bomp_Self_more7(position pos[12][9])///消除自己的7个泡泡
{
    int i,j,n = 0,bomp[7][2];
    for(i = 11; i>=0; i--)
    {
        for(j = 8; j>=0; j--)
        {
            if(pos[i][j].flag==1)
            {
                pos[i][j].flag = 0;
                bomp[n][0] = i;
                bomp[n++][1] = j;
            }
            if(n==7)
                break;
        }
        if(n==7)
            break;
    }

    for(i = 0; i<5; i++)
    {
        Cover_Back();
        for(j = 0; j<n; j++)
        {
            putimage_transparent(NULL,baozha[i],pos[bomp[j][0]][bomp[j][1]].x,pos[bomp[j][0]][bomp[j][1]].y,BLACK);
        }
        Sleep(30);
    }
    InitFlag1();
    Init_flag2(pos);
    Pop_drap(pos);
    Cover_Back();
}

int Bomp_Self_one(position pos[12][9])///消除自己1个泡泡
{
    srand(time(0));
    int i,j,o,flag = 0,n = 0,temp[9] = {0},z;
    for(i = 11; i>=0; i--)
    {
        for(j = 8; j>=0; j--)
            if(pos[i][j].flag==1)
            {
                flag = 1;
                break;
            }
        if(flag==1)
            break;
    }
    if(flag==0) return 0;
    for(j = 0; j<8; j++)
    {
        if(pos[i][j].flag==1)
        {
            temp[n++] = j;
        }
    }
    z = rand()%n;
    pos[i][z].flag = 0;
    for(o = 0; o<5; o++)
    {
        Cover_Back();
        pos[i][temp[z]].flag = 0;
        putimage_transparent(NULL,baozha[o],pos[i][temp[z]].x,pos[i][temp[z]].y,BLACK);
        Sleep(30);
    }
    InitFlag1();
    Init_flag2(pos);
    Pop_drap(pos);
    Cover_Back();
}

void mainloop()///游戏
{
    Init_All();///初始化全部值
//    time(&start_time);
//    time(&start_time_1);


    int i = 0;
    pthread_create(&uploop, NULL, up_loop, (void *)(i));

    int j = 0;
//    pthread_t keyloop;
    pthread_create(&keyloop, NULL, key_loop, (void *)(j));

    delay_ms(500);
    //for ( ; is_run(); delay_fps(60) )
    //     Cover_Back();
    while(1) {}; // 避免直接退出
}

void *up_loop(void *arg)
{
    for ( ; is_run(); delay_fps(60) )
    {
        char k;
        double i1;
        char t;
        if(GetAsyncKeyState(VK_UP) & 0X8000)///按上 发射
        {
            t = 'y';
            pthread_create(&ppid,NULL,play_music,&t);
            time(&start_time);
            int n4,m4;
            n4 = angle[p][0];
            m4 = angle[p][1];
            while(!Judgement(n4,pos,b,p,q))
            {
              //  printf("%d %d\n",b[q].x,b[q].y);
                if(b[q].x<=363)
                {
                    t = 'r';
                    pthread_create(&ppid,NULL,play_music,&t);
                    n4 *= -1;
                    b[q].x = 363;
                    putimage_transparent(NULL, a[b[q].bubble],b[q].x,b[q].y,BLACK);
                }
                else if(b[q].x>=618)
                {
                    t = 'r';
                    pthread_create(&ppid,NULL,play_music,&t);
                    n4 *= -1;
                    b[q].x = 618;
                    putimage_transparent(NULL, a[b[q].bubble],b[q].x,b[q].y,BLACK);

                }
                b[q].x+=n4;
                b[q].y+=m4;
                putimage_transparent(NULL, a[b[q].bubble],b[q].x,b[q].y,BLACK);
                Sleep(20);
                Cover_Back();
            }
            q++;
        }
        if(GetAsyncKeyState(VK_LEFT) & 0X8000)
        {
            p--;
            if(p<0) p = 32;
        }
        if(GetAsyncKeyState(VK_RIGHT) & 0X8000)
        {
            p++;
            if(p>32) p = 0;
        }
        while(GetAsyncKeyState('6') & 0X8000)
        {
            if(amount[0]!=0)
            {
                t = 'u';
                pthread_create(&ppid,NULL,play_music,&t);
                for(i1 = 0; i1<1.56; i1+=0.08)
                {
                    Cover_Back();
                    putimage_transparent(NULL,bmp[2],620-80*i1,481-180*sin(i1),BLACK);//蓝右
                    Sleep(50);
                }
                if(amount[0]>6)
                {
                    Bomp_Self_more7(pos);
                    amount[0]-=7;
                }
                else
                {
                    if(Bomp_Self_one(pos))
                    amount[0]-=1;
                }
            }
        }
        while(GetAsyncKeyState('7') & 0X8000)
        {
            if(amount[0]!=0)
            {
                t = 'i';
                pthread_create(&ppid,NULL,play_music,&t);
                for(i1 = 0; i1<1.56; i1+=0.08)
                {
                    Cover_Back();
                    putimage_transparent(NULL,bmp[1],620-300*i1,481-180*sin(i1),BLACK);//红右
                    Sleep(50);
                }
                if(amount[0]>6)
                {
                    Add_seven_pop(pos1, 7);
                    amount[0]-=7;
                }
                else
                {
                    Add_one_pop(pos1,7);
                }
            }
        }
        if(KO == 1)
        {
            Cover_Back();
        }
        else
        {
            pthread_exit(0);
        }
    }
}

void *key_loop(void *arg)
{
    for ( ; is_run(); delay_fps(60) )
    {
        char k;
        double i1;
        char t;
        while(GetAsyncKeyState('1') & 0X8000)
        {
            if(amount[1])
            {
                t = 'u';
                pthread_create(&ppid,NULL,play_music,&t);
                for(i1 = 0; i1<1.56; i1+=0.08)
                {
                    Cover_Back1();
                    putimage_transparent(NULL,bmp[2],258-80*i1,481-180*sin(i1),BLACK);//蓝左
                    Sleep(50);
                }
                if(amount[1]>6)
                {
                    Bomp_Self_more7(pos1);
                    amount[1]-=7;
                }
                else
                {
                    if(Bomp_Self_one(pos1))
                    amount[1]-=1;
                }
            }
        }
        while(GetAsyncKeyState('2') & 0X8000)
        {
            if(amount[1]!=0)
            {
                t = 'i';
                pthread_create(&ppid,NULL,play_music,&t);
                for(i1 = 0; i1<1.56; i1+=0.08)
                {
                    Cover_Back();
                    Cover_Back1();
                    putimage_transparent(NULL,bmp[0],258+160*i1,481-180*sin(i1),BLACK);//红左
                    Sleep(50);
                }
                if(amount[1]>6)
                {
                    Add_seven_pop(pos,2);
                    amount[1]-=7;
                }
                else
                {
                    Add_one_pop(pos,2);
                }
            }
        }
        if(GetAsyncKeyState('W') & 0X8000)
        {
            t = 'y';
            pthread_create(&ppid,NULL,play_music,&t);
            int n4,m4;
            n4 = angle[p1][0];
            m4 = angle[p1][1];
            while(!Judgement(n4,pos1,b1,p1,q1))
            {
                if(b1[q1].x<=0)
                {
                    t = 'r';
                    pthread_create(&ppid,NULL,play_music,&t);
                    n4 *= -1;
                    b1[q1].x = 0;
                    putimage_transparent(NULL, a[b1[q1].bubble],b1[q1].x,b1[q1].y,BLACK);
                }
                if(b1[q1].x>=256)
                {
                    t = 'r';
                    pthread_create(&ppid,NULL,play_music,&t);
                    n4 *= -1;
                    b1[q1].x = 256;
                    putimage_transparent(NULL, a[b1[q1].bubble],b1[q1].x,b1[q1].y,BLACK);

                }
                b1[q1].x+=n4;
                b1[q1].y+=m4;
                putimage_transparent(NULL, a[b1[q1].bubble],b1[q1].x,b1[q1].y,BLACK);
                Sleep(20);
                Cover_Back1();
            }
            q1++;
        }
        if(GetAsyncKeyState('A') & 0X8000)
        {
            p1--;
            if(p1<0) p1 = 32;
        }
        if(GetAsyncKeyState('D') & 0X8000)
        {
            p1++;
            if(p1>32) p1 = 0;
        }
        if(KO == 1)
        {
            Cover_Back1();
        }
        else
        {
            pthread_exit(0);
        }
    }
}

void *thr_fn(void *arg)///时间线程
{
    int n = 5;
    while(n)
    {
        Cover_Back();
        putimage_transparent(NULL,time1[n],400,370,BLACK);
        Sleep(1000);
        n--;
    }
    return NULL;
}

int main(void)
{
    // 各种初始化
    // setinitmode设置初始化模式：
    // INIT_DEFAULT ege默认配置
    // INIT_NOFORCEEXIT 点了右上角X的时候不强制退出程序，由程序本身检测和控制结束
    // INIT_RENDERMANUAL 与以下的setrendermode(RENDER_MANUAL)等效
    setinitmode(0);
    initgraph(650, 593);
    setcaption("Pop Dragon");
    setrendermode(RENDER_MANUAL);
    // 随机数初始化，如果需要使用随机数的话
    randomize();
    // 绘图更新模式设置为手动，仅调用delay_fps/delay_ms等函数时才刷新，避免闪烁
    // 区别是在RENDER_AUTO模式下，任何绘图操作进行时都可能刷新，甚至刚cleardevice就显示
    // 导致屏幕上有时仅显示一个背景而没有其它内容导致闪烁
    setrendermode(RENDER_MANUAL);
    // 程序主循环

    moun = newimage();
    explain =newimage();
    getimage(moun,"jiemian2.png");
    getimage(explain,"explain.png");
    setcolor(EGERGB(0xD2, 0x69, 0x1E));
    setfont(28, 0, "隶书");
    setbkmode(TRANSPARENT);
    play_BK_BGM();
    mouse_msg msg = {0};
    for ( ; is_run(); delay_fps(60))
    {
        putimage(0,0,moun);
        outtextxy(244,240,"开 始 游 戏");
        outtextxy(244,314,"游 戏 规 则");
        outtextxy(244,387,"退 出 游 戏");
        while (mousemsg())
        {
            msg = getmouse();
        }
        if((int)msg.is_left() && (int)msg.is_up())
        {
            if(msg.x>=234 &&msg.x<=401 && msg.y>=232 && msg.y<=274)
            {
                char t = 't';
                close_BK_BGM();
                pthread_create(&ppid,NULL,play_music,&t);
                play_BK1_BGM();
                mainloop();
            }
            if(msg.x>=234 &&msg.x<=401 && msg.y>=306 && msg.y<=347)
            {
                putimage_transparent(NULL,explain,190,90,BLACK);
                getch();
            }
            if(msg.x>=234 &&msg.x<=401 && msg.y>=379 && msg.y<=421)
                exit(0);
        }
    }
    closegraph();
    return 0;
}
