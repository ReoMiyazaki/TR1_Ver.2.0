#include "Global.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言
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

	float targetLength = 800;	// 初期値が800なのは(0,0)と(640,480)のベクトルの長さ

	int searchValue = 0;

	int pattern = 0;

	int tmp = 0;


	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 255; i++) {
			oldkeys[i] = keys[i];
		}
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// - - - - - 更新処理 - - - - - //

//		if (-1 < playerPos.x || playerPos.x < 640) {
//			if (-1 < playerPos.y || playerPos.x < 480) {
//				if (keys[KEY_INPUT_W]) { playerPos.y--; };
//				if (keys[KEY_INPUT_A]) { playerPos.x--; };
//				if (keys[KEY_INPUT_S]) { playerPos.y++; };
//				if (keys[KEY_INPUT_D]) { playerPos.x++; };
//			}
//		}

		// 上下矢印で判定切り替え
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

		// 左右矢印でターゲット変更
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

		// スペースキーを押したとき
		if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {

			// ターゲットをプレイヤーの座標にリセット
			target = playerPos;

			// 敵の座標、識別番号、プレイヤーまでの距離などをそれぞれ代入とリセット
			for (int i = 0; i < enemyMax; i++) {
				enemy[i].pos.x = rand() % 600 + 40;
				enemy[i].pos.y = rand() % 400 + 80;
				enemy[i].num = rand() % 10 + 1;
				enemy[i].length = enemy[i].pos.length() - playerPos.length();
				if (enemy[i].length < 0) {
					enemy[i].length *= -1;	// 今回はあまり関係ない、ベクトルの長さがマイナスになった場合の処理
				}
				enemy[i].hp = rand() % 100 + 1;
			}
			tmp = 0;
			targetLength = 800;

			// 判定パターンでターゲットする敵を変更
			switch (pattern)
			{
			case 0:
				// 敵の識別番号がターゲットする識別番号と同じで、プレイヤーにより近い敵を狙う
				for (int i = 0; i < enemyMax; i++) {

					if (enemy[i].num == searchValue) {			// ここでターゲットする敵かどうか判別
						if (enemy[i].length < targetLength) {	// ターゲットまでのベクトルの長さの長短を判別
							target = enemy[i].pos;				// より近い敵がいたらターゲット変更
							targetLength = enemy[i].length;		//　その敵までの距離を更新
						}
					}
				}
				break;

			case 1:
				// 一番近い敵を狙う(コードはcase 0とあまり変わらない)
				for (int i = 0; i < enemyMax; i++) {
					if (enemy[i].length < targetLength) {
						target = enemy[i].pos;
						targetLength = enemy[i].length;
					}
				}
				break;

			case 2:
				// ランダムで敵を狙う
				searchValue = rand() % enemyMax;	// ターゲットをランダム選出(上限は敵の最大数)
				target = enemy[searchValue].pos;	//　ターゲットする敵を更新
				break;

			case 3:
				// HPが一番低い敵を狙う
				for (int i = 0; i < enemyMax; i++) {			// 一番体力のある敵を確定させるループ処理
					for (int j = 0; j < enemyMax - 1; j++) {	// 隣り合う要素の大小を判定
						if (enemy[j].hp > enemy[j + 1].hp) {
							tmp = enemy[j].hp;					// 
							enemy[j].hp = enemy[j + 1].hp;		// enemy[j]のHPが大きい場合要素の入替
							enemy[j + 1].hp = tmp;				// 
						}
					}
				}
				target = enemy[0].pos;	// ターゲットする敵を更新
				break;
			}
		}


		// - - - - - 描画処理 - - - - - //

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
			DrawFormatString(660, 25, GetColor(255, 255, 255), "現在の判定：敵の識別番号が、ターゲットする敵の識別番号と同じ場合");
			DrawFormatString(660, 45, GetColor(255, 255, 255), "ターゲット：Number[ %d ]", searchValue);
			break;

		case 1:
			for (int i = 0; i < enemyMax; i++) {
				DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), false);
				DrawCircle(target.x, target.y, enemy[i].r, GetColor(255, 255, 255), true);
			}
			DrawFormatString(660, 25, GetColor(255, 255, 255), "現在の判定：プレイヤーから一番近い敵を狙う");
			break;

		case 2:
			for (int i = 0; i < enemyMax; i++) {
				DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), false);
				DrawCircle(target.x, target.y, enemy[i].r, GetColor(255, 255, 255), true);
				DrawFormatString(660, 25, GetColor(255, 255, 255), "現在の判定：ランダムで選択");
				DrawFormatString(660, 45, GetColor(255, 255, 255), "ターゲット：Number[ %d ]", searchValue);
			}
			break;

		case 3:
			for (int i = 0; i < enemyMax; i++) {
				DrawCircle(enemy[i].pos.x, enemy[i].pos.y, enemy[i].r, GetColor(255, 255, 255), false);
				DrawCircle(target.x, target.y, enemy[i].r, GetColor(255, 255, 255), true);
				DrawFormatString(660, 25, GetColor(255, 255, 255), "現在の判定：HPが一番低い敵を狙う");
				DrawFormatString(660, 45 + 20 * i, GetColor(255, 255, 255), "Number[ %d ]：HP[ %d ]", enemy[i].num, enemy[i].hp);
			}
			break;
		}

		DrawCircle(playerPos.x, playerPos.y, playerRadius, GetColor(250, 250, 250), true);
		DrawLine(650, 0, 650, 480, GetColor(250, 50, 50));

		DrawFormatString(660, 5, GetColor(255, 255, 255), "SPACEで敵の位置をランダム変更、上下で判定切り替え、左右でターゲット変更");


		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
