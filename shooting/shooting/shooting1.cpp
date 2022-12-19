#include "DxLib.h"
#include "stdio.h"
#include "stdlib.h"
#include "random"
#include "string"
#include "iostream"
#include "time.h"
#include "resource.h"

#define WIDTH 640
#define HEIGHT 800

int sub[8], flag[8], N = 0, e_count[2] = { 0 }, body, PL = -1, PR = -1, Ml = 0, Mr = 0, tmpl = 0, tmpr = 0, Cl[2][3] = { 0 }, ep[100], score = 0, TIM = 0;

typedef struct {
    int x;
    int y;
    int r;
    bool flag;
} Bullet;

typedef struct {
    int x;
    int y;
    int vel;
    int power;
    bool flag;
    Bullet b[200];
} Character;

//キャラクター
Character pl = {}, pr = {};
Character e[100] = {};

void gameInit() {
    //プレイヤーの初期状態
    pl.x = 320 - 24, pr.x = 320 + 24;
    pl.y = 700, pr.y = 700;
    pl.vel = 5, pr.vel = 5;
    for (int i = 0; i < 200; i++) {
        pl.b[i].flag = false;
        pr.b[i].flag = false;
    }

    for (int i = 0; i < 100; i++) {
        srand(time(NULL) * i - 13 * i);
        ep[i] = rand() % 1000 + 10;
    }
    for (int s = 0; s < 100; s++) {
        for (int t = s + 1; t < 100; t++) {
            if (ep[t] < ep[s]) {
                int tmp = ep[t];
                ep[t] = ep[s];
                ep[s] = tmp;
            }
        }
    }

    //敵の座標
    for (int i = 0; i < 100; i++) {
        srand(time(NULL) * i * i);
        e[i].x = rand() % 500 + 70;
        e[i].y = 100;
        e[i].flag = false;
        e[i].power = ep[i];
    }

    for (int i = 0; i < 8; i++) {
        flag[i] = 0;
    }
    e_count[0] = 0;
    e_count[1] = 0;
}

void LoadData() {
    body = LoadGraphToResource(MAKEINTRESOURCE(101), "PNG");
    sub[0] = LoadGraphToResource(MAKEINTRESOURCE(102), "PNG");
    sub[1] = LoadGraphToResource(MAKEINTRESOURCE(103), "PNG");
    sub[2] = LoadGraphToResource(MAKEINTRESOURCE(104), "PNG");
    sub[3] = LoadGraphToResource(MAKEINTRESOURCE(105), "PNG");
    sub[4] = LoadGraphToResource(MAKEINTRESOURCE(106), "PNG");
    sub[5] = LoadGraphToResource(MAKEINTRESOURCE(107), "PNG");
    sub[6] = LoadGraphToResource(MAKEINTRESOURCE(108), "PNG");
    sub[7] = LoadGraphToResource(MAKEINTRESOURCE(109), "PNG");
}

//タイトル画面
int title(char buf[]) { //gamemode 0
    SetFontSize(72);
    DrawString(80, 150, "Shooting Game", GetColor(255, 255, 255));
    SetFontSize(36);
    DrawString(420, 240, "with math", GetColor(255, 255, 255));
    DrawString(130, 550, "Enterでゲームスタート", GetColor(255, 255, 255));

    if (buf[KEY_INPUT_RETURN] == 1) return 1; //gamemode select
    return 0;
}

