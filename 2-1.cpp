#include <stdio.h>
#include <math.h>
#include <GL/glut.H>


//データ
int N; //データの数
float f[1000]; //データを格納する配列 f[i] で i 番目にアクセス (とりあえず十分大きな長さで、必要なところだけ使う)
float h; //データの間隔

int edge[1000];//エッジを描くためのデータ (1 のときエッジ)
int convex[1000]; //凹凸の描くためのデータ (1のとき凸，0なら凹, -1 は分からない)

//課題1-1 (折れ線で描く)
void drawData(){
  //直線
  glLineWidth(3); //線は少し細め
  glColor3f(0.8,1,0.8); //色は緑 (変えてもよい)
  //以下を埋める
  //for文を使って直線を立てていく
  for (int i = 0; i < N; i++)
  {
	  printf("%f ",f[i]);
  }
  glBegin(GL_LINES);
  float j = 0;
  for (int i = 0; i < N; i++)
  {
	  glVertex3f(j , f[i], 0);
	  glVertex3f(j + h, f[i+1], 0);
	  j = j + h;
  }
  glEnd();
  
  //折れ線
  glLineWidth(3);
  glColor3f(0,0,0); //色は黒 (変えてもよい)
  //以下を埋める
  //for文を使って折れ線を描いていく
  glBegin(GL_LINES);
  float j1 = h;
  for (int i = 0; i < N-2; i++)
  {
	  glVertex3f(j1, 0, 0);
	  glVertex3f(j1 , f[i + 1], 0);
	  j1 = j1 + h;
  }
  glEnd();

  
}


//課題3-1 (エッジを見つける)
void detectEdge(){
  //1階微分値を計算して、絶対値が適当な値より大きければ
  //edge[i] へ 1 を代入する
  
  
}


//課題3-2 (エッジを描く)
void drawEdge(){
  //for で edge[i] を調べて、1 だったら点を描く
  glPointSize(5);
  glColor3f(1,0,0); //色は赤 (変えてもよい)
  
  
}


//課題4-1 (平滑化)
void smooth(){
  float tmp[1000]; //新しい値を一時的に代入するための配列
  
  //for 文で左右の値の平均値を temp[i] へ代入していき、
  //終わったら 新たな for 文で tmp[i] を f[i] へ代入する
  
  
}


//課題5-1 (エッジ保存平滑化)
void adaptiveSmooth(){
  float tmp[1000]; //新しい値を一時的に代入するための配列
  
  //for 文で左右の値の重み付き平均値を temp[i] へ代入していき、
  //終わったら 新たな for 文で tmp[i] を f[i] へ代入する
  float s = 1/(100*h*h); //パラメータ
  
  
}


//課題6-1 と 課題6-2 (Canny エッジ検出)
void CannyEdgeDetector(){
  float d[1000]; //1階微分の大きさを代入する配列
  
  //まず for で d[i] へ1階微分の大きさを代入する
  //そして, 新しい for でエッジの判定をする。(エッジなら edge[i] へ 1 を代入)
  
  
}

//課題7-1 (凹凸の判定)
void segment(){
  //2階微分値を計算して、
  //正なら convex[i] へ 1 を代入
  //負なら convex[i] へ 0 を代入
  //もしもゼロなら convex[i] へ -1 を代入
  
  
}


//課題7-2 (凹凸の描画)
void drawSegment(){
  //for で convex[i] を調べて、0 なら黄色、1なら水色の点を描く
  glPointSize(5);
  
  
}

/************ この下は課題には関係のない関数 (みなくてよい) *****************/

//レンダリングの関数
void display(void){ 
  //スクリーンと変換行列を初期化
  glClear(GL_COLOR_BUFFER_BIT); 
  glLoadIdentity();
  
  drawData();//データを描く
  drawEdge();//エッジを描く
  drawSegment();
  
  glFlush(); //スクリーンを更新
}

//ファイルからデータを読み込む関数
void readValues(const char* file_name){
  FILE* in;
  fopen_s(&in, file_name, "r");
  
  fscanf_s(in, "%d", &N);
  for(int i=0; i<N; i++){
    fscanf_s(in, "%f", &f[i]);
    edge[i] = 0; //エッジでないようにしておく
    convex[i] = -1; //凹凸は不明にしておく
  }
  
  h = 1.0/(N-1); //(h は N個目のデータを書いたときに 1 になるようにする)
  fclose(in);
}

//キーボード操作で対応する関数を呼び出す
void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 's':
    printf("平滑化\n");
    smooth();
    display();
    break;
  case 'a':
    printf("エッジ保存平滑化\n");
    adaptiveSmooth();
    display();
    break;
  case 'e':
    printf("エッジ抽出\n");
    detectEdge();
    display();
    break;
  case 'n':
    printf("Canny抽出\n");
    CannyEdgeDetector();
    display();
    break;
  case 'h':
    printf("凹凸\n");
    segment();
    display();
    break;
  }
}

//ウィンドウの大きさが変わった時に呼ばれる関数
void myReshape(int w, int h){
  glViewport(0, 0, w, h); //スクリーンの大きさを決める
  
  //変換行列を設定
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  
  //平行投影のためのパラメータを設定
  //見える領域は (0,0,-1) と (1,1,1) をコーナーに持つ直方体の中
  glOrtho(0, 1, //左右
          0, 1, //下上
          -1, 1 //手前奥
          );
  
  glMatrixMode(GL_MODELVIEW); //図形描画モードへ切り替え
} 

void myinit(){
  //背景の色を設定 (４番目は透過度で通常は不透明なので 1 を設定)
  glClearColor (1, 1, 1, 1);

}

/**********************理解しなくとよいところここまで***********************/

int main(int argc, char** argv){
  //課題1-2
  //読み込めるデータは "noisy", "wave", "cubic"
  readValues("data/noisy");
  //課題 7 は wave を使う
  
  //ウィンドウの初期化
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //使用するバッファの設定
  glutInitWindowSize(500, 500); //ウィンドウの大きさ
  glutCreateWindow("prog2-1"); //ウィンドウのタイトル
  
  myinit();//OpenGL の初期化
  glutReshapeFunc(myReshape);//ウィンドウサイズが変更された時に呼ばれる関数を設定
  glutDisplayFunc(display); //ウィンドウの更新のための関数を設定
  glutKeyboardFunc(keyboard); //キーボードが押されたときに呼ばれる関数を設定
  glutMainLoop(); //メインループ
  
  return 0;
}

