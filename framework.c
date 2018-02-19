#define _CRT_SECURE_NO_WARNINGS

#define LEFT 75
#define RIGHT 77


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>
#include<time.h>
#include"Screen.h""

typedef enum _GAME_STAGE {
	READY, RUNNING, PAUSE, RESULT
}GAME_STAGE;
GAME_STAGE GameStage = READY;

typedef struct _PLAYER { // ♀
	int nPosX;
	int nPosY;

}PLAYER;
PLAYER Player;
char strPlayer[] = "♀";

typedef struct  _STAR {
	int nIsReady; // 별 요소의 준비상태
	int nPosX;
	int nPosY;
	clock_t MoveTime; // 이동시간
	clock_t OldTime;   // 이전 이동시간
}STAR;
STAR Star[100];
char strStar[] = "☆";


typedef struct _RAPID_STAR {
	int nIsReady; 
	int nPosX;
	int nPosY;
	clock_t MoveTime; // 이동시간
	clock_t OldTime;   // 이전 이동시간
}RAPID_STAR;
RAPID_STAR RapidStar[50];
char strRapidStar[] = "★";




// 점수
int nScore = 0;
clock_t TotalTime = 0.0;


// 스테이지 기본 틀
void Map(void) {
	int nNum = 0;
	ScreenPrint(0, 0, "□□□□□□□□□□□□□□□□□□□□□");
	for (int i = 1; i < 22; i++) {
		ScreenPrint(0, i, "□\t\t\t\t\t□");
	}
	ScreenPrint(0, 22, "□□□□□□□□□□□□□□□□□□□□□");
}

// 점수출력 화면
void Score(void) {
	ScreenPrint(42, 0, "");
	char chScore[100];
	sprintf(chScore, "점수 : %d점", nScore);
	ScreenPrint(43, 1, chScore);
	char chTime[30];
	sprintf(chTime, "시간 : %d.%d초", TotalTime/10, TotalTime %10);
	ScreenPrint(43, 2, chTime);
	ScreenPrint(42, 3, "");
	ScreenPrint(42, 4, "");
}


// 시작 화면
void StartPage1(void) {
	ScreenPrint(9,10,"Press Enter to Start!!");

}

// 별 스폰 함수
void StarSpwan(STAR* ptStar) {
	ptStar->nIsReady = 0;
	ptStar->nPosY = 1;
	ptStar->nPosX = (rand()%37)+2; // 별 스폰위치의 무작위 배정
}

void RapidStarSpwan(RAPID_STAR* ptStar) { // 3초에 하나씩 생성
	ptStar->nIsReady = 0;
	ptStar->nPosY = 1;
	ptStar->nPosX = (rand() % 37) + 2; // 별 스폰위치의 무작위 배정
}


// 스폰된 별 움직임 함수
void StarsMoving(void) {
	for (int i = 0; i < 100; i++) {
		if (!Star[i].nIsReady) {
			ScreenPrint(Star[i].nPosX, Star[i].nPosY, strStar);
		}
	}
}
void RapidStarsMoving(void) {
	for (int i = 0; i < 50; i++) {
		if (!RapidStar[i].nIsReady) {
			ScreenPrint(RapidStar[i].nPosX, RapidStar[i].nPosY, strRapidStar);
		}
	}


}



// 결과 처리 
void ScreenResult(void){
	ScreenPrint(3, 3, "Result");
	ScreenPrint(3, 4, "=========");
	char chScore[100];
	sprintf(chScore, "점수 : %d점", nScore);
	ScreenPrint(3, 5, chScore);
}








void init() {
	// 게임 스테이지 초기화
	GameStage = READY;
	

	// 플레이어 초기화
	Player.nPosX = 20;
	Player.nPosY = 21;

	// 별 초기화
	Star[0].nPosX = 20;
	Star[0].nPosY = 0;
	Star[0].MoveTime = 100;
	Star[0].OldTime = clock();
	for (int i = 1; i < 100; i++) {
		Star[i].nIsReady = 1; // nIsReady가 1이면 출발가능상태, 0이면 내려가는 중
		Star[i].nPosY = 1;
		Star[i].MoveTime = 100;
	}

	// 빠른 별 초기화
	RapidStar[0].nPosX = 20;
	RapidStar[0].nPosY = 0;
	RapidStar[0].MoveTime = 800;
	RapidStar[0].OldTime = clock();
	for (int i = 1; i < 50; i++) {
		RapidStar[i].nIsReady = 1; // nIsReady가 1이면 출발가능상태, 0이면 내려가는 중
		RapidStar[i].nPosY = 1;
		RapidStar[i].MoveTime = 50;
	}


}


