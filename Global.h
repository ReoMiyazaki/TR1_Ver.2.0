#pragma once

#include "DxLib.h"
#include "stdlib.h"
#include "time.h"
#include "Vector2.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "ターゲティング";

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 480;

typedef struct Enemy {
	Vector2 pos;
	int r;
	int num;
	float length;
	int hp;
}; 
