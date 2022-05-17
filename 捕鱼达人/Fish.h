#pragma once
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <graphics.h>
#include <easyx.h>
#include<iostream>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>
#include <string.h>
#include <math.h>
#include<mmsystem.h>
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib,"Winmm.lib")
using namespace std;

//宏定义

#define loginbackground_path ".\\Picture\\登录背景.png"
#define theme_path ".\\Picture\\捕鱼达人.png"
#define BossBackground_path ".\\Picture\\魔王城.jpg"
#define board1_path ".\\Picture\\Board1.png"
#define board2_path ".\\Picture\\Board2.png"
#define tryplaybutton1_path ".\\Picture\\试玩1.png"
#define tryplaybutton2_path ".\\Picture\\试玩2.png"
#define loginbutton_path ".\\Picture\\登录.png"
#define registerbutton_path ".\\Picture\\注册.png"
#define usersave_path ".\\Player.txt"
#define Ranklist_path ".\\RankList.txt"
#define background_path ".\\Picture\\背景.jpg"
#define Count3_path ".\\Picture\\Count3.png"
#define Count2_path ".\\Picture\\Count2.png"
#define Count1_path ".\\Picture\\Count1.png"
#define Go_path ".\\Picture\\Go.png"
#define pause_path ".\\Picture\\暂停.png"
#define continue_path ".\\Picture\\继续.png"
#define GameBoard_path ".\\Picture\\GameBoard.png"
#define shot1_path ".\\Picture\\炮台.png"
#define shot1t_path ".\\Picture\\炮台t.png"
#define Darkshot_path ".\\Picture\\暗影炮台t.jpg"
#define Darkshott_path ".\\Picture\\暗影炮台.jpg"
#define bullet1_path ".\\Picture\\bullet1.png"
#define bullet2_path ".\\Picture\\bullet2.png"
#define bullets1_path ".\\Picture\\superbullet1.png"
#define bullets2_path ".\\Picture\\superbullet2.png"
#define change_path ".\\Picture\\ChangeBullet.png"
#define bulletlight_path ".\\Picture\\bulletlight.png"
#define bulletlight1_path ".\\Picture\\bulletlight1.png"
#define bulletlight2_path ".\\Picture\\bulletlight2.png"
#define fish1_path ".\\Picture\\fish1.png"
#define fish2_path ".\\Picture\\fish2.png"
#define fish3_path ".\\Picture\\fish3.png"
#define fish4_path ".\\Picture\\fish4.png"
#define fish5_path ".\\Picture\\fish5.png"
#define fish6_path ".\\Picture\\fish6.png"
#define Rewardfish1_path ".\\Picture\\Rewardfish1.png"
#define Rewardfish2_path ".\\Picture\\Rewardfish2.png"
#define Rewardfish3_path ".\\Picture\\Rewardfish3.png"
#define Rewardfish4_path ".\\Picture\\Rewardfish4.png"
#define Rewardfish5_path ".\\Picture\\Rewardfish5.png"
#define Rewardfish6_path ".\\Picture\\Rewardfish6.png"
#define Ghostfish_path ".\\Picture\\幽灵鱼.png"
#define boss1_path ".\\Picture\\Boss1.png"
#define boss2_path ".\\Picture\\Boss2.png"
#define Bosslight1_path ".\\Picture\\Bosslight1.jpg"
#define Bosslight2_path ".\\Picture\\Bosslight2.jpg"
#define ReplayButton_path ".\\Picture\\ReplayButton.png"
#define Hinder_path ".\\Picture\\Hinder.png"
#define wildbutton1_path ".\\Picture\\狂暴1.jpg"
#define wildbutton2_path ".\\Picture\\狂暴2.jpg"