void Update() {
	clock_t Curtime = clock();
	

	switch (GameStage) {
	case READY:
		break;

	case RUNNING :
		TotalTime = clock()/ 100;

		// 0.5초마다 별 하나씩 SPWAN
		if (Curtime - Star[0].OldTime > Star[0].MoveTime) {
			Star[0].OldTime = Curtime;
			for (int i = 1; i < 100; i++) {
				if (Star[i].nIsReady) {
					// 스폰 함수
					StarSpwan(&Star[i]);
					break;
				}
			}
		}

		// 3.0초마다 빠른 별 하나씩 SWPAN
		if (Curtime - RapidStar[0].OldTime > RapidStar[0].MoveTime) {
			RapidStar[0].OldTime = Curtime;
			for (int i = 1; i < 50; i++) {
				if (RapidStar[i].nIsReady) {
					// 스폰 함수
					RapidStarSpwan(&RapidStar[i]);
					break;
				}
			}
		}

		// 출발한 별들에 대한 제어 부분
		for (int i = 1; i < 100; i++) {
			if (!Star[i].nIsReady) {
				if (Curtime - Star[i].OldTime > Star[i].MoveTime) {
					Star[i].nPosY++;
					Star[i].OldTime = Curtime;
				}
			}
			// 경계선 접촉 시 별 초기화
			if (Star[i].nPosY > 21) {
				Star[i].nIsReady = 1;
				Star[i].nPosY = 0;
				nScore++;

			}

		}
		// 출발한 빠른 별들에 대한 제어 부분
		for (int i = 1; i < 50; i++) {
			if (!RapidStar[i].nIsReady) {
				if (Curtime - RapidStar[i].OldTime > RapidStar[i].MoveTime) {
					RapidStar[i].nPosY++;
					RapidStar[i].OldTime = Curtime;
				}
			}
			if (RapidStar[i].nPosY > 21) {
				RapidStar[i].nIsReady = 1;
				RapidStar[i].nPosY = 0;
				nScore++;
			}
		}




		// 플레이어와 충돌 처리
		for (int i = 0; i < 100; i++) {

			if (Star[i].nPosY == Player.nPosY) {
				if (Star[i].nPosX + 1 == Player.nPosX || Star[i].nPosX == Player.nPosX || Star[i].nPosX == Player.nPosX + 1) {
					GameStage = RESULT;
				}
			}

			if (RapidStar[i].nPosY == Player.nPosY) {
				if (RapidStar[i].nPosX + 1 == Player.nPosX || RapidStar[i].nPosX == Player.nPosX || RapidStar[i].nPosX == Player.nPosX + 1) {
					GameStage = RESULT;
				}
			}
		}
		break;
	case PAUSE:
		break;


	case RESULT :
		break;
	}
	
	
	



}

void Render() {
	clock_t Curtime = clock();
	ScreenClear();
	//출력코드
	Map();
	ScreenPrint(43, 11, "Press \'p\'to Pause");
	ScreenPrint(43, 13, "Press \'q\'to Regame");
	ScreenPrint(43, 15, "Press \'s\'to Quit");
	Score();
	switch (GameStage) {
	case READY:
		if (Curtime % 1000 > 500) {
			StartPage1();
		}
		break;
	case PAUSE:
		if (Curtime % 1000 > 500) {
			ScreenPrint(43, 10, "PAUSE");
		}
		ScreenPrint(43, 11, "Press \'p\'to Restart");

	case RUNNING:
		ScreenPrint(Player.nPosX, Player.nPosY, strPlayer);
		//ScreenPrint(Star[0].nPosX, Star[0].nPosY, strStar);
		StarsMoving();
		RapidStarsMoving();
		break;

	case RESULT:
		ScreenResult();

		break;
	}
	ScreenFlipping();
}



void Release() {

}



int main(void) {
	
	ScreenInit();
	init(); // 초기화

	while (1) {
		int nKey;
		if (_kbhit()) {
			nKey = _getch();
			if (nKey == 224) {
				nKey = _getch();
				switch (nKey) {
				case LEFT :
					Player.nPosX--;
					break;
				case RIGHT :
					Player.nPosX++;
					break;
				default :
					printf("\a");
					break;
				}
			}
			else if (nKey == 13) {
				GameStage = RUNNING;
			}
			else if (nKey == 'p') {
				if (GameStage == RUNNING) {
					GameStage = PAUSE;
				}
				else if (GameStage == PAUSE) {
					GameStage = RUNNING;
				}
			}
			else if (nKey == 'q') {
				GameStage = READY;
			}
			else if (nKey == 's') {
				break;
			
			}


		}
		Update();  // 데이터 갱신
		Render();  // 화면출력
	}



	Release(); // 해제
	ScreenRelease();
	return 0;
}