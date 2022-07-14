#include "Global.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���


	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	srand(time(nullptr));

	Vector2 playerPos = { 0,0 };
	Vector2 target = playerPos;
	int playerRadius = 25;

	const int enemyMax = 25;
	Enemy enemy[enemyMax];
	for (int i = 0; i < enemyMax; i++) {
		enemy[i].pos.x = rand() % 600 + 40;
		enemy[i].pos.y = rand() % 400 + 80;
		enemy[i].r = 10;
		enemy[i].num = 0;
		enemy[i].length = 0;
		enemy[i].hp = 0;
	}

	float targetLength = 800;	// �����l��800�Ȃ̂�(0,0)��(640,480)�̃x�N�g���̒���

	int searchValue = 0;

	int pattern = 0;

	int tmp = 0;


	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 255; i++) {
			oldkeys[i] = keys[i];
		}
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// - - - - - �X�V���� - - - - - //

//		if (-1 < playerPos.x || playerPos.x < 640) {
//			if (-1 < playerPos.y || playerPos.x < 480) {
//				if (keys[KEY_INPUT_W]) { playerPos.y--; };
//				if (keys[KEY_INPUT_A]) { playerPos.x--; };
//				if (keys[KEY_INPUT_S]) { playerPos.y++; };
//				if (keys[KEY_INPUT_D]) { playerPos.x++; };
//			}
//		}

		// �㉺���Ŕ���؂�ւ�
		if (keys[KEY_INPUT_UP] == 1 && oldkeys[KEY_INPUT_UP] == 0) {
			pattern++;
			if (pattern >= 4) {
				pattern = 0;
			}
		}
		if (keys[KEY_INPUT_DOWN] == 1 && oldkeys[KEY_INPUT_DOWN] == 0) {
			pattern--;
			if (pattern < 0) {
				pattern = 3;
			}
		}

		// ���E���Ń^�[�Q�b�g�ύX
		if (pattern != 2) {
			if (keys[KEY_INPUT_RIGHT] == 1 && oldkeys[KEY_INPUT_RIGHT] == 0) {
				searchValue++;
				if (searchValue > 10) {
					searchValue = 1;
				}
			}
			if (keys[KEY_INPUT_LEFT] == 1 && oldkeys[KEY_INPUT_LEFT] == 0) {
				searchValue--;
				if (searchValue < 1) {
					searchValue = 10;
				}
			}
		}

		// �X�y�[�X�L�[���������Ƃ�
		if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {

			// �^�[�Q�b�g���v���C���[�̍��W�Ƀ��Z�b�g
			target = playerPos;

			// �G�̍��W�A���ʔԍ��A�v���C���[�܂ł̋����Ȃǂ����ꂼ�����ƃ��Z�b�g
			for (int i = 0; i < enemyMax; i++) {
				enemy[i].pos.x = rand() % 600 + 40;
				enemy[i].pos.y = rand() % 400 + 80;
				enemy[i].num = rand() % 10 + 1;
				enemy[i].length = enemy[i].pos.length() - playerPos.length();
				if (enemy[i].length < 0) {
					enemy[i].length *= -1;	// ����͂��܂�֌W�Ȃ��A�x�N�g���̒������}�C�i�X�ɂȂ����ꍇ�̏���
				}
				enemy[i].hp = rand() % 100 + 1;
			}
			tmp = 0;
			targetLength = 800;

			// ����p�^�[���Ń^�[�Q�b�g����G��ύX
			switch (pattern)
			{
			case 0:
				// �G�̎��ʔԍ����^�[�Q�b�g���鎯�ʔԍ��Ɠ����ŁA�v���C���[�ɂ��߂��G��_��
				for (int i = 0; i < enemyMax; i++) {

					if (enemy[i].num == searchValue) {			// �����Ń^�[�Q�b�g����G���ǂ�������
						if (enemy[i].length < targetLength) {	// �^�[�Q�b�g�܂ł̃x�N�g���̒����̒��Z�𔻕�
							target = enemy[i].pos;				// ���߂��G��������^�[�Q�b�g�ύX
							targetLength = enemy[i].length;		//�@���̓G�܂ł̋������X�V
						}
					}
				}
				break;

			case 1:
				// ��ԋ߂��G��_��(�R�[�h��case 0�Ƃ��܂�ς��Ȃ�)
				for (int i = 0; i < enemyMax; i++) {
					if (enemy[i].length < targetLength) {
						target = enemy[i].pos;
						targetLength = enemy[i].length;
					}
				}
				break;

			case 2:
				// �����_���œG��_��
				searchValue = rand() % enemyMax;	// �^�[�Q�b�g�������_���I�o(����͓G�̍ő吔)
				target = enemy[searchValue].pos;	//�@�^�[�Q�b�g����G���X�V
				break;

			case 3:
				// HP����ԒႢ�G��_��
				for (int i = 0; i < enemyMax; i++) {			// ��ԑ̗͂̂���G���m�肳���郋�[�v����
					for (int j = 0; j < enemyMax - 1; j++) {	// �ׂ荇���v�f�̑召�𔻒�
						if (enemy[j].hp > enemy[j + 1].hp) {
							tmp = enemy[j].hp;					// 
							enemy[j].hp = enemy[j + 1].hp;		// enemy[j]��HP���傫���ꍇ�v�f�̓���
							enemy[j + 1].hp = tmp;				// 
						}
					}
				}
				target = enemy[0].pos;	// �^�[�Q�b�g����G���X�V
				break;
			}
		}


		// - - - - - �`�揈�� - - - - - //

		DrawLine(playerPos.x, playerPos.y, target.x, target.y, GetColor(250, 50, 50));

		switch (pattern)
		{
		case 0:
			for (int i = 0; i < enemyMax; i++) {
				if (enemy[i].num == searchValue) {
					DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), true);
				}
				else {
					DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), false);
				}
			}
			DrawFormatString(660, 25, GetColor(255, 255, 255), "���݂̔���F�G�̎��ʔԍ����A�^�[�Q�b�g����G�̎��ʔԍ��Ɠ����ꍇ");
			DrawFormatString(660, 45, GetColor(255, 255, 255), "�^�[�Q�b�g�FNumber[ %d ]", searchValue);
			break;

		case 1:
			for (int i = 0; i < enemyMax; i++) {
				DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), false);
				DrawCircle(target.x, target.y, enemy[i].r, GetColor(255, 255, 255), true);
			}
			DrawFormatString(660, 25, GetColor(255, 255, 255), "���݂̔���F�v���C���[�����ԋ߂��G��_��");
			break;

		case 2:
			for (int i = 0; i < enemyMax; i++) {
				DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), false);
				DrawCircle(target.x, target.y, enemy[i].r, GetColor(255, 255, 255), true);
				DrawFormatString(660, 25, GetColor(255, 255, 255), "���݂̔���F�����_���őI��");
				DrawFormatString(660, 45, GetColor(255, 255, 255), "�^�[�Q�b�g�FNumber[ %d ]", searchValue);
			}
			break;

		case 3:
			for (int i = 0; i < enemyMax; i++) {
				DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), false);
				DrawCircle(target.x, target.y, enemy[i].r, GetColor(255, 255, 255), true);
				DrawFormatString(660, 25, GetColor(255, 255, 255), "���݂̔���FHP����ԒႢ�G��_��");
				DrawFormatString(660, 45 + 20 * i, GetColor(255, 255, 255), "Number[ %d ]�FHP[ %d ]", enemy[i].num, enemy[i].hp);
			}
			break;
		}

		DrawCircle(playerPos.x, playerPos.y, playerRadius, GetColor(250, 250, 250), true);
		DrawLine(650, 0, 650, 480, GetColor(250, 50, 50));

		DrawFormatString(660, 5, GetColor(255, 255, 255), "SPACE�œG�̈ʒu�������_���ύX�A�㉺�Ŕ���؂�ւ��A���E�Ń^�[�Q�b�g�ύX");


		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
