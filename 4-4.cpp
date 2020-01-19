#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

#define PI 3.1415926535897932384626433832795

//制御点の x 座標
float cx[3][3] = { {-2,0,2}, {-2,0,2}, {-1,0,1} };
//制御点の y 座標
float cy[3][3] = { {-2,-2,-2}, {0,0,0}, {2,2,2} };
//制御点の z 座標
float cz[3][3] = { {-1,-2,-1}, {-1,1,-1}, {-1,0,1} };

#define N 30 //描画の点の数
float px[N][N]; //x 座標
float py[N][N]; //y 座標
float pz[N][N]; //z 座標

//曲面上の点を計算する関数
void generatePoints(){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      float u = (float)i/(N-1); //u: 0～1
      float v = (float)j/(N-1); //v: 0～1
      
      //課題 3-1: 2次のベジエ曲面を計算して
      //px[i][j], py[i][j], pz[i][j] へ曲面上の点を代入
      float B0u = (1-u)*(1-u);
      float B1u = 2*u*(1-u);
      float B2u = u*u;
      
      float B0v = (1-v)*(1-v);
      float B1v = 2*v*(1-v);
      float B2v = v*v;
      
      px[i][j] = B0u*B0v*cx[0][0] + B1u*B0v*cx[0][1] + B2u*B0v*cx[0][2] +
                 B0u*B1v*cx[1][0] + B1u*B1v*cx[1][1] + B2u*B1v*cx[1][2] +
                 B0u*B2v*cx[2][0] + B1u*B2v*cx[2][1] + B2u*B2v*cx[2][2];
      
      py[i][j] = B0u*B0v*cy[0][0] + B1u*B0v*cy[0][1] + B2u*B0v*cy[0][2] +
                 B0u*B1v*cy[1][0] + B1u*B1v*cy[1][1] + B2u*B1v*cy[1][2] +
                 B0u*B2v*cy[2][0] + B1u*B2v*cy[2][1] + B2u*B2v*cy[2][2];
      
      pz[i][j] = B0u*B0v*cz[0][0] + B1u*B0v*cz[0][1] + B2u*B0v*cz[0][2] +
                 B0u*B1v*cz[1][0] + B1u*B1v*cz[1][1] + B2u*B1v*cz[1][2] +
                 B0u*B2v*cz[2][0] + B1u*B2v*cz[2][1] + B2u*B2v*cz[2][2];
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
      //課題 3-1: 課題 1-1 からコピーする
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
      float u = (float)i/(N-1); //u: 0～1
      float v = (float)j/(N-1); //v: 0～1
      
      //課題 3-1: 2次のベジエ曲面の偏微分を計算して
      //nx[i][j], ny[i][j], nz[i][j] へ曲面上の単位法線ベクトルを代入
      float B0u = (1-u)*(1-u);
      float B1u = 2*u*(1-u);
      float B2u = u*u;
      
      float dB0u = -2*(1-u);
      float dB1u = 2*(1-2*u);
      float dB2u = 2*u;
      
      float B0v = (1-v)*(1-v);
      float B1v = 2*v*(1-v);
      float B2v = v*v;
      
      float dB0v = -2*(1-v);
      float dB1v = 2*(1-2*v);
      float dB2v = 2*v;
      
      //u 方向の微分
      float ux = dB0u*B0v*cx[0][0] + dB1u*B0v*cx[0][1] + dB2u*B0v*cx[0][2] +
                 dB0u*B1v*cx[1][0] + dB1u*B1v*cx[1][1] + dB2u*B1v*cx[1][2] +
                 dB0u*B2v*cx[2][0] + dB1u*B2v*cx[2][1] + dB2u*B2v*cx[2][2];
      
      float uy = dB0u*B0v*cy[0][0] + dB1u*B0v*cy[0][1] + dB2u*B0v*cy[0][2] +
                 dB0u*B1v*cy[1][0] + dB1u*B1v*cy[1][1] + dB2u*B1v*cy[1][2] +
                 dB0u*B2v*cy[2][0] + dB1u*B2v*cy[2][1] + dB2u*B2v*cy[2][2];
      
      float uz = dB0u*B0v*cz[0][0] + dB1u*B0v*cz[0][1] + dB2u*B0v*cz[0][2] +
                 dB0u*B1v*cz[1][0] + dB1u*B1v*cz[1][1] + dB2u*B1v*cz[1][2] +
                 dB0u*B2v*cz[2][0] + dB1u*B2v*cz[2][1] + dB2u*B2v*cz[2][2];
      
      //v 方向の微分
      float vx = B0u*dB0v*cx[0][0] + B1u*dB0v*cx[0][1] + B2u*dB0v*cx[0][2] +
                 B0u*dB1v*cx[1][0] + B1u*dB1v*cx[1][1] + B2u*dB1v*cx[1][2] +
                 B0u*dB2v*cx[2][0] + B1u*dB2v*cx[2][1] + B2u*dB2v*cx[2][2];
      
      float vy = B0u*dB0v*cy[0][0] + B1u*dB0v*cy[0][1] + B2u*dB0v*cy[0][2] +
                 B0u*dB1v*cy[1][0] + B1u*dB1v*cy[1][1] + B2u*dB1v*cy[1][2] +
                 B0u*dB2v*cy[2][0] + B1u*dB2v*cy[2][1] + B2u*dB2v*cy[2][2];
      
      float vz = B0u*dB0v*cz[0][0] + B1u*dB0v*cz[0][1] + B2u*dB0v*cz[0][2] +
                 B0u*dB1v*cz[1][0] + B1u*dB1v*cz[1][1] + B2u*dB1v*cz[1][2] +
                 B0u*dB2v*cz[2][0] + B1u*dB2v*cz[2][1] + B2u*dB2v*cz[2][2];
      
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
      //課題 3-2: 課題 2-1 からコピーする
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

void drawControlNet(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_LIGHTING); //ライトを OFF
  glColor3f(1.0, 0.0, 1.0);
  
  glLineWidth(3);
  for(int i=0; i<2; i++){
    for(int j=0; j<2; j++){
      glBegin(GL_POLYGON);
      glVertex3f(cx[i][j], cy[i][j], cz[i][j]);
      glVertex3f(cx[i][j+1], cy[i][j+1], cz[i][j+1]);
      glVertex3f(cx[i+1][j+1], cy[i+1][j+1], cz[i+1][j+1]);
      glVertex3f(cx[i+1][j], cy[i+1][j], cz[i+1][j]);
      glEnd();
    }
  }
  
  glPointSize(15);
  glBegin(GL_POINTS);
  for(int i=0; i<3; i++){
    for(int j=0; j<3; j++){
      glVertex3f(cx[i][j], cy[i][j], cz[i][j]);
    }
  }
  glEnd();
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
  drawControlNet();
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
  GLfloat light_diffuse1[]={0.5, 1.0, 0.5, 1.0};
  glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_dir);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
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