int select(char buf[]) { //gamemode 1
    int tmp = 0;

    DrawBox(25, 25, 615, 195, GetColor(255, 255, 255), FALSE);
    SetFontSize(20);
    DrawString(40, 40, "このゲームでは，上から流れてくる敵を数字で攻撃します．", GetColor(255, 255, 255));
    DrawString(40, 80, "下の8つの中から2つ選んで攻撃する数字を決めてください．", GetColor(255, 255, 255));
    DrawString(40, 120, "キーボードから選択してENTERで決定してください．", GetColor(255, 255, 255));
    DrawString(40, 160, "一度選ぶと選びなおせません．", GetColor(255, 255, 255));

    DrawRotaGraph(80, 320, 0.25, 0, sub[0], TRUE);
    DrawRotaGraph(240, 320, 0.25, 0, sub[1], TRUE);
    DrawRotaGraph(400, 320, 0.25, 0, sub[2], TRUE);
    DrawRotaGraph(560, 320, 0.25, 0, sub[3], TRUE);
    DrawRotaGraph(80, 600, 0.25, 0, sub[4], TRUE);
    DrawRotaGraph(240, 600, 0.25, 0, sub[5], TRUE);
    DrawRotaGraph(400, 600, 0.25, 0, sub[6], TRUE);
    DrawRotaGraph(560, 600, 0.25, 0, sub[7], TRUE);

    if (buf[KEY_INPUT_2] == 1 && tmp != 2) flag[0] = 1;
    if (buf[KEY_INPUT_3] == 1 && tmp != 2) flag[1] = 1;
    if (buf[KEY_INPUT_4] == 1 && tmp != 2) flag[2] = 1;
    if (buf[KEY_INPUT_5] == 1 && tmp != 2) flag[3] = 1;
    if (buf[KEY_INPUT_6] == 1 && tmp != 2) flag[4] = 1;
    if (buf[KEY_INPUT_7] == 1 && tmp != 2) flag[5] = 1;
    if (buf[KEY_INPUT_8] == 1 && tmp != 2) flag[6] = 1;
    if (buf[KEY_INPUT_9] == 1 && tmp != 2) flag[7] = 1;

    SetFontSize(36);
    if (flag[0] == 1 && tmp != 2) {
        Cl[0][0] = 255;
        Cl[0][1] = 242;
        Cl[0][2] = 0;
        DrawRotaString(40, 280, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp++;
        PL = 0;
    }
    if (flag[1] == 1 && tmp != 2) {
        if (PL != -1) {
            PR = 1;
            Cl[1][0] = 196;
            Cl[1][1] = 255;
            Cl[1][2] = 14;
        }
        else {
            PL = 1;
            Cl[0][0] = 196;
            Cl[0][1] = 255;
            Cl[0][2] = 14;
        }
        DrawRotaString(200, 280, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp++;
    }
    if (flag[2] == 1 && tmp != 2) {
        if (PL != -1) {
            PR = 2;
            Cl[1][0] = 14;
            Cl[1][1] = 209;
            Cl[1][2] = 69;
        }
        else {
            PL = 2;
            Cl[0][0] = 14;
            Cl[0][1] = 209;
            Cl[0][2] = 69;
        }
        DrawRotaString(360, 280, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp++;
    }
    if (flag[3] == 1 && tmp != 2) {
        if (PL != -1) {
            PR = 3;
            Cl[1][0] = 140;
            Cl[1][1] = 255;
            Cl[1][2] = 251;
        }
        else {
            PL = 3;
            Cl[0][0] = 140;
            Cl[0][1] = 255;
            Cl[0][2] = 251;
        }
        DrawRotaString(520, 280, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp++;
    }
    if (flag[4] == 1 && tmp != 2) {
        if (PL != -1) {
            PR = 4;
            Cl[1][0] = 0;
            Cl[1][1] = 168;
            Cl[1][2] = 243;
        }
        else {
            PL = 4;
            Cl[0][0] = 0;
            Cl[0][1] = 168;
            Cl[0][2] = 243;
        }
        DrawRotaString(40, 560, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp++;
    }
    if (flag[5] == 1 && tmp != 2) {
        if (PL != -1) {
            PR = 5;
            Cl[1][0] = 63;
            Cl[1][1] = 72;
            Cl[1][2] = 204;
        }
        else {
            PL = 5;
            Cl[0][0] = 63;
            Cl[0][1] = 72;
            Cl[0][2] = 204;
        }
        DrawRotaString(200, 560, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp++;
    }
    if (flag[6] == 1 && tmp != 2) {
        if (PL != -1) {
            PR = 6;
            Cl[1][0] = 184;
            Cl[1][1] = 61;
            Cl[1][2] = 186;
        }
        else {
            PL = 6;
            Cl[0][0] = 184;
            Cl[0][1] = 61;
            Cl[0][2] = 186;
        }
        DrawRotaString(360, 560, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp++;
    }
    if (flag[7] == 1 && tmp != 2) {
        if (PL != -1) {
            PR = 7;
            Cl[1][0] = 255;
            Cl[1][1] = 174;
            Cl[1][2] = 200;
        }
        else {
            PL = 7;
            Cl[0][0] = 255;
            Cl[0][1] = 174;
            Cl[0][2] = 200;
        }
        DrawRotaString(520, 560, 1, 1, 0, 0, 3.14 / 6, GetColor(255, 0, 0), 0, FALSE, "SELECT");
        tmp;
    }
    SetFontSize(20);

    if (buf[KEY_INPUT_RETURN] == 1) {
        if (tmp == 2) return 2; //gamemode rule
    }
    if (buf[KEY_INPUT_ESCAPE] == 1) return 9; //ESCが押されたら終了

    return 1;
}

int rule(char buf[]) { // gamemode 2
    TIM++;
    DrawBox(25, 25, 615, 775, GetColor(255, 255, 255), FALSE);
    SetFontSize(20);
    DrawString(40, 40, "このゲームでは，上から流れてくる敵を数字で攻撃します．", GetColor(255, 255, 255));
    DrawString(40, 80, "敵も数字(EN)を持っており，自身が選択した数字(PN)が敵", GetColor(255, 255, 255));
    DrawString(40, 120, "に当たったとき以下の計算結果がSCOREに加算されます．", GetColor(255, 255, 255));
    DrawString(40, 160, "  ENを (630 / PN) - PN で割った余りをPN倍した値", GetColor(255, 255, 255));
    DrawString(40, 240, "敵を50体倒せばゲームクリアです．しかし，敵を逃して100", GetColor(255, 255, 255));
    DrawString(40, 280, "体以上敵が出現すると負けとなります．100体目が出現する", GetColor(255, 255, 255));
    DrawString(40, 320, "前に50体倒し切りましょう．", GetColor(255, 255, 255));
    DrawString(40, 360, "敵が画面下部に行ったり，プレイヤーに当たったりすると，", GetColor(255, 255, 255));
    DrawString(40, 400, "敵の持つ数字分だけ減点されます．", GetColor(255, 255, 255));
    DrawString(40, 440, "敵の持つ数字は100体目に近づくにつれて大きくなっていく", GetColor(255, 255, 255));
    DrawString(40, 480, "ので，高得点を狙うには後半の敵を狙うのもいいかもしれま", GetColor(255, 255, 255));
    DrawString(40, 520, "せん．", GetColor(255, 255, 255));
    DrawString(40, 600, "左右の方向キーでプレイヤーを移動，Aキー，Dキーでそれぞ", GetColor(255, 255, 255));
    DrawString(40, 640, "れ対応する弾を発射できます．", GetColor(255, 255, 255));
    DrawString(200, 720, "ENTER でゲームスタート", GetColor(255, 255, 255));

    if (buf[KEY_INPUT_RETURN] == 1 && TIM > 10) return 3; //gamemode game
    if (buf[KEY_INPUT_ESCAPE] == 1) return 9; //ESCが押されたら終了

    return 2;
}

void MoveDraw() {
    //弾のフラグがオンのとき、画面外ならフラグ取り消し、画面内ならY座標を減らして描画。
    for (int i = 0; i < 200; i++) {
        if (pl.b[i].flag == true) {
            if (pl.b[i].y <= 0) {
                pl.b[i].flag = false;
                continue;
            }
            pl.b[i].y -= 5;
            DrawCircleAA(pl.b[i].x, pl.b[i].y, pl.b[i].r, 32, GetColor(Cl[0][0], Cl[0][1], Cl[0][2]), TRUE);
        }
    }
    for (int i = 0; i < 200; i++) {
        if (pr.b[i].flag == true) {
            if (pr.b[i].y <= 0) {
                pr.b[i].flag = false;
                continue;
            }
            pr.b[i].y -= 5;
            DrawCircleAA(pr.b[i].x, pr.b[i].y, pr.b[i].r, 32, GetColor(Cl[1][0], Cl[1][1], Cl[1][2]), TRUE);
        }
    }
}

void shoot_left() {
    //弾の射出フラグを立て、座標をあわせる
    for (int i = 0; i < 200; i++) {
        if (tmpl == 0 || Ml % 10 == 0) {
            if (pl.b[i].flag == false) {
                pl.b[i].flag = true;
                pl.b[i].r = 7;
                pl.b[i].x = pl.x;
                pl.b[i].y = pl.y - 20;
                Ml++;
                tmpl = 1;
                break;
            }
        }
        else {
            Ml++;
            tmpl = 1;
            break;
        }
    }
}

void shoot_right() {
    //弾の射出フラグを立て、座標をあわせる
    for (int i = 0; i < 200; i++) {
        if (tmpr == 0 || Mr % 10 == 0) {
            if (pr.b[i].flag == false) {
                pr.b[i].flag = true;
                pr.b[i].r = 7;
                pr.b[i].x = pr.x;
                pr.b[i].y = pr.y - 20;
                Mr++;
                tmpr = 1;
                break;
            }
        }
        else {
            Mr++;
            tmpr = 1;
            break;
        }
    }
}

void checkHit() {
    for (int i = 0; i < 200; i++) { // bullet
        if (pl.b[i].flag == true) {
            for (int j = 0; j < 100; j++) { // enemy
                if (e[j].flag) {
                    if (pl.b[i].x >= e[j].x && pl.b[i].x <= e[j].x + 50 && pl.b[i].y >= e[j].y && pl.b[i].y <= e[j].y + 50) {
                        e[j].flag = false;
                        pl.b[i].flag = false;
                        e_count[1]++;
                        score += (PL + 2) * (e[j].power % (630 / (PL + 2) - (PL + 2)));
                    }
                }
            }
        }
    }
    for (int i = 0; i < 200; i++) {
        if (pr.b[i].flag == true) {
            for (int j = 0; j < 100; j++) {
                if (e[j].flag) {
                    if (pr.b[i].x >= e[j].x && pr.b[i].x <= e[j].x + 50 && pr.b[i].y >= e[j].y && pr.b[i].y <= e[j].y + 50) {
                        e[j].flag = false;
                        pr.b[i].flag = false;
                        e_count[1]++;
                        score += (PR + 2) * (e[j].power % (630 / (PR + 2) - (PR + 2)));
                    }
                }
            }
        }
    }
    for (int i = 0; i < 100; i++) {
        if (e[i].flag) {
            if (e[i].x + 50 >= pl.x && e[i].x <= pr.x + 10 && e[i].y + 50 >= pl.y - 60 && e[i].y <= pl.y + 80) {
                e[i].flag = false;
                score -= e[i].power;
            }
        }
    }
}

int game(char buf[]) { // gamemode 3
    N++;
    //プレイヤーの移動
    if (buf[KEY_INPUT_LEFT] == 1) {
        pl.x -= pl.vel;
        pr.x -= pr.vel;
        if (pl.x < 20) {
            pl.x = 20;
            pr.x = 20 + 48;
        }
    }
    if (buf[KEY_INPUT_RIGHT] == 1) {
        pl.x += pl.vel;
        pr.x += pr.vel;
        if (pr.x > 620) {
            pr.x = 620;
            pl.x = 620 - 48;
        }
    }

    if (N % 120 == 1) {
        e_count[0]++;
        e[e_count[0]].flag = 1;
    }

    //敵の描画(正方形)
    for (int i = 0; i < 100; i++) {
        if (e[i].flag == 1) {
            DrawBoxAA(e[i].x, e[i].y, e[i].x + 50, e[i].y + 50, GetColor(255, 255, 255), TRUE);
            DrawFormatString(e[i].x + 10, e[i].y + 15, GetColor(0, 0, 0), "%d", e[i].power);
            if (N % 2 == 0) e[i].y += 1.0;
            if (e[i].y > 800) e[i].flag = false;
        }
    }

    //プレイヤーの描画
    DrawRotaGraph(pl.x + 24, 700, 0.15, 0, body, TRUE); //144x144 / 72
    DrawRotaGraph(pl.x, 730, 0.12, 0, sub[PL], TRUE); //115x115 / 57
    DrawRotaGraph(pr.x, 730, 0.12, 0, sub[PR], TRUE); //115x115 . 57

    MoveDraw(); //弾の描画、移動

    checkHit();

    DrawFormatString(30, 10, GetColor(255, 255, 255), "SCORE：%d", score);
    DrawFormatString(30, 30, GetColor(255, 255, 255), "出現した敵の数：%d", e_count[0]);
    DrawFormatString(30, 50, GetColor(255, 255, 255), "倒した敵の数：%d", e_count[1]);

    if (e_count[1] == 50) return 4;
    if (e_count[0] == 101) return 5;

    if (buf[KEY_INPUT_A] == 1) {
        shoot_left(); //Aを押したら射出
    }
    else {
        tmpl = 0;
        Ml = 0;
    }
    if (buf[KEY_INPUT_D] == 1) {
        shoot_right(); //Dを押したら射出
    }
    else {
        tmpr = 0;
        Mr = 0;
    }

    if (buf[KEY_INPUT_ESCAPE] == 1) return 9; //ESCが押されたら終了

    return 3;
}

int result_win(char buf[]) { // gamemode 4
    SetFontSize(72);
    DrawString(170, 150, "You Win!", GetColor(255, 255, 255));
    SetFontSize(24);
    DrawFormatString(190, 400, GetColor(255, 255, 255), "YOUR SCORE：%d", score);
    DrawString(80, 550, "続ける場合はENTERを，", GetColor(255, 255, 255));
    DrawString(80, 600, "終わりにする場合はESCを押してください．", GetColor(255, 255, 255));

    if (buf[KEY_INPUT_RETURN] == 1) {
        gameInit();
        return 1; // gamemode select
    }
    if (buf[KEY_INPUT_ESCAPE] == 1) return 9;
    return 4;
}

int result_lose(char buf[]) { // gamemode 5
    SetFontSize(72);
    DrawString(160, 150, "You Lose!", GetColor(255, 255, 255));
    SetFontSize(24);
    DrawFormatString(190, 400, GetColor(255, 255, 255), "YOUR SCORE：%d", score);
    DrawString(80, 550, "続ける場合はENTERを，", GetColor(255, 255, 255));
    DrawString(80, 600, "終わりにする場合はESCを押してください．", GetColor(255, 255, 255));

    if (buf[KEY_INPUT_RETURN] == 1) {
        gameInit();
        return 1; // gamemode select
    }
    if (buf[KEY_INPUT_ESCAPE] == 1) return 9;
    return 5;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    char gamemode = 0, buf[256];

    SetMainWindowText("Shooting Game with Math");
    ChangeWindowMode(TRUE); //ウィンドウモードで起動
    SetGraphMode(WIDTH, HEIGHT, 32); //画面の解像度指定
    SetWindowSizeChangeEnableFlag(FALSE); //画面サイズ変更不可
    SetDrawScreen(DX_SCREEN_BACK); //描画先を裏画面に

    if (DxLib_Init() == -1) { //DxLibのスタート
        return -1;
    }

    LoadData();
    gameInit();

    while (ProcessMessage() == 0 && gamemode != 9) {
        //画面全体を黒で塗りつぶし
        DrawBoxAA(0, 0, WIDTH, HEIGHT, GetColor(0, 0, 0), TRUE);

        GetHitKeyStateAll(buf); //キー入力取り込み

        switch (gamemode) {
        case 1:
            gamemode = select(buf);
            break;
        case 2:
            gamemode = rule(buf);
            break;
        case 3:
            gamemode = game(buf);
            break;
        case 4:
            gamemode = result_win(buf);
            break;
        case 5:
            gamemode = result_lose(buf);
            break;
        default:
            gamemode = title(buf);
        }

        ScreenFlip(); //画面裏返し
    }
}