#define _CRT_SECURE_NO_WARNINGS
#include"Fish.h"

int main() {
	initgraph(background_width, background_height);
	Putmenu();
	while (player.Replay) {
		player.time = 120;
		player.Replay = false;
		player.Caughtfish = 0;
		player.GameOver = false;
		player.Score = 0;
		player.Changebullet = false;
		player.Firstwild = true;
		player.Wild = false;
		StartGame();
		while (true) {
			while (MouseHit) {
				MOUSEMSG m;
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN) {
					m = GetMouseMsg();
					if (m.x > 475 && m.x < 525 && m.y>600 && m.y < 650) {
						player.Replay = true;
						mciSendString(TEXT("close Mmusic"), NULL, 0, NULL);
						break;
					}
				}
			}
			if (player.Replay)	break;
		}
	}
	return 0;
}

void StartGame() {
	mciSendString(TEXT("close Smusic"), NULL, 0, NULL);
	mciSendString(TEXT("open \".\\Audio\\PlayBGM.mp3\" alias Amusic"), NULL, 0, NULL);
	mciSendString(TEXT("play Amusic repeat"), NULL, 0, NULL);        //循环播放音乐
	srand((unsigned)time(NULL));
	time_t begin, end, start;
	IMAGE  pic, GameBoard, Count3, Count2, Count1, Go, pause, continuebutton, ChangeBulletbutton;
	shot* s = ShotInitialize();
	Fish* fishhead = NULL;
	bullet* bullethead = &Initialize(s);
	Boss* boss = Bossinitialize();
	Hinder* Hinderhead = Hinderinitialize();
	for (int i = 0;i < rand() % 4 + 4;i++) {//设置障碍Hinder
		InsertHinder(&Hinderhead);
	}
	loadimage(&pic, _T(background_path), background_width, background_height, 1);
	loadimage(&GameBoard, _T(GameBoard_path), 1000, 120, 1);
	double past;
	loadimage(&continuebutton, _T(continue_path), 40, 40);
	loadimage(&pause, _T(pause_path), 40, 40);
	loadimage(&Count3, _T(Count3_path), 100, 200);
	loadimage(&Count2, _T(Count2_path), 100, 200);
	loadimage(&Count1, _T(Count1_path), 100, 200);
	loadimage(&Go, _T(Go_path), 450, 300);
	loadimage(&ChangeBulletbutton, _T(change_path), 50, 50);
	start = time(NULL);
	while (true) {
		past = Countdown(start);
		if (past == 1) {
			BeginBatchDraw();
			cleardevice();
			putimage(0, 0, &pic);
			putTransparentImage(NULL, 0, 730, &GameBoard);
			putTransparentImage(NULL, 450, 300, &Count3);
			FlushBatchDraw();
		}
		if (past == 2) {
			BeginBatchDraw();
			cleardevice();
			putimage(0, 0, &pic);
			putTransparentImage(NULL, 0, 730, &GameBoard);
			putTransparentImage(NULL, 450, 300, &Count2);
			FlushBatchDraw();
		}
		if (past == 3) {
			BeginBatchDraw();
			cleardevice();
			putimage(0, 0, &pic);
			putTransparentImage(NULL, 0, 730, &GameBoard);
			putTransparentImage(NULL, 450, 300, &Count1);
			FlushBatchDraw();
		}
		if (past == 4 || past == 5) {
			BeginBatchDraw();
			cleardevice();
			putimage(0, 0, &pic);
			putTransparentImage(NULL, 0, 730, &GameBoard);
			putTransparentImage(NULL, 300, 290, &Go);
			FlushBatchDraw();
			if (past == 5)	break;
		}
	}

	/////////////////////////////////////////////////////////游戏内容开始///////////////////////////////////////////////////////////////////////////

	begin = time(NULL);
	while (true) {						//主体while
		DrawEv(fishhead, s, bullethead, pic, GameBoard, pause, continuebutton, boss, ChangeBulletbutton, Hinderhead);  //显示(fish, gun, bullet, img)
		InputFlush(s, &bullethead);//输入更新(gun, bullet, player, fish)
		if (!player.pause) NULLInputFlush(&fishhead, &bullethead, begin, boss, Hinderhead);  //自动更新(fish, bullet)
		if (player.GameOver) { break; }
		HpSleep(30);
	}
	mciSendString(TEXT("close Amusic"), NULL, 0, NULL);
	mciSendString(TEXT("close Bmusic"), NULL, 0, NULL);
	mciSendString(TEXT("open \".\\Audio\\EndBGM.mp3\" alias Mmusic"), NULL, 0, NULL);
	mciSendString(TEXT("play Mmusic repeat"), NULL, 0, NULL);
	DrawRankinglist(pic, GameBoard);
}