constexpr auto background_width = 1000;
constexpr auto background_height = 800;
constexpr auto fish_type = 7;
constexpr auto fish_length = 80;
constexpr auto fish_width = 60;
constexpr auto fish_move = 3;
constexpr auto fish_number = 100;
constexpr auto PI = 3.1415926535;
constexpr auto Shot_x = 500;
constexpr auto Shot_y = 730;
constexpr auto bullet_move = 6;
constexpr auto bullet_number = 30;
constexpr auto PlayerTime = 120;
constexpr auto BossMove = 1;


//结构体
typedef struct player {
	bool Tryplay = false;
	bool pause = false;
	char username[15];
	char password[15];
	int Score = 0;
	int Caughtfish = 0;
	double Wildtime = 0;
	double time;
	bool GameOver = false;
	bool Changebullet = false;
	bool Replay = true;
	bool Firstwild = true;
	bool Wild = false;
}Player;
typedef struct Shot {
	IMAGE pic1;
	IMAGE pic2;
	IMAGE darkshot1;
	IMAGE darkshot2;
	IMAGE pic1r;
	IMAGE pic2r;
	IMAGE darkshot1r;
	IMAGE darkshot2r;
	double degree;
}shot;
typedef struct Bullet {
	double x;
	double y;
	double degree;
	int cost;
	IMAGE pic1;
	IMAGE pic2;
	IMAGE pic1r;
	IMAGE pic2r;
	IMAGE pics1;
	IMAGE pics2;
	IMAGE pics1r;
	IMAGE pics2r;
	IMAGE bulletlight;
	IMAGE bulletlight1;
	IMAGE bulletlight2;
	int type;
	bool exist;
	bool toDelete = false;
	Bullet* next;
}bullet;
typedef struct fish {
	int x;
	int y;
	int rand;
	int score;
	int count = 0;
	int appear = 0;
	IMAGE pic;
	bool exist;
	bool toDelete = false;
	fish* next;
}Fish;
typedef struct bossfish {
	int x;
	int y;
	int HP;
	int score;
	int rand;
	int count;
	bool come;
	bool dead;
	IMAGE light1, light2;
	IMAGE pic1, pic2;
}Boss;
typedef struct hinder {
	int x;
	int y;
	int HP;
	IMAGE pic;
	hinder* next;
}Hinder;

Player player;
time_t pausestart;
time_t wildstart;
double pausepast = 0;
///////////////////////////////////////////


//函数声明
void StartGame();
void InputFlush(shot* p, bullet** head1);
void NULLInputFlush(Fish** _fish, bullet** _bullet, time_t begin, Boss* boss, Hinder* hinderhead);


//菜单相关
void Putmenu();
void Loadbackground();
void Register();
int choice(MOUSEMSG m);
char Getinput();
void Write(char  username[], char  password[]);
bool check(char input1[20], char input2[20], FILE* fp);


//绘图
void DrawEv(Fish* head, shot* s, bullet* head1, IMAGE pic, IMAGE GameBoard, IMAGE pause, IMAGE continuebutton, Boss* boss, IMAGE ChangeBulletbutton, Hinder* hinderhead);
void Print(Fish* head, shot* s, bullet* bullethead, Boss* boss, Hinder* hinderhead);
void DrawRankinglist(IMAGE pic, IMAGE GameBoard);


//发射
shot* ShotInitialize();
bullet Initialize(shot* s);
void Addbullet(bullet** head, shot* f);
void Addbullets(bullet** head, shot* f);
bullet* _deleteBullet(bullet* head);


//鱼类
void Fishcheck(Fish* p);
fish* _deleteFish(fish* head);
void _insertFish(Fish** _head);


//工具
void Loadfont(LOGFONT* font, COLORREF color, int height, int degree);
void putTransparentImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
void PutImage(int x, int y, IMAGE* mask, IMAGE* img);
void HpSleep(int ms);
double Countdown(time_t begin);

//创新关卡
Boss* Bossinitialize();
void BossdeadFish(Fish** head, Boss* boss);

//Hinder障碍
Hinder* Hinderinitialize();
void InsertHinder(Hinder** Hinderhead);