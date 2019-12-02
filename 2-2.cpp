#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//データ
int W, H; //横と縦のデータ数
float f[1000][1000]; //値の2次配列. 適当に大きな領域をとっている
float h; //データの縦横の間隔

//以下は計算のために必要な配列
int edge[1000][1000]; //エッジを描くためのデータ (1 のときエッジ)
int convex[1000][1000]; //凹凸の描くためのデータ (1のとき凸，0なら凹, 2なら鞍型．-1 は分からない)

//以下は課題6-3のため
float tmp[1000][1000]; //平滑化のための一時記憶領域
float dx[1000][1000]; //Canny エッジ検出のための一時記憶領域 (x方向の微分)
float dy[1000][1000]; //Canny エッジ検出のための一時記憶領域 (y方向の微分)
float gl[1000][1000]; //Canny エッジ検出のための一時記憶領域 (勾配の長さ)

float maxVal = 0.25; //高さをスケールする値で見た目だけのものです。見かけがでこぼこすぎると感じたら小さく変更


//課題2-3 (上下反転)
void flipUpDown() {
	//2重ループで上下を反転する。
	//データは0～1までとする。つまり 0→1, 1→0 となればよい
	//f[j][i] は (j, i) に対応することに十分注意 !!!

	for (int i = 0; i < 256; i++)
	{
		for (int j = 0;  j< 256; j++)
			f[i][j] = 1.0 - f[i][j];
	}
	printf("%d %d", H,W);
	/*
	printf("%d",H);
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			f[j][i] = 1.0 - f[j][i];
		}
	}
	*/
}


//課題3-3 (エッジ抽出)
void detectEdge(){
  //勾配を計算して、長さが適当な値より大きければ
  //edge[i] へ 1 を代入する
	for (int j = 1; j < H-1; j++) {
		for (int i = 1; i < W - 1; i++) {
			float diff_x = (f[j][i + 1] - f[j][i - 1]) / (2 * h);
			float diff_y = (f[j+1][i] - f[j-1][i]) / (2 * h);
			float gra = sqrt(pow(diff_x, 2) + pow(diff_y, 2));
			if (gra > 10.0) {
				edge[j][i] = 1;
				}
			else {
				edge[j][i] = 0;	
			}
		}
  }
  
}


//課題4-2 (平滑化)
void smooth(){
  //for 文で上下左右の値の平均値を temp[j][i] へ代入していき、
  //終わったら 新たな for 文で tmp[j][i] を f[j][i] へ代入する
	for (int j = 1; j < H - 1; j++) {
		for (int i = 1; i < W - 1; i++) {
			tmp[j][i] = (f[j - 1][i] + f[j + 1][i] + f[j][i - 1] + f[j][i + 1]) / 4.0;
		}
	}
	for (int j = 1; j < H - 1; j++) {
		for (int i = 1; i < W - 1; i++) {
			f[j][i] = tmp[j][i];
		}
	}
}

float weight(float dif) {
	return 1.0 / (1 + pow(1 / ( h * h), 2) * dif * dif);
}

//課題5-2 (エッジ保存平滑化)
void adaptiveSmooth(){
  //for 文で上下左右の値の平均値を temp[j][i] へ代入していき、
  //終わったら 新たな for 文で tmp[j][i] を f[j][i] へ代入する
  float s = 1/(h*h); //パラメータ
  for (int j = 1; j < H - 1; j++) {
	  for (int i = 1; i < W - 1; i++) {
		  tmp[j][i] = (weight(f[j - 1][i] - f[j][i]) * f[j - 1][i]
			  + weight(f[j + 1][i] - f[j][i]) * f[j + 1][i]
			  + weight(f[j][i - 1] - f[j][i]) * f[j][i - 1]
			  + weight(f[j][i + 1] - f[j][i]) * f[j][i + 1])
			  / ((weight(f[j - 1][i] - f[j][i]))
				  + weight(f[j + 1][i] - f[j][i])
				  + weight(f[j][i - 1] - f[j][i])
				  + weight(f[j][i + 1] - f[j][i]));
	  }
  }
  for (int j = 1; j < H - 1; j++) {
	  for (int i = 1; i < W - 1; i++) {
		  f[j][i] = tmp[j][i];
	  }
  }
  
}