//———————————————————————————————输入更新——————————————————————————————————//
/////////////////////////////////////////////////////////////////////////////
void InputFlush(shot* p, bullet** bullethead) {
	//炮台旋转
	char key = 0;
	MOUSEMSG m;
	while (MouseHit()) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && !player.pause && !player.Changebullet) {
			m = GetMouseMsg();
			if (m.x > 880 && m.x < 920 && m.y>0 && m.y < 40 && !player.Wild &&!player.pause) {
				if (player.Firstwild) {
					player.Wild = true;
					wildstart = time(NULL);
					player.Firstwild = false;
					mciSendString(TEXT("stop Amusic"), NULL, 0, NULL);
					mciSendString(TEXT("open \".\\Audio\\狂暴.mp3\" alias Kmusic"), NULL, 0, NULL);
					mciSendString(TEXT("play Kmusic repeat"), NULL, 0, NULL);
				}
			}
			else if (m.x > 573 && m.x < 618 && m.y>730 && m.y < 770) {			//切换子弹
				mciSendString(TEXT("close Lmusic"), NULL, 0, NULL);
				mciSendString(TEXT("open \".\\Audio\\Change.mp3\" alias Lmusic"), NULL, 0, NULL);
				mciSendString(TEXT("play Lmusic"), NULL, 0, NULL);
				if (player.Changebullet) player.Changebullet = false;
				else player.Changebullet = true;
			}
			else if (m.x > 960 && m.x < 1000 && m.y>0 && m.y < 40) {			//游戏暂停
				pausepast = 0;
				player.pause = true;
				pausestart = time(NULL);
			}
			else {
				Addbullet(bullethead, p);
			}
		}
		else if (m.uMsg == WM_LBUTTONDOWN && !player.pause && player.Changebullet) {//切换子弹发射
			m = GetMouseMsg();
			if (m.x > 880 && m.x < 920 && m.y>0 && m.y < 40 && !player.Wild) {//开启狂暴模式
				if (player.Firstwild) {
					player.Wild = true;
					wildstart = time(NULL);
					player.Firstwild = false;
					mciSendString(TEXT("stop Amusic"), NULL, 0, NULL);
					mciSendString(TEXT("open \".\\Audio\\狂暴.mp3\" alias Kmusic"), NULL, 0, NULL);
					mciSendString(TEXT("play Kmusic repeat"), NULL, 0, NULL);
				}
			}
			else if (m.x > 573 && m.x < 618 && m.y>730 && m.y < 770) {			//切换子弹
				mciSendString(TEXT("close Lmusic"), NULL, 0, NULL);
				mciSendString(TEXT("open \".\\Audio\\Change.mp3\" alias Lmusic"), NULL, 0, NULL);
				mciSendString(TEXT("play Lmusic"), NULL, 0, NULL);
				if (player.Changebullet) player.Changebullet = false;
				else player.Changebullet = true;
			}
			else if (m.x > 960 && m.x < 1000 && m.y>0 && m.y < 40) {			//游戏暂停
				pausepast = 0;
				player.pause = true;
				pausestart = time(NULL);
			}
			else{
				Addbullets(bullethead, p);
			}
		}
		else if (m.uMsg == WM_LBUTTONDOWN && player.pause) {				//继续游戏
			m = GetMouseMsg();
			if (m.x > 960 && m.x < 1000 && m.y>0 && m.y < 40) {
				player.pause = false;
				pausepast = Countdown(pausestart);
			}
		}
		p->degree = atan2((Shot_x - m.x), (Shot_y - m.y));
		if (!player.Changebullet) {
			rotateimage(&p->pic1r, &p->pic1, p->degree, WHITE, true);
			rotateimage(&p->pic2r, &p->pic2, p->degree, BLACK, true);
		}
		else {
			rotateimage(&p->darkshot1r, &p->darkshot1, p->degree, WHITE, true);
			rotateimage(&p->darkshot2r, &p->darkshot2, p->degree, BLACK, true);
		}
	}
	if (_kbhit() != 0) {				//键盘操作
		while (_kbhit() != 0) { key = _getch(); }
		switch (key) {
		case 75://左键
			p->degree += (15.0 / 180.0) * PI;
			if (!player.Changebullet) {
				rotateimage(&p->pic1r, &p->pic1, p->degree, WHITE, true);
				rotateimage(&p->pic2r, &p->pic2, p->degree, BLACK, true);
			}
			else {
				rotateimage(&p->darkshot1r, &p->darkshot1, p->degree, WHITE, true);
				rotateimage(&p->darkshot2r, &p->darkshot2, p->degree, BLACK, true);
			}
			break;
		case 77://右键
			p->degree -= (15.0 / 180.0) * PI;
			if (!player.Changebullet) {
				rotateimage(&p->pic1r, &p->pic1, p->degree, WHITE, true);
				rotateimage(&p->pic2r, &p->pic2, p->degree, BLACK, true);
			}
			else {
				rotateimage(&p->darkshot1r, &p->darkshot1, p->degree, WHITE, true);
				rotateimage(&p->darkshot2r, &p->darkshot2, p->degree, BLACK, true);
			}
			break;
		case 72://上键
			if (!player.pause && !player.Changebullet)	Addbullet(bullethead, p);
			else if (!player.pause && player.Changebullet) Addbullets(bullethead, p);
			break;
		}
	}
}
//炮台初始化
shot* ShotInitialize() {
	shot* p = new shot;
	loadimage(&p->pic1, _T(shot1_path), 60, 90);
	loadimage(&p->pic2, _T(shot1t_path), 60, 90);
	loadimage(&p->darkshot1, _T(Darkshot_path), 90, 60);
	loadimage(&p->darkshot2, _T(Darkshott_path), 90, 60);
	return p;
}
//子弹初始化
bullet Initialize(shot* f) {					//子弹初始化
	bullet* p = new bullet;
	p->cost = -1;
	p->x = Shot_x;
	p->y = Shot_y;
	p->degree = f->degree;
	p->exist = false;
	p->type = 0;
	loadimage(&p->pic1, _T(bullet1_path), 45, 60);
	loadimage(&p->pic2, _T(bullet2_path), 45, 60);
	loadimage(&p->pics1, _T(bullets1_path), 55, 55);
	loadimage(&p->pics2, _T(bullets2_path), 55, 55);
	p->next = NULL;
	return *p;
}
//添加子弹节点
void Addbullet(bullet** head, shot* f) {
	bullet* _newNode = new bullet;
	_newNode->degree = f->degree;
	loadimage(&_newNode->pic1, _T(bullet1_path), 45, 60);
	loadimage(&_newNode->pic2, _T(bullet2_path), 45, 60);
	loadimage(&_newNode->bulletlight1, _T(bulletlight_path), 120, 120);
	loadimage(&_newNode->bulletlight1, _T(bulletlight1_path), 120, 120);
	loadimage(&_newNode->bulletlight2, _T(bulletlight2_path), 120, 120);
	loadimage(&_newNode->pics1, _T(bullets1_path), 55, 55);
	loadimage(&_newNode->pics2, _T(bullets2_path), 55, 55);
	rotateimage(&_newNode->pic1r, &_newNode->pic1, _newNode->degree, WHITE, true);
	rotateimage(&_newNode->pic2r, &_newNode->pic2, _newNode->degree, BLACK, true);
	rotateimage(&_newNode->pics1r, &_newNode->pics1, _newNode->degree, WHITE, true);
	rotateimage(&_newNode->pics2r, &_newNode->pics2, _newNode->degree, BLACK, true);
	_newNode->x = Shot_x + 5 - (_newNode->pic1r).getwidth() / 2.0;
	_newNode->y = Shot_y - (_newNode->pic2r).getheight() / 2.0;
	_newNode->exist = true;
	_newNode->cost = -1;
	_newNode->type = 0;
	_newNode->next = *head;
	*head = _newNode;
	_newNode = NULL;

	return;
}
void Addbullets(bullet** head, shot* f) {
	bullet* _newNode = new bullet;
	_newNode->degree = f->degree;
	loadimage(&_newNode->pic1, _T(bullet1_path), 45, 60);
	loadimage(&_newNode->pic2, _T(bullet2_path), 45, 60);
	loadimage(&_newNode->bulletlight1, _T(bulletlight_path), 120, 120);
	loadimage(&_newNode->bulletlight1, _T(bulletlight1_path), 120, 120);
	loadimage(&_newNode->bulletlight2, _T(bulletlight2_path), 120, 120);
	loadimage(&_newNode->pics1, _T(bullets1_path), 50, 50);
	loadimage(&_newNode->pics2, _T(bullets2_path), 50, 50);
	rotateimage(&_newNode->pic1r, &_newNode->pic1, _newNode->degree, WHITE, true);
	rotateimage(&_newNode->pic2r, &_newNode->pic2, _newNode->degree, BLACK, true);
	rotateimage(&_newNode->pics1r, &_newNode->pics1, _newNode->degree, WHITE, true);
	rotateimage(&_newNode->pics2r, &_newNode->pics2, _newNode->degree, BLACK, true);
	_newNode->x = Shot_x + 5 - (_newNode->pic1r).getwidth() / 2.0;
	_newNode->y = Shot_y - (_newNode->pic2r).getheight() / 2.0;
	_newNode->exist = true;
	_newNode->cost = -1;
	_newNode->type = 1;
	_newNode->next = *head;
	*head = _newNode;
	_newNode = NULL;

	return;
}
//—————————————————————————————————————————————————————————————————————//


