#pragma once

#include "DxLib.h"
#include "stdlib.h"
#include "time.h"
#include "Vector2.h"

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "�^�[�Q�e�B���O";

// �E�B���h�E����
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��
const int WIN_HEIGHT = 480;

typedef struct Enemy {
	Vector2 pos;
	int r;
	int num;
	float length;
	int hp;
}; 
