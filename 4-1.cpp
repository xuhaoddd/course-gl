#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//点の数
#define N 21
//x 座標
float px[N];
//y 座標
float py[N];

//座標を計算する関数
void generatePoints(){
  for(int i=0; i<N; i++){
    //課題1-1: 以下を埋めて配列 px[i] と py[i] へ値を代入する
    //float t = ??; などとすると楽
	  float element = (float)i / (N-1);
	  px[i] = (-4) * pow(element, 2) + 4 *  element - 0.5;
	  py[i] = 4 * pow( element, 3) - 8 * pow( element, 2) + 3  * element + 0.5;
  }
}

//折れ線を描く関数
void drawCurve(){
  glLineWidth(3); //太さ
  glColor3f(0,0,0); //色は黒

  glBegin(GL_LINES);
  for (int i = 0; i < N-1; i++) {
	  glVertex3f(px[i], py[i], 0);
	  glVertex3f(px[i + 1], py[i + 1], 0);
  }
  glEnd();
  
  //課題1-2: 繰り返しを使って線分を描いていく
  //以下を埋める (前の課題とほぼ同じ)
  glPointSize(10);
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

//レンダリングの関数
void display(void){ 
  //スクリーンと変換行列を初期化
  glClear(GL_COLOR_BUFFER_BIT); 
  glLoadIdentity();
  
  //座標軸を描く
  glLineWidth(1); 
  glColor3f(0,0,1); 
  glBegin(GL_LINES);
    glVertex3f(-1, 0, 0);
    glVertex3f( 1, 0, 0);
    glVertex3f(0, -1, 0);
    glVertex3f(0,  1, 0);
  glEnd();
  
  //曲線を描く
  drawCurve();
  
  glFlush(); //スクリーンへ表示
} 

//ウィンドウの大きさが変わった時に呼ばれる関数
void myReshape(int w, int h){
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

int main(int argc, char** argv){
  generatePoints();
  
  //ウィンドウの初期化
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //使用するバッファの設定
  glutInitWindowSize(500, 500); //ウィンドウの大きさ
  glutCreateWindow("1-1"); //ウィンドウのタイトル
  
  
  myinit();//OpenGL の初期化
  glutReshapeFunc(myReshape);//ウィンドウサイズが変更された時に呼ばれる関数を設定
  glutDisplayFunc(display); //ウィンドウの更新のための関数を設定
  glutMainLoop(); //メインループ
  
  return 0;
}