//———————————————————————————————自动更新——————————————————————————————————//
/////////////////////////////////////////////////////////////////////////////
void NULLInputFlush(Fish** _fish, bullet** _bullet, time_t begin, Boss* boss, Hinder* hinderhead) {
	//初始化十条鱼
	Fish* s;
	int i = 0;
	if (*_fish == NULL) {
		for (i = 0; i < fish_number; i++) {
			_insertFish(_fish);
		}
	}
	//更新鱼链表
	Fish* p = *_fish;
	if (!p->exist) {
		if(!player.Wild)	p->x += fish_move * 3;
		else p->x += fish_move * 20;		//狂暴模式
		if (p->x > rand() % 300 + 300) {
			p->exist = true;
		}
	}
	else {
		i = 0;
		for (p = *_fish; p != NULL; p = p->next) {
			i++;
			if (p->exist) {
				if ((p->x > 850) || (p->x < 20) || (p->y < 10) || (p->y > 700)) {
					Fishcheck(p);
				}
				else {
					if (p->count >= rand() % 5 + 10) {
						p->rand = rand() % 8;
						p->count = 0;
					}
					if (p->rand == 0) {
						p->x += fish_move;
						p->y += fish_move;
						p->count++;
					}
					else if (p->rand == 1) {
						p->x -= fish_move;
						p->y += fish_move;
						p->count++;
					}
					else if (p->rand == 2) {
						p->x += fish_move;
						p->y -= fish_move;
						p->count++;
					}
					else if (p->rand == 3) {
						p->x -= fish_move;
						p->y -= fish_move;
						p->count++;
					}
					else if (p->rand == 4) {
						p->x += fish_move;
						p->count++;
					}
					else if (p->rand == 5) {
						p->x -= fish_move;
						p->count++;
					}
					else if (p->rand == 6) {
						p->y += fish_move;
						p->count++;
					}
					else if (p->rand == 7) {
						p->y -= fish_move;
						p->count++;
					}
				}
				if (p->next != NULL) {
					if (!p->next->exist) { //如果上一条鱼存在且下一条鱼不存在让下一条鱼开始走
						if(!player.Wild)	p->next->x += fish_move * 3;
						else p->next->x += fish_move * 20;
						if (p->next->x > rand() % 300 + 300) {
							p->next->exist = true;
						}
					}
				}
			}
		}
		if (i < fish_number) {
			Fish* s = *_fish;
			Fish* _newNode = new Fish;
			int r, y;
			r = rand() % fish_type;
			y = rand() % 450 + 200;
			_newNode->exist = false;
			_newNode->y = y;
			_newNode->rand = rand() % 8;
			if (r == 0) {
				loadimage(&_newNode->pic, _T(fish1_path), fish_length, fish_width);
				_newNode->x = -90;
				_newNode->score = 10;
			}
			else if (r == 1) {
				loadimage(&_newNode->pic, _T(fish2_path), fish_length, fish_width);
				_newNode->x = -90;
				_newNode->score = 5;
			}
			else if (r == 2) {
				loadimage(&_newNode->pic, _T(fish3_path), fish_length, fish_width);
				_newNode->x = -90;
				_newNode->score = 15;
			}
			else if (r == 3) {
				loadimage(&_newNode->pic, _T(fish4_path), fish_length + 10, fish_width);
				_newNode->x = -100;
				_newNode->score = 40;
			}
			else if (r == 4) {
				loadimage(&_newNode->pic, _T(fish5_path), fish_length + 60, fish_width + 40);
				_newNode->x = -100;
				_newNode->score = 30;
			}
			else if (r == 5) {
				loadimage(&_newNode->pic, _T(fish6_path), fish_length + 60, fish_width);
				_newNode->x = -140;
				_newNode->score = 50;
			}
			else if (r == 6) {//幽灵鱼
				loadimage(&_newNode->pic, _T(Ghostfish_path), fish_length - 30, fish_width);
				_newNode->x = -140;
				_newNode->score = 60;
			}
			for (;s->next != NULL;s = s->next) {}
			s->next = _newNode;
			_newNode->next = NULL;
		}
	}
	//批量删除
	*_bullet = _deleteBullet(*_bullet);
	*_fish = _deleteFish(*_fish);
	double bullet_x = 0, bullet_y = 0;
	//更新炮弹链表(顺便判断张网)
	for (bullet* p = *_bullet; p != NULL; p = p->next) {
		if (p->exist) {
			p->x -= bullet_move * sin(p->degree);
			p->y -= bullet_move * cos(p->degree);
		}
		bullet_x = p->x + p->pic1r.getwidth() * 2 / 5 - p->pic1.getheight() * sin(p->degree) / 2;//计算子弹判定位置
		bullet_y = p->y + p->pic1r.getheight() / 2 - p->pic1.getheight() * cos(p->degree) / 2;
		if (p->x < 0) {
			p->toDelete = true;
		}
		for (Fish* f = *_fish; f != NULL; f = f->next) {
			///判定鱼
			double a = f->pic.getwidth() / 2; //椭圆判定区的半长轴
			double b = f->pic.getheight() / 4;//同
			double x = f->x + f->pic.getwidth() / 2.0; //椭圆中心横坐标
			double y = f->y + f->pic.getheight() / 2.0;//同
			bool insideEllipse = pow(((bullet_x - x) / a), 2) + pow(((bullet_y - y) / b), 2) < 1;//判断炮弹是否在鱼椭圆内
			if (insideEllipse && f->score != 60 && !p->type) {
				f->toDelete = true;
				p->toDelete = true;
				player.Score += f->score;
				player.Caughtfish++;
			}
			else if (insideEllipse && f->score == 60 && p->type) {	//判断是否是幽灵鱼
				f->toDelete = true;
				p->toDelete = true;
				player.Score += f->score;
				player.Caughtfish++;
			}
			double m = boss->x + boss->pic1.getwidth() / 2.0; //圆中心横坐标
			double n = boss->y + boss->pic1.getheight() / 2.0;//同
			bool insideBoss = pow((f->x - m), 2) + pow((f->y - n), 2) <= (14400);//判断炮弹是否在Boss椭圆内
			if (insideBoss && boss->come && !boss->dead) {		//boss会吃鱼
				f->toDelete = true;
				boss->HP += 3;				//吃鱼回血
			}
		}

		//改变Boss位置并判定boss与子弹
		if (boss->come && !boss->dead) {
			if ((boss->x > 800) || (boss->x < 100) || (boss->y < 200) || (boss->y > 600)) {
				if (boss->x <= 100) {
					boss->rand = 4;
					boss->x += BossMove;
				}
				else if (boss->x >= 750) {
					boss->rand = 5;
					boss->x -= BossMove;
				}
				else if (boss->y <= 200) {
					boss->rand = 6;
					boss->y += BossMove;
				}
				else if (boss->y >= 600) {
					boss->rand = 7;
					boss->y -= BossMove;
				}
			}
			else {
				if (boss->count >= rand() % 5 + 8) {
					boss->rand = rand() % 8;
					boss->count = 0;
				}
				if (boss->rand == 0) {
					boss->x += BossMove;
					boss->y += BossMove;
					boss->count++;
				}
				else if (boss->rand == 1) {
					boss->x -= BossMove;
					boss->y += BossMove;
					boss->count++;
				}
				else if (boss->rand == 2) {
					boss->x += BossMove;
					boss->y -= BossMove;
					boss->count++;
				}
				else if (boss->rand == 3) {
					boss->x -= BossMove;
					boss->y -= BossMove;
					boss->count++;
				}
				else if (boss->rand == 4) {
					boss->x += BossMove;
					boss->count++;
				}
				else if (boss->rand == 5) {
					boss->x -= BossMove;
					boss->count++;
				}
				else if (boss->rand == 6) {
					boss->y += BossMove;
					boss->count++;
				}
				else if (boss->rand == 7) {
					boss->y -= BossMove;
					boss->count++;
				}
			}
			double x = boss->x + boss->pic1.getwidth() / 2.0; //圆中心横坐标
			double y = boss->y + boss->pic1.getheight() / 2.0;//同
			bool insideCircle = pow((bullet_x - x), 2) + pow((bullet_y - y), 2) < (115 * 115);//判断炮弹是否在Boss圆内
			if (insideCircle) {
				p->toDelete = true;
				if (!p->type)	boss->HP--;
				else if (p->type)	boss->HP -= 3;
			}
		}
		for (Hinder* h = hinderhead;h != NULL;h = h->next) {
			if (h->HP > 0) {
				double u = h->x + h->pic.getwidth() / 2;
				double v = h->y + h->pic.getheight() / 2;
				bool insideCircle = pow((bullet_x - u), 2) + pow((bullet_y - v), 2) < (30 * 30);
				if (insideCircle) {
					if (!p->type) {
						p->toDelete = true;
						h->HP--;
					}
				}
			}
		}
	}
	player.time = PlayerTime - Countdown(begin) + pausepast;
	if (boss->HP <= 0 && !boss->dead) {
		boss->dead = true;
		player.Score += boss->score;
		for (int i = 0;i < rand() % 10 + 5;i++) {
			BossdeadFish(_fish, boss);
		}
		mciSendString(TEXT("close Bmusic"), NULL, 0, NULL);
		mciSendString(TEXT("play Amusic repeat"), NULL, 0, NULL);
	}
	if (player.time <= 0) {
		player.GameOver = true;
		mciSendString(TEXT("close Amusic"), NULL, 0, NULL);
		mciSendString(TEXT("open \".\\Audio\\GameOver.mp3\" alias Hmusic"), NULL, 0, NULL);	//GameOverBGM
		mciSendString(TEXT("play Hmusic"), NULL, 0, NULL);
		HpSleep(3000);
	}
	if (player.time == 60) {
		boss->come = true;
		mciSendString(TEXT("stop Amusic"), NULL, 0, NULL);
		mciSendString(TEXT("open \".\\Audio\\BossCome.mp3\" alias Bmusic"), NULL, 0, NULL);	//Boss登场BGM
		mciSendString(TEXT("play Bmusic repeat"), NULL, 0, NULL);
	}
	if (Countdown(wildstart) == 15) {
		player.Wild = false;
		mciSendString(TEXT("close Kmusic"), NULL, 0, NULL);
		mciSendString(TEXT("play Amusic repeat"), NULL, 0, NULL); 
	}
	return;
}