//課題6-3 と 課題6-4 (Canny エッジ検出)
void CannyEdgeDetector(){
  //まず1回目のループで dx[j][i], dy[j][i] へ勾配ベクトルを代入。dl[i][j] へ長さを代入
  //そして, 2回目のループでエッジの判定をする。(エッジなら edge[i] へ 1 を代入)
	for (int j = 1; j < H - 1; j++)
		for (int i = 1; i < W - 1; i++) {
			dx[j][i] = (f[j][i + 1] - f[j][i - 1]) / (2.0 * h);
			dy[j][i] = (f[j + 1][i] - f[j - 1][i]) / (2.0 * h);
			gl[j][i] = sqrt(dx[j][i] * dx[j][i] + dy[j][i] * dy[j][i]);
		}

	for (int j = 2; j < H - 2; j++)
		for (int i = 2; i < W - 2; i++) {
			float a = dy[j][i] / dx[j][i]; //直線の傾き
			if (fabs(a) < 1) { //横方向で交わる
				if (a > 0) { //右は上、左は下
					if ((1 - a) * gl[j][i + 1] + a * gl[j + 1][i + 1] < gl[j][i] &&
						(1 - a) * gl[j][i - 1] + a * gl[j - 1][i - 1] < gl[j][i] &&
						10.0< gl[j][i])
						edge[j][i] = 1;
					else
						edge[j][i] = 0;
				}
				else { //右は下、左は上
					if ((1 + a) * gl[j][i + 1] - a * gl[j - 1][i + 1] < gl[j][i] &&
						(1 + a) * gl[j][i - 1] - a * gl[j + 1][i - 1] < gl[j][i] &&
						10.0 < gl[j][i])
						edge[j][i] = 1;
					else
						edge[j][i] = 0;
				}
			}
			else { //縦方向で交わる
				a = dx[j][i] / dy[j][i];
				if (a > 0) { //上は右、下は左
					if ((1 - a) * gl[j + 1][i] + a * gl[j + 1][i + 1] < gl[j][i] &&
						(1 - a) * gl[j - 1][i] + a * gl[j - 1][i - 1] < gl[j][i] &&
						10.0 < gl[j][i])
						edge[j][i] = 1;
					else
						edge[j][i] = 0;
				}
				else { //上は左、下は右
					if ((1 + a) * gl[j + 1][i] - a * gl[j + 1][i - 1] < gl[j][i] &&
						(1 + a) * gl[j - 1][i] - a * gl[j - 1][i + 1] < gl[j][i] &&
						10.0 < gl[j][i])
						edge[j][i] = 1;
					else
						edge[j][i] = 0;
				}
			}
		}
}
  


//課題7-2 (凹凸の判定)
void segment(){
  //ヘシアンの トレース と 行列式を計算し、資料 62 ページの
  //①なら convex[j][i] へ 0 を代入
  //②なら convex[j][i] へ 1 を代入
  //③なら convex[j][i] へ 2 を代入
  //もしもその他なら convex[j][i] へ -1 を代入
	for (int j = 1; j < H - 1; j++) {
		for (int i = 1; i < W - 1; i++) {
			float xx = f[j][i + 1] + f[j][i - 1] - 2.0 * f[j][i];
			float yy = f[j + 1][i] + f[j - 1][i] - 2.0 * f[j][i];
			float xy = 0.25f * (f[j + 1][i + 1] + f[j - 1][i - 1] - f[j + 1][i - 1] - f[j - 1][i + 1]);
			if (xx * yy - xy * xy < 0)
				convex[j][i] = 2;
			else if (xx + yy > 0)
				convex[j][i] = 0;
			else if (xx + yy < 0)
				convex[j][i] = 1;
			else
				convex[j][i] = -1;
		}
	}

  
  
}

/************ この下は課題には関係のない関数 (みなくてよい) *****************/

//回転のための変数
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire;
bool color;
int width, height;

//ファイルから値を読み込む関数
void readValues(const char* file_name){
  
  FILE* in;
  fopen_s(&in, file_name, "r");
  
  fscanf_s(in, "%d", &W);
  fscanf_s(in, "%d", &H);
  
  for(int j=0; j<H; j++)
    for(int i=0; i<W; i++){
      fscanf_s(in, "%f", &f[j][i]);
      edge[j][i] = 0;
      convex[j][i] = -1;
    }
  
  fclose(in);
  
  h = 1.0/(W-1);
}

//四辺形の法線の計算する関数 (詳細はポリゴンメッシュの回で)
void newell(float n[3], float (*v)[3], int N){
  n[0] = n[1] = n[2] = 0;
  for(int i=0; i<N; i++){
    float* p = v[i];
    float* q = v[(i+1)%N];
    n[0] += (p[1]-q[1])*(p[2]+q[2]);
    n[1] += (p[2]-q[2])*(p[0]+q[0]);
    n[2] += (p[0]-q[0])*(p[1]+q[1]);
  }
  float l = (float)sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
  if(l != 0){
    n[0] /= l;
    n[1] /= l;
    n[2] /= l;
  }
}

