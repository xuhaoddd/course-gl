#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//描画のための点の数
#define N 100

//曲線上の x 座標 と y 座標
float px[N];
float py[N];

//制御点の座標
float cx[4] = {-0.75, -0.5, 0.5,  0.75};
float cy[4] = {-0.5,   0.5,  0.5,  -0.5};

//座標を計算する関数
void generatePoints(){
  for(int i=0; i<N; i++){
    //課題2-1: 以下を埋めて3次のベジエ曲線上の点を px[i] と py[i] へ値を代入する
    //t やB0,B1,B2,B3 までの値は変数へ一度代入したほうが楽
    //float B0 = ??; などとする。
	  float t = (float)i / (N - 1); //バラメータ t

	  float B0 = (1 - t) * (1 - t) * (1 - t);
	  float B1 = 3 * (1 - t) * (1 - t) * t;
	  float B2 = 3 * t * t * (1 - t);
	  float B3 = t * t * t;
	  px[i] = B0 * cx[0] + B1 * cx[1] + B2 * cx[2] + B3 * cx[3];
	  py[i] = B0 * cy[0] + B1 * cy[1] + B2 * cy[2] + B3 * cy[3];
  }
}

//折れ線を描く関数
void drawCurve(){
  glLineWidth(3); //太さ
  glColor3f(0,0,0); //色は黒
  
  //課題1-2 からコピーする : 繰り返しを使って線分を描いていく
  glBegin(GL_LINES);
  for (int i = 0; i < N - 1; i++) {
	  glVertex3f(px[i], py[i], 0);
	  glVertex3f(px[i + 1], py[i + 1], 0);
  }
  glEnd();

  //課題1-2 (おまけ): 点の色を変えて描いてみる
  glPointSize(3);
  glBegin(GL_POINTS);
  for (int i = 0; i < N; i++) {
	  if (i < N / 2)
		  glColor3f(1, 0, 2.0 * i / (N - 1));
	  else
		  glColor3f(1.0 - 2.0 * (i - N / 2) / (N - 1), 0, 1);
	  glVertex3f(px[i], py[i], 0);
  }
  glEnd();
}

void moveControlPoints(float x, float y){
  //課題2-2 : 制御点の移動をする
  
  int k = 0; //k は最も近い制御点の番号 (暫定的に 0 番となっている)
  //以下は (x,y) と (cx[0], cy[0]) の間の距離の2乗
  float min = (x-cx[0])*(x-cx[0]) + (y-cy[0])*(y-cy[0]);
  
  //課題は以下を埋める (他の制御点との距離を計って k を更新する)
  //for 文は使ってもつかわなくてもよい
  for (int i = 1; i < 4; i++) {
	  float d = (x - cx[i]) * (x - cx[i]) + (y - cy[i]) * (y - cy[i]);
	  if (d < min) {
		  min = d;
		  k = i;
	  }
  }
  //制御点の更新
  cx[k] = x;
  cy[k] = y;
}

//曲線上の法線ベクトル (長さは 1 とする)
float nx[N];
float ny[N];

void computeNormals(){
  for(int i=0; i<N; i++){
    //課題3-1: 以下を埋めて3次のベジエ曲線上の法線を nx[i] と ny[i] へ値を代入する
	  float t = (float)i / (N - 1);

	  //基底関数の微分
	  float dB0 = -3 * (1 - t) * (1 - t);
	  float dB1 = 3 * (1 - t) * (1 - 3 * t);
	  float dB2 = 3 * t * (2 - 3 * t);
	  float dB3 = 3 * t * t;

	  float dx = dB0 * cx[0] + dB1 * cx[1] + dB2 * cx[2] + dB3 * cx[3];
	  float dy = dB0 * cy[0] + dB1 * cy[1] + dB2 * cy[2] + dB3 * cy[3];

	  float l = sqrt(dx * dx + dy * dy);
	  //法線ベクトルは接線ベクトルを90度回転したベクトル
	  nx[i] = -dy / l;
	  ny[i] = dx / l;
  }
}

void drawOffsetCurves(){
  glLineWidth(1); //太さ
  glColor3f(0,1,1); //色は水色
  
  //課題3-2 と 3-3 : オフセット曲線を描く
  glBegin(GL_LINES);
  for (int j = -10; j <= 10; j++) {
	  if (j != 0) {
		  for (int i = 0; i < N - 1; i++) {
			  glVertex3f(px[i] + 0.1 * j * nx[i], py[i] + 0.1 * j * ny[i], 0);
			  glVertex3f(px[i + 1] + 0.1 * j * nx[i + 1], py[i + 1] + 0.1 * j * ny[i + 1], 0);
		  }
	  }
  }
  glEnd();
  
}

//曲率半径
float R[N];

