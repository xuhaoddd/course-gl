#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

#define PI 3.1415926535897932384626433832795

#define N 50 //点の数
float px[N][N]; //x 座標
float py[N][N]; //y 座標
float pz[N][N]; //z 座標

//曲面上の点を計算する関数
void generatePoints(){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      float u = 2*PI*(float)i/(N-1); //u: 0～2π
      float v = 2*PI*(float)j/(N-1); //v: 0～2π
      
      //回転面で描くので断面の円をまず計算
      //中心 (3,0,0) で半径 1 の円
      float cx = 3 + cos(v);
      float cy = 0;
      float cz = sin(v);
      //z の軸の周りに回転
      px[i][j] = cos(u)*cx - sin(u)*cy;
      py[i][j] = sin(u)*cx + cos(u)*cy;
      pz[i][j] = cz;
      
      //もちろん以下のように計算後の式でもよい
      //px[i][j] = cos(u)*(3+cos(v));
      //py[i][j] = sin(u)*(3+cos(v));
      //pz[i][j] = sin(v);
      
      /* //課題 1-2 : 以下の式を円環に変える
      float u = 2*PI*(float)i/(N-1); //u は、経度なので 0～2π
      float v = PI*((float)j/(N-1)-0.5); //v は緯度なので -0.5π～0.5π
      px[i][j] = cos(u)*cos(v);
      py[i][j] = sin(u)*cos(v);
      pz[i][j] = sin(v);
      */
    }
  }
}

//曲面をワイヤフレームで描く関数
void drawWire(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_LIGHTING); //ライトを OFF
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(1);
  
  for(int i=0; i<N-1; i++){
    for(int j=0; j<N-1; j++){
      //課題 1-1 : 以下で OpenGL の命令で四辺形を描く
      glBegin(GL_POLYGON);
      glVertex3f(px[i  ][j  ], py[i  ][j  ], pz[i]  [j  ]);
      glVertex3f(px[i  ][j+1], py[i  ][j+1], pz[i  ][j+1]);
      glVertex3f(px[i+1][j+1], py[i+1][j+1], pz[i+1][j+1]);
      glVertex3f(px[i+1][j  ], py[i+1][j  ], pz[i+1][j  ]);
      glEnd();
    }
  }
}

float nx[N][N]; //法線ベクトルの x 成分
float ny[N][N]; //法線ベクトルの y 成分
float nz[N][N]; //法線ベクトルの z 成分

//法線ベクトルを計算する関数
void computeNormals(){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      float u = 2*PI*(float)i/(N-1); //u: 0～2π
      float v = 2*PI*(float)j/(N-1); //v: 0～2π
      
      //u 方向の微分
      float ux = -sin(u)*(3+cos(v));
      float uy = cos(u)*(3+cos(v));
      float uz = 0;
      
      //v 方向の微分
      float vx = -sin(v)*cos(u);
      float vy = -sin(v)*sin(u);
      float vz = cos(v);      
      
      /* //課題 2-2: 以下の式を円環の式の偏微分に変える
      float u = 2*3.1412*(float)i/(N-1); //u は、経度なので 0～2π
      float v = 3.1412*((float)j/(N-1)-0.5); //v は緯度なので -0.5π～0.5π
      
      //u 方向の微分
      float ux = -sin(u)*cos(v);
      float uy = cos(u)*cos(v);
      float uz = 0;
      
      //v 方向の微分
      float vx = -cos(u)*sin(v);
      float vy = -sin(u)*sin(v);
      float vz = cos(v);
      */
      
      //外積を計算
      float cx = uy*vz - uz*vy;
      float cy = uz*vx - ux*vz;
      float cz = ux*vy - uy*vx;
      
      //外積の長さ
      float l = sqrt(cx*cx + cy*cy + cz*cz);
      
      nx[i][j] = cx/l;
      ny[i][j] = cy/l;
      nz[i][j] = cz/l;
    }
  }
}

//曲面をスムースシェーディングで描く関数
void drawSurface(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glShadeModel(GL_SMOOTH); 
  glEnable(GL_LIGHTING);
  
  for(int i=0; i<N-1; i++){
    for(int j=0; j<N-1; j++){
      //課題 2-1: 各頂点で法線を指定しながら四辺形を描く
      glBegin(GL_POLYGON);
      glNormal3f(nx[i  ][j  ], ny[i  ][j  ], nz[i  ][j  ]);
      glVertex3f(px[i  ][j  ], py[i  ][j  ], pz[i  ][j  ]);
      glNormal3f(nx[i  ][j+1], ny[i  ][j+1], nz[i  ][j+1]);
      glVertex3f(px[i  ][j+1], py[i  ][j+1], pz[i  ][j+1]);
      glNormal3f(nx[i+1][j+1], ny[i+1][j+1], nz[i+1][j+1]);
      glVertex3f(px[i+1][j+1], py[i+1][j+1], pz[i+1][j+1]);
      glNormal3f(nx[i+1][j  ], ny[i+1][j  ], nz[i+1][j  ]);
      glVertex3f(px[i+1][j  ], py[i+1][j  ], pz[i+1][j  ]);
      glEnd();
    }
  }
}