void _insertFish(Fish** _head) {
	Fish* _newNode = new Fish;
	int r, y;
	r = rand() % fish_type;
	y = rand() % 450 + 200;
	_newNode->exist = false;
	_newNode->y = y;
	_newNode->rand = rand() % 8;
	if (r == 0) {
		loadimage(&_newNode->pic, _T(fish1_path), fish_length, fish_width);
		_newNode->x = -90;
		_newNode->score = 10;
	}
	else if (r == 1) {
		loadimage(&_newNode->pic, _T(fish2_path), fish_length, fish_width);
		_newNode->x = -90;
		_newNode->score = 5;
	}
	else if (r == 2) {
		loadimage(&_newNode->pic, _T(fish3_path), fish_length, fish_width);
		_newNode->x = -90;
		_newNode->score = 15;
	}
	else if (r == 3) {
		loadimage(&_newNode->pic, _T(fish4_path), fish_length + 10, fish_width);
		_newNode->x = -100;
		_newNode->score = 40;
	}
	else if (r == 4) {
		loadimage(&_newNode->pic, _T(fish5_path), fish_length + 60, fish_width + 40);
		_newNode->x = -110;
		_newNode->score = 30;
	}
	else if (r == 5) {
		loadimage(&_newNode->pic, _T(fish6_path), fish_length + 60, fish_width);
		_newNode->x = -140;
		_newNode->score = 50;
	}
	else if (r == 6) {//幽灵鱼
		loadimage(&_newNode->pic, _T(Ghostfish_path), fish_length - 30, fish_width);
		_newNode->x = -90;
		_newNode->score = 60;
	}
	_newNode->next = *_head;
	*_head = _newNode;
	_newNode = NULL;
	return;
}
fish* _deleteFish(fish* head) {
	////////////////////////////////////////////////////////
	fish* cur = head;
	fish* pre = NULL;
	while (true) {

		cur = head;
		pre = NULL;

		while (cur != NULL && cur->toDelete == false) {
			pre = cur;
			cur = cur->next;
		}

		if (cur != NULL) {
			if (pre == NULL) head = cur->next;
			else pre->next = cur->next;
			delete cur;
		}
		else break;
	}
	return head;
	/////////////////////////////////////////////////////////
}
bullet* _deleteBullet(bullet* head) {
	bullet* cur = head;
	bullet* pre = NULL;
	while (true) {

		cur = head;
		pre = NULL;

		while (cur != NULL && cur->toDelete == false) {
			pre = cur;
			cur = cur->next;
		}

		if (cur != NULL) {
			if (pre == NULL) head = cur->next;
			else pre->next = cur->next;
			delete cur;
		}
		else break;
	}
	return head;
	/////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////
//出界
void Fishcheck(Fish* p) {				//检查鱼是否出界
	if (p->exist) {
		if (p->x <= 100) {
			p->rand = 4;
			p->x += fish_move;
		}
		else if (p->x >= 750) {
			p->rand = 5;
			p->x -= fish_move;
		}
		else if (p->y <= 100) {
			p->rand = 6;
			p->y += fish_move;
		}
		else if (p->y >= 600) {
			p->rand = 7;
			p->y -= fish_move;
		}
	}
}
//—————————————————————————————————————————————————————————————————————//


//—————————————————————————————————显示——————————————————————————————————//
void DrawEv(Fish* head, shot* s, bullet* head1, IMAGE pic, IMAGE GameBoard, IMAGE pause, IMAGE continuebutton, Boss* boss, IMAGE ChangeBulletbutton, Hinder* hinderhead) {
	BeginBatchDraw();
	cleardevice();
	if(!boss->come)	putimage(0, 0, &pic);
	else {
		loadimage(&pic, _T(BossBackground_path), 1000, 800);
		putimage(0, 0, &pic);
	}
	if (!player.pause)	putTransparentImage(NULL, 960, 0, &pause);
	else putTransparentImage(NULL, 960, 0, &continuebutton);
	putTransparentImage(NULL, 0, 730, &GameBoard);
	putTransparentImage(NULL, 570, 725, &ChangeBulletbutton);
	Print(head, s, head1, boss, hinderhead);
	FlushBatchDraw();
}
void Print(Fish* head, shot* s, bullet* bullethead, Boss* boss, Hinder* hinderhead) {
	TCHAR Score[20], Fishnum[20], Time[20];
	IMAGE bullet1, bullet2, superbullet1, superbullet2;
	wchar_t textscore[] = L"Score:";
	wchar_t textfish[] = L"Fish:";
	wchar_t texttime[] = L"剩余时间:";
	wchar_t BossCome[] = L"！！！Boss还有五秒降临！！！";
	wchar_t Bossexplain1[] = L"在指定时间内击杀Boss可获得大量积分奖励！";
	wchar_t Bossexplain2[] = L"但是小心Boss会吃掉你的鱼！";
	Fish* p;
	bullet* b;
	MOUSEMSG m;
	Hinder* h;
	IMAGE wildbutton1, wildbutton2;
	LOGFONT* f = new LOGFONT;
	COLORREF color = RGB(0, 255, 255);
	COLORREF color1 = RGB(139, 0, 0);
	if (!player.Changebullet) {
		PutImage(((double)Shot_x - s->pic1r.getwidth() / 2), ((double)Shot_y - s->pic1r.getheight() / 2), &s->pic1r, &s->pic2r);//打印炮台1
	}
	else{
		PutImage(((double)Shot_x - s->darkshot1r.getwidth() / 2), ((double)Shot_y - s->darkshot1r.getheight() / 2), &s->darkshot1r, &s->darkshot2r);//打印暗影炮台
	}
	for (p = head; p != NULL; p = p->next) {
		if (p->score != 60) {
			putTransparentImage(NULL, p->x, p->y, &p->pic);				//打印非幽灵鱼
		}
		else {
			int a, b, c;
			a = rand() % 100;
			b = rand() % 25 + 175;
			if (p->appear < a) {//出现
				putTransparentImage(NULL, p->x, p->y, &p->pic);				//打印幽灵鱼
				p->appear++;
			}
			else if (p->appear >= a && p->appear <= b) {//消失
				p->appear++;
			}
			else if (p->appear > b) {
				p->appear = 0;
			}
		}
		//ellipse(p->x, p->y + p->pic.getheight() / 4, p->x + p->pic.getwidth(), p->y + p->pic.getheight() * 3 / 4);
	}
	for (b = bullethead; b != NULL; b = b->next) {
		if (!b->toDelete && b->exist) {
			if (b->type == 0) {//普通子弹
				PutImage(b->x, b->y, &b->pic1r, &b->pic2r);
			}
			else if (b->type == 1) {//幽灵子弹
				PutImage(b->x, b->y, &b->pics1r, &b->pics2r);
			}
			//circle((int)b->x + (int)(b->pic1r.getwidth() * 2 / 5) - (int)(b->pic1.getheight() * sin(b->degree) / 2),
				//(int)b->y + (int)(b->pic1r.getheight() / 2) - (int)(b->pic1.getheight() * cos(b->degree) / 2), 5);//显示子弹判定位置
		}
		if (b->toDelete) {//子弹爆炸
			putTransparentImage(NULL, b->x - 40, b->y - 40, &b->bulletlight);
			putTransparentImage(NULL, b->x - 40, b->y - 40, &b->bulletlight1);
			putTransparentImage(NULL, b->x - 40, b->y - 40, &b->bulletlight2);
		}
	}
	if ((player.time == 66) || (player.time == 65) || (player.time == 64) || (player.time == 63) || (player.time == 62) || (player.time == 61)) {
		Loadfont(f, color1, 50, 0);
		outtextxy(200, 300, BossCome);
		Loadfont(f, color1, 38, 0);
		outtextxy(150, 375, Bossexplain1);
		outtextxy(250, 450, Bossexplain2);
	}
	if (boss->come && !boss->dead) {
		PutImage(boss->x - 213, boss->y - 125, &boss->light2, &boss->light1);
		PutImage(boss->x, boss->y, &boss->pic1, &boss->pic2);
		//circle(boss->x + (boss->pic1).getwidth()/2, boss->y + (boss->pic1).getheight()/2, 115);//Boss判定区域
	}
	if (!player.Changebullet) {
		loadimage(&bullet1, _T(bullet1_path), 45, 60);
		loadimage(&bullet2, _T(bullet2_path), 45, 60);
		PutImage(380, 730, &bullet1, &bullet2);
	}
	else {
		loadimage(&superbullet1, _T(bullets1_path), 50, 50);
		loadimage(&superbullet2, _T(bullets2_path), 50, 50);
		PutImage(370, 730, &superbullet1, &superbullet2);
	}
	for (h = hinderhead;h != NULL;h = h->next) {
		if (h->HP > 0) {
			putTransparentImage(NULL, h->x, h->y, &h->pic);
			//circle(h->x + h->pic.getwidth() / 2, h->y + h->pic.getheight() / 2, 30);//Hinder判定区域
		}
	}
	if (!player.Wild&& player.Firstwild) {	//狂暴按钮
		loadimage(&wildbutton1, _T(wildbutton1_path), 40, 40);
		loadimage(&wildbutton2, _T(wildbutton2_path), 40, 40);
		PutImage(880, 0, &wildbutton2, &wildbutton1);
		//rectangle(880, 0, 920, 40);
	}
	_stprintf(Score, _T("%d"), player.Score);
	_stprintf(Fishnum, _T("%d"), player.Caughtfish);
	_stprintf(Time, _T("%d"), int(player.time));
	Loadfont(f, color, 20, 0);
	outtextxy(329, 740, Fishnum);
	outtextxy(253, 740, Score);
	outtextxy(740, 740, Time);
	outtextxy(200, 740, textscore);
	outtextxy(286, 740, textfish);
	outtextxy(650, 740, texttime);
}
void DrawRankinglist(IMAGE pic, IMAGE GameBoard) {		//显示排行榜
	IMAGE ReplayButton;
	loadimage(&ReplayButton, _T(ReplayButton_path), 50, 50);
	char* storedata[3][10] = { 0 };
	int i = 0, count = 0, m = 0, n = 0;
	char s[200];
	char getusername[20];
	char getscore[20];
	char getfishnum[20];
	char score1[10] = { 0 };
	char fishnum1[10] = { 0 };
	char str1[30] = { 0 };
	char str2[30] = { 0 };
	FILE* fp;
	TCHAR fishnum[20], score[20], Getusername[20], Getscore[20], Getfishnum[20];
	IMAGE Board1, Board2;
	LOGFONT* f = new LOGFONT;
	COLORREF color1 = RGB(0, 0, 0);
	COLORREF color2 = RGB(0, 0, 205);
	COLORREF color3 = RGB(160, 32, 240);
	COLORREF color4 = RGB(255, 239, 213);
	loadimage(&Board1, _T(board1_path), 800, 600);
	loadimage(&Board2, _T(board2_path), 800, 600);
	wchar_t Rank[] = L"!RankList!";
	wchar_t UserName[] = L"玩家名称";
	wchar_t FishNum[] = L"捕鱼数量";
	wchar_t Score[] = L"用户得分";
	BeginBatchDraw();
	cleardevice();
	putimage(0, 0, &pic);
	PutImage(100, 100, &Board1, &Board2);
	putTransparentImage(NULL, 0, 730, &GameBoard);
	Loadfont(f, color2, 30, 0);
	outtextxy(400, 250, Rank);
	Loadfont(f, color1, 20, 0);
	outtextxy(235, 300, UserName);
	outtextxy(435, 300, Score);
	outtextxy(635, 300, FishNum);
	//显示总排行
	Loadfont(f, color4, 30, 0);
	fp = fopen(Ranklist_path, "r");
	while (fscanf(fp, "%[^\n]", s) != EOF)
	{
		fgetc(fp);
		//for (int j = 0;j < strlen(s);j++) {
		//	outtextxy(235 + 15 * j + 100 * judge, 360 + 30 * count, s[j]);
		//}
		for (int j = 0;j < strlen(s);j++) {
			outtextxy(235 + 15 * j, 360 + 30 * count, s[j]);
			if (s[j] == 13 && s[j + 1] != 13) {
				m = j + 1;
				break;
			}
		}
		for (i = m;i < strlen(s);i++) {
			outtextxy(435 + 15 * (i - m), 360 + 30 * count, s[i]);
			if (s[i] == 13 && s[i + 1] != 13) {
				m = i + 1;
				break;
			}
		}
		for (i = m;i < strlen(s);i++) {
			outtextxy(635 + 15 * (i - m), 360 + 30 * count, s[i]);
			if (s[i] == 13 && s[i + 1] != 13) break;
		}
		count++;
	}
	//显示当前玩家排行
	Loadfont(f, color3, 30, 0);
	if (!player.Tryplay) {
		for (int i = 0;i < strlen(player.username);i++) {
			outtextxy(235 + 15 * i, 330, player.username[i]);
		}
		_stprintf(score, _T("%d"), player.Score);
		_stprintf(fishnum, _T("%d"), player.Caughtfish);
		outtextxy(435, 330, fishnum);
		outtextxy(635, 330, score);
		if ((fp = fopen(Ranklist_path, "r")) == NULL) {
			printf("无法打开文件");
		}
		else {
			fp = fopen(Ranklist_path, "a+");
			fwrite(player.username, sizeof(char), strlen(player.username), fp);
			sprintf(score1, "%d\n", player.Score);
			sprintf(fishnum1, "%d\n", player.Caughtfish);
			fwrite(score1, sizeof(char), strlen(score1), fp);
			fwrite(fishnum1, sizeof(char), strlen(fishnum1), fp);
			fclose(fp);
		}
	}
	putTransparentImage(NULL, 475, 600, &ReplayButton);
	//rectangle(475, 600, 525, 650);
	FlushBatchDraw();
}
//—————————————————————————————————————————————————————————————————————//


//—————————————————————————————————工具——————————————————————————————————//
void Loadfont(LOGFONT* font, COLORREF color, int height, int degree) {
	gettextstyle(font);
	settextcolor(color);
	setbkmode(TRANSPARENT);
	font->lfHeight = height;
	font->lfEscapement = degree * 10;
	font->lfQuality = ANTIALIASED_QUALITY;
	_tcscpy_s(font->lfFaceName, _T("Consolas"));
	settextstyle(font);
}
void putTransparentImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}
void PutImage(int x, int y, IMAGE* mask, IMAGE* img) {
	putimage((int)x, (int)y, mask, SRCAND);
	putimage((int)x, (int)y, img, SRCPAINT);
}
void HpSleep(int ms) {
	static clock_t oldclock = clock();		// 静态变量，记录上一次 tick

	oldclock += ms * CLOCKS_PER_SEC / 1000;	// 更新 tick

	if (clock() > oldclock)					// 如果已经超时，无需延时
		oldclock = clock();
	else
		while (clock() < oldclock)			// 延时
			Sleep(1);						// 释放 CPU 控制权，降低 CPU 占用率
}
double Countdown(time_t begin) {				//倒计时
	time_t end;
	double past;
	end = time(NULL);
	past = difftime(end, begin);
	return past;
}
//—————————————————————————————————————————————————————————————————————//


