#include <stdio.h>
#include <math.h>
#include <GL/glut.H>


/************ 以下変形のデータ (課題1-2はここを変更する) ***********/
//変形ベクトルの数 
#define N 7

//変形ベクトル
float t[N][3] = {
  {0,0,0.75f}, //頭
  {0.5f,0,0.3f}, {-0.5f,0,0.3f}, //手
  {-0.2f,0,-0.5f}, {0.2f,0,-0.5f}, //足
  {0,0.5f,0}, {0,0.5f,0} //目
};

// 変形領域の中心
float c[N][3] = {
  {0,0,1},
  {1,0,0}, {-1,0,0}, 
  {0.5f,0,-1}, {-0.5f,0,-1}, 
  {0.3f,-1,0.3f}, {-0.3f,-1,0.3f}  
};

//変形領域の半径
float r[N] = {
  1.5f,
  0.4f, 0.4f,
  0.5f, 0.5f,
  0.25f, 0.25f
};
/************** 変形のデータはここまで ***************************/


//頂点数と頂点座標の配列 (メモリはファイルを読んだ時に確保済み)
int verN;
float (*ver0)[3]; //変形前
float (*ver)[3]; //変形後

//課題1-1: 変形の関数
void deform(){
  for(int i=0; i<verN; i++){
    float s[3] = {0,0,0}; // 始めゼロにして値を足していく
    for(int j=0; j<N; j++){
      // ここを埋める (式のΣ記号の中)
    }
    ver[i][0] = ver0[i][0] + s[0];
    ver[i][1] = ver0[i][1] + s[1];
    ver[i][2] = ver0[i][2] + s[2];
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

//三角形数と三角形の頂点番号の配列 (メモリはファイルを読んだ時に確保済み)
int triN;
int (*tri)[3];

//三角形の法線 (サイズは triN × 3)
float (*norT)[3];

//変形前に戻す関数
void backOriginal(){
  for(int i=0; i<verN; i++){
    ver[i][0] = ver0[i][0];
    ver[i][1] = ver0[i][1];
    ver[i][2] = ver0[i][2];
  }
}

//ワイヤーで表示する
void drawWire(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_LIGHTING); //ライトを OFF
  
  glColor3f(0.4, 0.4, 0.4);
  for(int i=0; i<triN; i++){
    int *t = tri[i];
    glBegin(GL_POLYGON);
      glVertex3fv(ver[t[0]]);
      glVertex3fv(ver[t[1]]);
      glVertex3fv(ver[t[2]]);
    glEnd();
  }
}

//フラットシェーディング
void flatShading(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glShadeModel(GL_FLAT); 
  glEnable(GL_LIGHTING);
  
  for(int i=0; i<triN; i++){
    int *t = tri[i];
    glBegin(GL_POLYGON);
      glNormal3fv(norT[i]);
      glVertex3fv(ver[t[0]]);
      glVertex3fv(ver[t[1]]);
      glVertex3fv(ver[t[2]]);
    glEnd();
  }
}

void drawDisps(){
  glDisable(GL_LIGHTING);
  glColor3f (1.0, 0.5, 0.5);
  glDisable(GL_DEPTH_TEST);
  
  for(int i=0; i<N; i++){
    glBegin(GL_LINES);
      glVertex3fv(c[i]);
      glVertex3f(c[i][0]+t[i][0],
                 c[i][1]+t[i][1],
                 c[i][2]+t[i][2]);
    glEnd();
    glBegin(GL_POINTS);
      glVertex3fv(c[i]);
    glEnd();
  }
  glEnable(GL_DEPTH_TEST);
}

void display(void){ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();
  
  glTranslatef(shiftX, shiftY, 0);
  float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
  if(s != 0)
    glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
  
  //メッシュのレンダリング
  flatShading();
  drawDisps();
  if(wire)
    drawWire();
  
  glFlush(); 
  glutSwapBuffers(); 
} 

//三角形の法線を計算する関数
void computeTriangleNormals(){
  for(int i=0; i<triN; i++){
    int* t = tri[i];
    float* A = ver[ t[0] ]; 
    float* B = ver[ t[1] ];
    float* C = ver[ t[2] ];
    
    float cx = (B[1]-A[1])*(C[2]-A[2]) - (B[2]-A[2])*(C[1]-A[1]);
    float cy = (B[2]-A[2])*(C[0]-A[0]) - (B[0]-A[0])*(C[2]-A[2]);
    float cz = (B[0]-A[0])*(C[1]-A[1]) - (B[1]-A[1])*(C[0]-A[0]);
    
    float l = sqrt(cx*cx + cy*cy + cz*cz);
    if(l != 0){
      norT[i][0] = cx/l;
      norT[i][1] = cy/l;
      norT[i][2] = cz/l;
    }
  }
}

void allocateMem(){
  ver =  new float[verN][3];
  ver0 = new float[verN][3];
  tri = new int[triN][3];
  norT = new float[triN][3];
}

void deleteMesh(){
  delete[] ver;
  delete[] ver0;
  delete[] tri;
  delete[] norT;
}

void readMesh(char* file_name){
  /* Read the mesh file */
	FILE* in;
	fopen_s(&in,file_name, "r");
  
  fscanf_s(in, "%d", &verN);
  fscanf_s(in, "%d", &triN);
  
  allocateMem();
  
  for(int i=0; i<verN; i++){
    float* v = ver0[i];
    fscanf_s(in, "%f %f %f", &v[0], &v[1], &v[2]);
  }
  
  for(int i=0; i<triN; i++){
    int* t = tri[i];
    fscanf_s(in, "%d %d %d", &t[0], &t[1], &t[2]);
  }
  
  fclose(in);
}
  
void normalizeSize(float radius){
  // Compute the bounding box
  float max[3], min[3];
  float size[3], mid[3];
  for(int i=0; i<3; i++){
    max[i] = ver0[0][i];
    min[i] = ver0[0][i];
    for(int j=1; j<verN; j++){
      float v = ver0[j][i];
      if(v > max[i])
        max[i] = v;
      else if(v < min[i])
        min[i] = v;
    }
    size[i] = max[i] - min[i];
    mid[i] = 0.5f*(max[i] + min[i]);
  }
  
  float diagonal = (float)sqrt(size[0]*size[0] + size[1]*size[1] + size[2]*size[2]);
  float scale = radius/(0.5f*diagonal);
  
  for(int i=0; i<3; i++)
    for(int j=0; j<verN; j++)
      ver[j][i] = ver0[j][i] = scale*(ver0[j][i] - mid[i]);
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
  case 'd':
    printf("変形後\n");
    deform();
    computeTriangleNormals();
    display();
    break;
  case 'o':
    printf("変形前\n");
    backOriginal();
    computeTriangleNormals();
    display();
    break;
  }
}


/**********************理解しなくとよいところここまで***********************/

int main(int argc, char** argv){
  readMesh("data/sphere");
  
  normalizeSize(sqrt(3.0)); //メッシュの大きさを視界に合うように拡大縮小する (※ 実寸とは変わる)
  computeTriangleNormals(); //三角形法線を計算する
  
  //回転などの初期化
  qw = 1;
  qx = qy =  qz = 0;
  zoom = 1;
  shiftX = shiftY = 0;
  wire = false;
  
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
  
  deleteMesh(); //*マークの配列で使っているメモリを開放する
  
  return 0;
}