//ライト効果を入れて高さを表示
void shading(){
  glPolygonMode(GL_FRONT, GL_FILL);
  glShadeModel(GL_FLAT);
  glEnable(GL_LIGHTING);
  
  for(int j=0; j<H-1; j++){
    for(int i=0; i<W-1; i++){
      float v[4][3] = {
        {i*h-0.5, j*h-0.5, maxVal*(f[j][i]-0.5)},
        {(i+1)*h-0.5, j*h-0.5, maxVal*(f[j][i+1]-0.5)},
        {(i+1)*h-0.5, (j+1)*h-0.5, maxVal*(f[j+1][i+1]-0.5)},
        {i*h-0.5, (j+1)*h-0.5, maxVal*(f[j+1][i]-0.5)}
      };
      float n[3];
      newell(n, v, 4);
      glBegin(GL_POLYGON);
        glNormal3fv(n);
        glVertex3fv(v[0]);
        glVertex3fv(v[1]);
        glVertex3fv(v[2]);
        glVertex3fv(v[3]);
      glEnd();
    }
  }
}

//高いほうを白で表示
void coloring(){
  glPolygonMode(GL_FRONT, GL_FILL);
  glDisable(GL_LIGHTING);
  
  for(int j=0; j<H-1; j++){
    for(int i=0; i<W-1; i++){
      glBegin(GL_POLYGON);
        float v = f[j][i];
        glColor3f(v, v, v);
        glVertex3f(i*h-0.5, j*h-0.5, maxVal*(v-0.5));
        
        v = f[j][i+1];
        glColor3f(v, v, v);
        glVertex3f((i+1)*h-0.5, j*h-0.5, maxVal*(v-0.5));
        
        v = f[j+1][i+1];
        glColor3f(v, v, v);
        glVertex3f((i+1)*h-0.5, (j+1)*h-0.5, maxVal*(v-0.5));
        
        v = f[j+1][i];
        glColor3f(v, v, v);
        glVertex3f(i*h-0.5, (j+1)*h-0.5, maxVal*(v-0.5));
      glEnd();
    }
  }
}

//エッジを赤い点として描く関数
void drawEdge(){
  glPolygonMode(GL_FRONT, GL_LINE);
  glDisable(GL_LIGHTING);
  
  glColor3f (1, 0, 0);
  glPointSize(5);
  
  glBegin(GL_POINTS);
  for(int j=0; j<H; j++){
    for(int i=0; i<W; i++){
      if(edge[j][i] == 1)
        glVertex3f(i*h-0.5, j*h-0.5, maxVal*(f[j][i]-0.5));
    }
  }
  glEnd();
}

//曲面の凹凸を点の色で描く関数
void drawSegment(){
  glPolygonMode(GL_FRONT, GL_LINE);
  glDisable(GL_LIGHTING);
  
  glPointSize(5);
  glBegin(GL_POINTS);
  for(int j=0; j<H; j++){
    for(int i=0; i<W; i++){
      if(convex[j][i] == -1)
        continue;
      
      if(convex[j][i] == 1)
        glColor3f(1, 1, 0);
      else if(convex[j][i] == 0)
        glColor3f(0, 1, 1);
      else if(convex[j][i] == 2)
        glColor3f(1, 0, 1);
      glVertex3f(i*h-0.5, j*h-0.5, maxVal*(f[j][i]-0.5));
    }
  }
  glEnd();
}

//隣接を表す線を黄色で表示
void drawWire(){
  glPolygonMode(GL_FRONT, GL_LINE);
  glDisable(GL_LIGHTING);
  
  glColor3f (1.0, 1.0, 0.0);
  glLineWidth(1.0f);
  
  for(int j=0; j<H-1; j++){
    for(int i=0; i<W-1; i++){
      glBegin(GL_POLYGON);
        glVertex3f(i*h-0.5, j*h-0.5, maxVal*(f[j][i]-0.5));
        glVertex3f(i*h-0.5, (j+1)*h-0.5, maxVal*(f[j+1][i]-0.5));
        glVertex3f((i+1)*h-0.5, (j+1)*h-0.5, maxVal*(f[j+1][i+1]-0.5));
        glVertex3f((i+1)*h-0.5, j*h-0.5, maxVal*(f[j][i+1]-0.5));
      glEnd();
    }
  }
}

void display(void){ 
  //スクリーンと変換行列を初期化
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();
  
  //回転などの変換
  glTranslatef(shiftX, shiftY, 0);
  float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
  if(s != 0)
    glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
  
  //データの描画
  if(color)
    coloring(); //データを描く (色)
  else
    shading(); //データを描く (高さ)
  
  if(wire)
    drawWire(); //線形補間の線
  
  drawEdge(); //データを描く
  drawSegment(); //エッジを描く
  
  //スクリーンを更新
  glFlush(); 
  glutSwapBuffers(); 
} 