//————————————————————————————————菜单相关—————————————————————————————————//
void Putmenu() {
	mciSendString(TEXT("open \".\\Audio\\LoginBGM.mp3\" alias Smusic"), NULL, 0, NULL);
	mciSendString(TEXT("play Smusic repeat"), NULL, 0, NULL);
	char inputtext1[20] = { 0 };
	char inputtext2[20] = { 0 };
	char inputusername[15] = { 0 };
	char inputpassword[15] = { 0 };
	FILE* fp;
	MOUSEMSG m;
	IMAGE RegisterButton, LoginButton, Board1, Board2, tryplaybutton1, tryplaybutton2;
	IMAGE RegisterButtont, LoginButtont, tryplaybutton1t, tryplaybutton2t;
	loadimage(&LoginButtont, _T(loginbutton_path), 125, 50);
	loadimage(&RegisterButtont, _T(registerbutton_path), 125, 50);
	loadimage(&tryplaybutton1t, _T(tryplaybutton1_path), 125, 50);
	loadimage(&tryplaybutton2t, _T(tryplaybutton2_path), 125, 50);
	//////////////////////////////////////////////////////////////
	loadimage(&LoginButton, _T(loginbutton_path), 150, 75);
	loadimage(&RegisterButton, _T(registerbutton_path), 150, 75);
	loadimage(&Board1, _T(board1_path), 800, 600);
	loadimage(&Board2, _T(board2_path), 800, 600);
	loadimage(&tryplaybutton1, _T(tryplaybutton1_path), 150, 75);
	loadimage(&tryplaybutton2, _T(tryplaybutton2_path), 150, 75);
	char input = 0;
	while (true) {
		BeginBatchDraw();
		cleardevice();
		Loadbackground();
		putTransparentImage(NULL, 215, 575, &RegisterButtont);
		putTransparentImage(NULL, 615, 575, &LoginButtont);
		PutImage(415, 575, &tryplaybutton1t, &tryplaybutton2t);
		FlushBatchDraw();
		while (MouseHit) {
			m = GetMouseMsg();
			if (choice(m) == 1) {
				BeginBatchDraw();
				cleardevice();
				Loadbackground();
				putTransparentImage(NULL, 200, 560, &RegisterButton);
				putTransparentImage(NULL, 615, 575, &LoginButtont);
				PutImage(415, 575, &tryplaybutton1t, &tryplaybutton2t);
				FlushBatchDraw();
			}
			else if (choice(m) == 2) {
				BeginBatchDraw();
				cleardevice();
				Loadbackground();
				putTransparentImage(NULL, 215, 575, &RegisterButtont);
				putTransparentImage(NULL, 600, 560, &LoginButton);
				PutImage(415, 575, &tryplaybutton1t, &tryplaybutton2t);
				FlushBatchDraw();
			}
			else if (choice(m) == 3) {
				BeginBatchDraw();
				cleardevice();
				Loadbackground();
				putTransparentImage(NULL, 215, 575, &RegisterButtont);
				putTransparentImage(NULL, 615, 575, &LoginButtont);
				PutImage(400, 560, &tryplaybutton1, &tryplaybutton2);
				FlushBatchDraw();
			}
			else {
				BeginBatchDraw();
				cleardevice();
				Loadbackground();
				putTransparentImage(NULL, 215, 575, &RegisterButtont);
				putTransparentImage(NULL, 615, 575, &LoginButtont);
				PutImage(415, 575, &tryplaybutton1t, &tryplaybutton2t);
				FlushBatchDraw();
			}
			if (m.uMsg == WM_LBUTTONDOWN) {
				m = GetMouseMsg();
				if (choice(m) == 1) {	//新用户注册
					int i = 0;
					while (true) {			//输入用户名
						LOGFONT* f = new LOGFONT;
						COLORREF color = RGB(0, 0, 0);
						BeginBatchDraw();
						cleardevice();
						Loadbackground();
						PutImage(100, 100, &Board1, &Board2);
						Register();						//注册信息
						if (_kbhit() && input != 13) {
							input = Getinput();
							if (input == 8 && strlen(inputtext1) > 0) {				//退格删除
								inputtext1[strlen(inputtext1) - 1] = '\0';
							}
							else {
								inputtext1[strlen(inputtext1)] = input;
							}
						}
						else {
							input = '\0';
						}
						Loadfont(f, color, 40, 0);
						for (i = 0;i < strlen(inputtext1);i++) {
							outtextxy(400 + 18 * i, 465, inputtext1[i]);
						}
						if (input == 13) {
							break;
						}
						FlushBatchDraw();
						HpSleep(30);
					}
					i = 0;
					while (true) {			//输入密码
						LOGFONT* f = new LOGFONT;
						COLORREF color = RGB(0, 0, 0);
						BeginBatchDraw();
						cleardevice();
						Loadbackground();
						PutImage(100, 100, &Board1, &Board2);
						Register();
						input = 0;
						if (_kbhit() && input != 13) {
							input = Getinput();
							if (input == 8 && strlen(inputtext2) > 0) {				//退格删除
								inputtext2[i - 1] = '\0';
							}
							else {
								inputtext2[i] = input;
							}
						}
						Loadfont(f, color, 40, 0);
						for (i = 0;i < strlen(inputtext1);i++) {		//保持登录名持续输出
							outtextxy(400 + 18 * i, 465, inputtext1[i]);
						}
						for (i = 0;i < strlen(inputtext2);i++) {
							outtextxy(400 + 18 * i, 565, inputtext2[i]);
						}
						if (input == 13) {
							break;
						}
						FlushBatchDraw();
						HpSleep(30);
					}
					for (i = 0;i < strlen(inputtext1);i++) {
						player.username[i] = inputtext1[i];
					}
					for (i = 0;i < strlen(inputtext2);i++) {
						player.password[i] = inputtext2[i];
					}
					Write(inputtext1, inputtext2);
					break;
				}

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				else if (choice(m) == 2) {	//输入登录信息
					int i = 0;
					while (true) {
						LOGFONT* f = new LOGFONT;
						COLORREF color = RGB(0, 0, 0);
						BeginBatchDraw();
						cleardevice();
						Loadbackground();
						PutImage(100, 100, &Board1, &Board2);
						Register();						//注册信息
						if (_kbhit() && input != 13) {
							input = Getinput();
							if (input == 8 && strlen(inputusername) > 0) {				//退格删除
								inputusername[strlen(inputusername) - 1] = '\0';
							}
							else {
								inputusername[strlen(inputusername)] = input;
							}
						}
						else {
							input = '\0';
						}
						Loadfont(f, color, 40, 0);
						for (i = 0;i < strlen(inputusername);i++) {
							outtextxy(400 + 18 * i, 465, inputusername[i]);
						}
						FlushBatchDraw();
						if (input == 13) {
							break;
						}
						HpSleep(30);
					}
					i = 0;
					while (true) {			//输入密码
						LOGFONT* f = new LOGFONT;
						COLORREF color = RGB(0, 0, 0);
						BeginBatchDraw();
						cleardevice();
						Loadbackground();
						PutImage(100, 100, &Board1, &Board2);
						Register();
						input = 0;
						if (_kbhit() && input != 13) {
							input = Getinput();
							if (input == 8 && strlen(inputpassword) > 0) {				//退格删除
								inputpassword[strlen(inputpassword) - 1] = '\0';
							}
							else {
								inputpassword[i] = input;
							}
						}
						Loadfont(f, color, 40, 0);
						for (i = 0;i < strlen(inputusername);i++) {		//保持登录名持续输出
							outtextxy(400 + 18 * i, 465, inputusername[i]);
						}
						for (i = 0;i < strlen(inputpassword);i++) {
							outtextxy(400 + 18 * i, 565, inputpassword[i]);
						}
						FlushBatchDraw();
						if (input == 13) {
							break;
						}
						FILE* fp;
						fp = fopen(usersave_path, "r");
					}
					for (i = 0;i < strlen(inputusername);i++) {
						player.username[i] = inputusername[i];
					}
					for (i = 0;i < strlen(inputpassword);i++) {
						player.password[i] = inputpassword[i];
					}
					fp = fopen(usersave_path, "r");
					if (check(inputusername, inputpassword, fp)) {
						break;
					}
					else {
						exit(0);
					}
				}

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				else if (choice(m) == 3) {							//进入游客模式
					player.Tryplay = true;
					break;
				}
			}
		}
		break;
	}
}
void Loadbackground() {
	IMAGE  pic, theme;
	loadimage(&theme, _T(theme_path), 400, 180);
	loadimage(&pic, _T(loginbackground_path), 1000, 800);
	putimage(0, 0, &pic);
	putTransparentImage(NULL, 270, 100, &theme);
	wchar_t welcome[] = L"欢迎来到深海捕鱼达人";
	LOGFONT* f = new LOGFONT;
	COLORREF color1 = RGB(238, 0, 0);
	Loadfont(f, color1, 58, 0);
	outtextxy(220, 340, welcome);
}
int choice(MOUSEMSG m) {
	if (m.x > 200 && m.x < 350 && m.y>560 && m.y < 635) {
		return 1;
	}
	else if (m.x > 600 && m.x < 750 && m.y>560 && m.y < 635) {
		return 2;
	}
	else if (m.x > 410 && m.x < 540 && m.y>570 && m.y < 625) {
		return 3;
	}
	return 0;
}
void Register() {
	wchar_t userName_text[] = L"用户名：";
	wchar_t passWord_text[] = L"密  码：";
	wchar_t Remind_text[] = L"请依次输入用户名密码";
	LOGFONT* f = new LOGFONT;
	COLORREF color = RGB(100, 149, 237);
	COLORREF color1 = RGB(255, 215, 0);
	Loadfont(f, color1, 60, 0);
	outtextxy(230, 300, Remind_text);
	setfillcolor(color);
	setlinecolor(color);
	bar(400, 450, 750, 530);//画个无边框矩形
	bar(400, 550, 750, 630);//画个无边框矩形
	Loadfont(f, color, 58, 0);
	outtextxy(200, 460, userName_text);
	outtextxy(200, 560, passWord_text);
}
char Getinput()
{
	char m = 0;
	if (_kbhit()) {
		m = _getwch();
	}
	return m;
}
void Write(char  username[], char  password[]) {
	char str1[30] = { 0 };
	char str2[30] = { 0 };
	FILE* fp;
	if ((fp = fopen(usersave_path, "r")) == NULL) {
		printf("无法打开文件");
	}
	else {
		fp = fopen(usersave_path, "a+");
		fwrite(username, sizeof(char), strlen(username), fp);
		fwrite(password, sizeof(char), strlen(password), fp);
		fclose(fp);
	}
}
bool check(char input1[20], char input2[20], FILE* fp) {
	char username[20];
	char password[20];
	while (!feof(fp))
	{
		memset(username, 0, sizeof(username));
		fgets(username, 20, fp); // 包含了换行符
		if (strcmp(username, input1))
		{
			fgets(password, 20, fp);
			if (strcmp(password, input2)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	fclose(fp);
	return false;
}
//—————————————————————————————————————————————————————————————————————//


//————————————————————————————————Boss关卡—————————————————————————————————//
Boss* Bossinitialize() {
	Boss* boss = new Boss;
	boss->HP = 200;
	boss->score = 1000;
	boss->x = rand() % 300 + 300;
	boss->y = rand() % 100 + 150;
	boss->come = false;
	boss->dead = false;
	boss->rand = rand() % 8;
	boss->count = 0;
	loadimage(&boss->light1, _T(Bosslight1_path), 770, 530);
	loadimage(&boss->light2, _T(Bosslight2_path), 770, 530);
	loadimage(&boss->pic1, _T(boss1_path), 300, 300);
	loadimage(&boss->pic2, _T(boss2_path), 300, 300);
	return boss;
}
void BossdeadFish(Fish** head, Boss* boss) {
	Fish* _newNode = new Fish;
	Fish* s = *head;
	int r, y, x, r1;
	r = rand() % (fish_type - 1);
	r1 = rand() % 4;
	if (r1 == 0) {
		x = boss->x + (boss->pic1).getwidth() / 2 + rand() % 115;
		y = boss->y + (boss->pic1).getheight() / 2 + rand() % 115;
	}
	else if (r1 == 1) {
		x = boss->x + (boss->pic1).getwidth() / 2 + (rand() % 115 + 10) * (-1);
		y = boss->y + (boss->pic1).getheight() / 2 + rand() % 115;
	}
	else if (r1 == 2) {
		x = boss->x + (boss->pic1).getwidth() / 2 + rand() % 115;
		y = boss->y + (boss->pic1).getheight() / 2 + (rand() % 115 + 10) * (-1);
	}
	else if (r1 == 3) {
		x = boss->x + (boss->pic1).getwidth() / 2 + (rand() % 115 + 10) * (-1);
		y = boss->y + (boss->pic1).getheight() / 2 + (rand() % 115 + 10) * (-1);
	}
	_newNode->exist = true;
	_newNode->y = y;
	_newNode->x = x;
	_newNode->rand = rand() % 8;
	if (r == 0) {
		loadimage(&_newNode->pic, _T(Rewardfish1_path), fish_length, fish_width);
		_newNode->score = 10 * 3;
	}
	else if (r == 1) {
		loadimage(&_newNode->pic, _T(Rewardfish2_path), fish_length, fish_width);
		_newNode->score = 5 * 3;
	}
	else if (r == 2) {
		loadimage(&_newNode->pic, _T(Rewardfish3_path), fish_length, fish_width);
		_newNode->score = 15 * 3;
	}
	else if (r == 3) {
		loadimage(&_newNode->pic, _T(Rewardfish4_path), fish_length + 10, fish_width);
		_newNode->score = 40 * 3;
	}
	else if (r == 4) {
		loadimage(&_newNode->pic, _T(Rewardfish5_path), fish_length + 60, fish_width + 40);
		_newNode->score = 30 * 3;
	}
	else if (r == 5) {
		loadimage(&_newNode->pic, _T(Rewardfish6_path), fish_length + 60, fish_width);
		_newNode->score = 50 * 3;
	}
	for (;s->next != NULL;s = s->next) {}
	s->next = _newNode;
	_newNode->next = NULL;
	return;
}
//—————————————————————————————————————————————————————————————————————//


//————————————————————————————————障碍Hinder————————————————————————————————//
Hinder* Hinderinitialize() {
	Hinder* hinder = new Hinder;
	loadimage(&hinder->pic, _T(Hinder_path), 70, 70);
	hinder->x = rand() % 500 + 250;
	hinder->y = rand() % 400 + 150;
	hinder->HP = rand() % 50 + 50;
	hinder->next = NULL;
	return hinder;
}
void InsertHinder(Hinder** Hinderhead) {
	Hinder* s = *Hinderhead;
	for (;s->next != NULL;s = s->next) {}
	Hinder* p = Hinderinitialize();
	s->next = p;
}
//—————————————————————————————————————————————————————————————————————//