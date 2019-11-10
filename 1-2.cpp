#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//レンダリングの関数
void display(void){ 
  //スクリーンと変換行列を初期化
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();
  
  //課題1と2のアフィン変換はここに書く
  //課題2ではスタックを利用すると楽
  
  //ティーポットをレンダリング (0.5は大きさ)
  glutSolidTeapot(0.5);
  
  glFlush(); //スクリーンへ表示
} 

//ウィンドウの大きさが変わった時に呼ばれる関数
void myReshape(int w, int h){
  glViewport(0, 0, w, h); //スクリーンの大きさを決める
  
  //変換行列を設定
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  
  //平行投影のためのパラメータを設定
  //課題5の透視変換は以下を置き換える
  if (w < h) 
    glOrtho(-1.0, 1.0, - h/w, h/w, -10.0, 10.0); 
  else 
    glOrtho(-w/h, w/h, -1.0, 1.0, -10.0, 10.0);
  
  glMatrixMode(GL_MODELVIEW); //図形描画モードへ切り替え
} 

//OpenGL の初期化
void myinit(){
  //光源の特性
  //課題4の光源の追加は以下をコピーしてライトの1番をONにする
  GLfloat light_ambient[]={1.0, 1.0, 1.0, 1.0};
  GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
  
  //光源の特性を OpenGL へ送る
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  
  glEnable(GL_LIGHTING); //ライトを使う
  glEnable(GL_LIGHT0);   //ライト0番をオン
  
  //オブジェクトの反射特性
  //課題３の反射特性は以下の値を変更する
  GLfloat mat_specular[]={0.5, 0.5, 0.5, 1.0};
  GLfloat mat_diffuse[]={0.5, 0.5, 1.0, 1.0};
  GLfloat mat_ambient[]={0.1, 0.1, 0.2, 1.0}; 
  GLfloat mat_shininess=20.0;
  
  //オブジェクトの反射特性を OpenGL へ送る
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
  
  //その他の設定
  glShadeModel(GL_FLAT);   //ポリゴンベタ塗り
  glEnable(GL_DEPTH_TEST); //陰面消去を行う
  
  glClearColor (1.0, 1.0, 1.0, 1.0); //背景の色
}

int main(int argc, char** argv){
  //ウィンドウの初期化
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //使用するバッファの設定
  glutInitWindowSize(500, 500); //ウィンドウの大きさ
  glutCreateWindow("1-2"); //ウィンドウのタイトル
  
  myinit();//OpenGL の初期化
  glutReshapeFunc(myReshape);//ウィンドウサイズが変更された時に呼ばれる関数を設定
  glutDisplayFunc(display); //ウィンドウの更新のための関数を設定
  glutMainLoop(); //メインループ
  
  return 0;
}