void computeCurvatureRadius(){
  for(int i=0; i<N; i++){
    //課題4-1: 以下を埋めて3次のベジエ曲線上の曲率半径を R[i] へ値を代入する
	  float t = (float)i / (N - 1);

	  //基底関数の1階微分
	  float dB0 = -3 * (1 - t) * (1 - t);
	  float dB1 = 3 * (1 - t) * (1 - 3 * t);
	  float dB2 = 3 * t * (2 - 3 * t);
	  float dB3 = 3 * t * t;

	  float dx = dB0 * cx[0] + dB1 * cx[1] + dB2 * cx[2] + dB3 * cx[3];
	  float dy = dB0 * cy[0] + dB1 * cy[1] + dB2 * cy[2] + dB3 * cy[3];

	  //基底関数の2階微分
	  float ddB0 = 6 * (1 - t);
	  float ddB1 = 6 * (3 * t - 2);
	  float ddB2 = 6 * (1 - 3 * t);
	  float ddB3 = 6 * t;

	  float ddx = ddB0 * cx[0] + ddB1 * cx[1] + ddB2 * cx[2] + ddB3 * cx[3];
	  float ddy = ddB0 * cy[0] + ddB1 * cy[1] + ddB2 * cy[2] + ddB3 * cy[3];

	  R[i] = pow(dx * dx + dy * dy, 1.5f) / (dx * ddy - dy * ddx);
  }
}

//縮閉線を描く関数
void drawEvolue(){
  glLineWidth(2); //太さ
  glColor3f(1,0,0); //色は赤色
  
  //課題4-2 : 縮閉線を描く
  /*
  glBegin(GL_LINES);
  for (int i = 0; i < N - 1; i++) {
	  if (R[i] * R[i + 1] > 0) { //曲率の符号が同じ時のみ描く
		  glVertex3f(px[i] + R[i] * nx[i], py[i] + R[i] * ny[i], 0);
		  glVertex3f(px[i + 1] + R[i + 1] * nx[i + 1], py[i + 1] + R[i + 1] * ny[i + 1], 0);
	  }
	  */
  glBegin(GL_LINES);
	  for (int i = 0; i < N; i++) {
		  glVertex3f(px[i], py[i], 0);
		  glVertex3f(px[i] + R[i] * nx[i], py[i] + R[i] * ny[i], 0);
	  }
  glEnd();
}

//レンダリングの関数
void display(void){ 
  //スクリーンと変換行列を初期化
  glClear(GL_COLOR_BUFFER_BIT); 
  glLoadIdentity();
  
  //座標軸を描く
  glLineWidth(1); 
  glColor3f(0,0,1); 
  glBegin(GL_LINES);
    glVertex3f(-1, 0, 0); glVertex3f( 1, 0, 0);
    glVertex3f(0, -1, 0); glVertex3f(0,  1, 0);
  glEnd();
  
  //制御点を描く
  glLineWidth(2); 
  glColor3f(0,1,0); 
  glBegin(GL_LINE_STRIP);
    glVertex3f(cx[0], cy[0], 0);
    glVertex3f(cx[1], cy[1], 0);
    glVertex3f(cx[2], cy[2], 0);
    glVertex3f(cx[3], cy[3], 0);
  glEnd();
  
  glPointSize(10);
  glBegin(GL_POINTS);
    glVertex3f(cx[0], cy[0], 0);
    glVertex3f(cx[1], cy[1], 0);
    glVertex3f(cx[2], cy[2], 0);
    glVertex3f(cx[3], cy[3], 0);
  glEnd();

  //曲線を描く
  drawCurve();
  
  //オフセットを描く
  drawOffsetCurves();
  
  //縮閉線を描く
  drawEvolue();
  
  glFlush(); //スクリーンへ表示
} 

//ウィンドウの大きさが変わった時に呼ばれる関数
int width, height;
void myReshape(int w, int h){
  width = w;
  height = h;
  
  glViewport(0, 0, w, h); //スクリーンの大きさを決める
  
  //変換行列を設定
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  
  //平行投影のためのパラメータを設定
  //見える領域は (-1,-1,-1) と (1,1,1) をコーナーに持つ立方体の中
  glOrtho(-1, 1, //左右
          -1, 1, //下上
          -1, 1 //手前奥
          );
  
  glMatrixMode(GL_MODELVIEW); //図形描画モードへ切り替え
} 

void myinit(){
  //背景の色を設定 (４番目は透過度で通常は不透明なので 1 を設定)
  glClearColor (1, 1, 1, 1);

}

//マウス操作の関数
void mouse(int btn, int state, int x, int y){
  moveControlPoints(2*(float)x/width-1, 2*(float)(height-y)/height-1);
  
  //曲線上の点や微分値を再計算する
  generatePoints();
  computeNormals();
  computeCurvatureRadius();
  
  //再描画
  display();
}

int main(int argc, char** argv){
  //曲線の点や微分値を計算する
  generatePoints();
  computeNormals();
  computeCurvatureRadius();
  
  //ウィンドウの初期化
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //使用するバッファの設定
  glutInitWindowSize(500, 500); //ウィンドウの大きさ
  glutCreateWindow("1-1"); //ウィンドウのタイトル
  
  myinit();//OpenGL の初期化
  glutReshapeFunc(myReshape);//ウィンドウサイズが変更された時に呼ばれる関数を設定
  glutMouseFunc(mouse); //マウスが操作されたときに呼ばれる関数を設定
  glutDisplayFunc(display); //ウィンドウの更新のための関数を設定
  glutMainLoop(); //メインループ
  
  return 0;
}