void myinit(){
  GLfloat mat_specular[]={0.5, 0.5, 0.5, 1.0};
  GLfloat mat_diffuse[]={0.5, 0.5, 1.0, 1.0};
  GLfloat mat_ambient[]={0.5, 0.5, 0.5, 1.0}; 
  GLfloat mat_shininess={20.0};
  GLfloat light_ambient[]={0.1, 0.1, 0.1, 1.0};
  GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
  
  /* set up ambient, diffuse, and specular components for light 0 */
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  
  /* define material proerties for front face of all polygons */
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
  
  glShadeModel(GL_SMOOTH);   /* smooth shading */
  glEnable(GL_LIGHTING); /* enable lighting */
  glEnable(GL_LIGHT0);   /* enable light 0 */
  glEnable(GL_DEPTH_TEST); /* enable z buffer */
  glDepthFunc(GL_LEQUAL);
  
  glClearColor (1.0, 1.0, 1.0, 1.0);
}

void myReshape(int w, int h){
  width = w;
  height = h;
  
  glViewport(0, 0, w, h); 
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity(); 
  if (w <= h) 
    glOrtho(-0.5/zoom, 0.5/zoom,
            -0.5*h/w/zoom, 0.5*h/w/zoom,
            -100.0, 100.0); 
  else 
    glOrtho(-0.5*w/h/zoom, 0.5*w/h/zoom,
            -0.5/zoom, 0.5/zoom,
            -100.0, 100.0);
  gluLookAt(0,0,-10, 0,0,0, 0,-1,0);
  glMatrixMode(GL_MODELVIEW); 
} 

void mouse(int btn, int state, int x, int y){ 
  /* mouse callback, selects an axis about which to rotate */
  if(state == GLUT_DOWN){
    mouseX = x;
    mouseY = y;
  }
  else if(btn==GLUT_LEFT_BUTTON){
    float mx = -0.0025f*(x - mouseX)/zoom;
    float my = 0.0025f*(y - mouseY)/zoom;
    
    //rotation
    float c = (float)cos(my);
    float s = (float)sin(my);
    
    float rw = c*qw - s*qx;
    float rx = c*qx + s*qw;
    float ry = c*qy - s*qz;
    float rz = c*qz + s*qy;
    
    c = (float)cos(mx);
    s = (float)sin(mx);
    
    qw = c*rw - s*ry;
    qx = c*rx + s*rz;
    qy = c*ry + s*rw;
    qz = c*rz - s*rx;
    
    float n = (float)sqrt(qw*qw + qx*qx + qy*qy + qz*qz);
    if(n != 0){
      qw /= n;
      qx /= n;
      qy /= n;
      qz /= n;
    }
    else{
      qw = 1.0f;
      qx = qy = qz = 0.0f;
    }
    display();
  }
  else if(btn==GLUT_RIGHT_BUTTON){
    zoom -= 0.0025f*(y - mouseY);
    if(zoom > 20.0f) zoom = 20.0f;
    else if(zoom < 0.05f) zoom = 0.05f;
    myReshape(width, height);
    display();
  }
  else if(btn==GLUT_MIDDLE_BUTTON){
    shiftX += 0.0025f*(x - mouseX)/zoom;
    shiftY += 0.0025f*(y - mouseY)/zoom;
    display();
  }
} 

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 'w':
    wire = !wire;
    display();
    break;
  case 'c':
    color = !color;
    display();
    break;
  case 'f':
    printf("上下反転\n");
    flipUpDown();
    display();
    break;
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


/**********************理解しなくとよいところここまで***********************/

int main(int argc, char** argv){
  //課題 2-2 読み込めるデータは "lena", "wave", "engine", "range", "dump"
  readValues("data/lena");
  //課題7 は wave か dump を用いて行う
  
  //回転などの初期化
  qw = 1;
  qx = qy =  qz = 0;
  zoom = 1;
  shiftX = shiftY = 0;
  wire = false;
  color = true;
  
  //ウィンドウの初期化
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //使用するバッファの設定
  glutInitWindowSize(500, 500); //ウィンドウの大きさ
  glutCreateWindow("prog2-2"); //ウィンドウのタイトル
  
  myinit(); //OpenGL の初期化
  glutReshapeFunc(myReshape); //ウィンドウサイズが変更された時に呼ばれる関数を設定
  glutDisplayFunc(display);  //ウィンドウの更新のための関数を設定
  glutMouseFunc(mouse); //マウスが操作されたときに呼ばれる関数を設定
  glutKeyboardFunc(keyboard);//キーボードが押されたときに呼ばれる関数を設定
  
  glutMainLoop(); //メインループ
  
  return 0;
}