/************ この下は課題には関係のない関数 (みなくてよい) *****************/

//変数いろいろ
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire;
int width, height;

void display(void){ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();
  
  glTranslatef(shiftX, shiftY, 0);
  float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
  if(s != 0)
    glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
  
  //レンダリング
  if(wire)
    drawWire();
  else
    drawSurface();
  
  glDisable(GL_LIGHTING); //ライトを OFF
  glLineWidth(5);
  glBegin(GL_LINES);
  glColor3f(1,0,0);
  glVertex3f(0,0,0);
  glVertex3f(3,0,0);
  glColor3f(0,1,0);
  glVertex3f(0,0,0);
  glVertex3f(0,3,0);
  glColor3f(0,0,1);
  glVertex3f(0,0,0);
  glVertex3f(0,0,3);
  glEnd();
  
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

  float light_pos[] = {0,0,-1,0};
  float light_dir[] = {0,0,1};
  glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_dir);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  
  /* define material proerties for front face of all polygons */
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  
  glShadeModel(GL_SMOOTH);   /* smooth shading */
  glEnable(GL_LIGHTING); /* enable lighting */
  glEnable(GL_LIGHT0);   /* enable light 0 */
  glEnable(GL_LIGHT1);
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
    glOrtho(-2.0/zoom, 2.0/zoom,
            -2.0 * (GLfloat) h / (GLfloat) w /zoom, 
            2.0 * (GLfloat) h / (GLfloat) w /zoom,
            -10.0, 10.0); 
  else 
    glOrtho(-2.0 * (GLfloat) w / (GLfloat) h / zoom, 
            2.0 * (GLfloat) w / (GLfloat) h /zoom,
            -2.0/zoom, 2.0/zoom, -10.0, 10.0); 
  glMatrixMode(GL_MODELVIEW); 
} 

//マウス操作
void mouse(int btn, int state, int x, int y){ 
  if(state == GLUT_DOWN){
    mouseX = x;
    mouseY = y;
  }
  else if(btn==GLUT_LEFT_BUTTON){
    float mx = -0.0025f*(x - mouseX)/zoom;
    float my = 0.0025f*(y - mouseY)/zoom;
    
    //回転
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
  else if(btn==GLUT_RIGHT_BUTTON){ //拡大縮小
    zoom -= 0.0025f*(y - mouseY);
    if(zoom > 20.0f) zoom = 20.0f;
    else if(zoom < 0.05f) zoom = 0.05f;
    myReshape(width, height);
    display();
  }
  else if(btn==GLUT_MIDDLE_BUTTON){ //平行移動
    shiftX += 0.0025f*(x - mouseX)/zoom;
    shiftY += 0.0025f*(y - mouseY)/zoom;
    display();
  }
} 

//キーが押された時に対応する関数を呼び出す
void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 'w':
    printf("ワイヤー表示切り替え\n");
    wire = !wire;
    display();
    break;
  }
}

/**********************理解しなくとよいところここまで***********************/

int main(int argc, char** argv){
  generatePoints(); //曲面上の点を生成する
  computeNormals(); //法線を計算する
  
  //回転などの初期化
  qw = 1;
  qx = qy =  qz = 0;
  zoom = 1;
  shiftX = shiftY = 0;
  wire = true;
  
  //ウィンドウの初期化
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //使用するバッファの設定
  glutInitWindowSize(500, 500); //ウィンドウの大きさ
  glutCreateWindow("prog3-1"); //ウィンドウのタイトル
  
  myinit(); //OpenGL の初期化
  glutReshapeFunc(myReshape); //ウィンドウサイズが変更された時に呼ばれる関数を設定
  glutDisplayFunc(display);  //ウィンドウの更新のための関数を設定
  glutMouseFunc(mouse); //マウスが操作されたときに呼ばれる関数を設定
  glutKeyboardFunc(keyboard);//キーボードが押されたときに呼ばれる関数を設定
  
  glutMainLoop(); //メインループ
  
  return 0;
}

