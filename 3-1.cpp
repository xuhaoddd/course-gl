#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//頂点数と頂点座標の配列 (メモリはファイルを読んだ時に確保済み)
int verN;
float (*ver)[3];

//三角形数と三角形の頂点番号の配列 (メモリはファイルを読んだ時に確保済み)
int triN;
int (*tri)[3];

//三角形の法線 (サイズは triN × 3)
//課題2-2で計算する (メモリはファイルを読んだ時に確保済み)
float (*norT)[3];

//頂点の法線 (サイズは verN × 3)
//課題2-3で計算する (メモリはファイルを読んだ時に確保済み)
float (*norV)[3];

//ラプラシアン (サイズは verN × 3)
//課題3 で計算する (メモリはファイルを読んだ時に確保済み)
float (*lap)[3];
//隣接頂点を数えるための配列
int *neiN;


//課題2-2 : 三角形の法線を計算する関数
void computeTriangleNormals(){
  for(int i=0; i<triN; i++){
    //配列の添え字が長くなるので変数に代入しておく
    int* t = tri[i];
    float* A = ver[ t[0] ]; // A[0] は x座標、A[1] は y座標、A[2] は z座標になる
    float* B = ver[ t[1] ];
    float* C = ver[ t[2] ];
    
    //課題は以下を正しく直す
    //ベクトル AB と ベクトル AC の外積
    float cx = 0;//??
    float cy = 0;//??
    float cz = 0;//??
    
    //長さを計算して正規化する
    float l = sqrt(cx*cx + cy*cy + cz*cz);
    if(l != 0){ //割り算して無限大にならないようにする
      norT[i][0] = cx/l;
      norT[i][1] = cy/l;
      norT[i][2] = cz/l;
    }
  }
}

//課題2-4 : 頂点法線を計算する関数
void computeVertexNormals(){
  //すべての頂点法線をゼロベクトルにする
  for(int i=0; i<verN; i++){
    //??
  }
  
  //各三角形において
  for(int i=0; i<triN; i++){
     //三角形の2辺の外積を計算して
    //??
    
    //得られた外積ベクトルをその三角形をなす3頂点の法線へ足しこむ
    //??
  }
  
  //すべての頂点法線を正規化する
  for(int i=0; i<verN; i++){
    //??
  }
}

//課題 3-1 : ラプラシアンの計算
void computeLaplacians(){
  //全ての頂点において
  //ラプラシアン lap[i][0], lap[i][1], lap[i][2] をゼロにする
  //隣接頂点数 neiN[i] をゼロにする
  //??
  
  //各三角形において
  for(int i=0; i<triN; i++){
    //各辺のベクトルを始点のラプラシアンに足し
    //各頂点の隣接頂点数を１増やす
    //???
  }
  
  //全ての頂点においてラプラシアンを隣接頂点数で割る
  //??
}

//課題 3-2 : ラプラシアン平滑化
void laplacianSmoothing(){
  //ラプラシアンを計算する
  computeLaplacians();
  
  //課題 3-3 ではこの値を負にしてみる
  float dt = 0.5; //0～1の値を入れる 
  
  //各頂点へ dt × ラプラシアンを足す (ここを埋めるのが課題 3-2)
  //??
}


/************ この下は課題には関係のない関数 (みなくてよい) *****************/

//変数いろいろ
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire, flat, smooth, cur;
int width, height;
float aveE; //辺の平均の長さ (凹凸の色づけに使う)

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

//スムースシェーディング
void smoothShading(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glShadeModel(GL_SMOOTH);  
  glEnable(GL_LIGHTING);
  
  for(int i=0; i<triN; i++){
    int *t = tri[i];
    glBegin(GL_POLYGON);
      glNormal3fv(norV[t[0]]);
      glVertex3fv(ver[t[0]]);
      glNormal3fv(norV[t[1]]);
      glVertex3fv(ver[t[1]]);
      glNormal3fv(norV[t[2]]);
      glVertex3fv(ver[t[2]]);
    glEnd();
  }
}

void curvature(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glShadeModel(GL_SMOOTH);  
  glDisable(GL_LIGHTING); //ライトをOFF
  
  for(int i=0; i<triN; i++){
    int *t = tri[i];
    glBegin(GL_POLYGON);
    for(int j=0; j<3; j++){
      float dot = lap[t[j]][0]*norV[t[j]][0] + lap[t[j]][1]*norV[t[j]][1] + lap[t[j]][2]*norV[t[j]][2];
      float c = -5.0*dot/aveE + 0.5; //色がよく付くように値を引き伸ばす
      glColor3f(c, c, c);
      glVertex3fv(ver[t[j]]);
    }
    glEnd();
  }
}

void display(void){ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();
  
  glTranslatef(shiftX, shiftY, 0);
  float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
  if(s != 0)
    glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
  
  //メッシュのレンダリング
  if(wire)
    drawWire();
  if(flat)
    flatShading();
  else if(smooth)
    smoothShading();
  else if(cur)
    curvature();
  
  glFlush(); 
  glutSwapBuffers(); 
} 

void allocateMem(){
  ver =  new float[verN][3];
  tri = new int[triN][3];
  norV = new float[verN][3];
  norT = new float[triN][3];
  lap = new float[verN][3];
  neiN = new int[verN];
}

void deleteMesh(){
  delete[] ver;
  delete[] tri;
  delete[] norV;
  delete[] norT;
  delete[] lap;
  delete[] neiN;
}

void readMesh(const char* file_name){
  /* Read the mesh file */
  FILE* in;
  fopen_s(&in,file_name, "r");
  
  fscanf_s(in, "%d", &verN);
  fscanf_s(in, "%d", &triN);
  
  allocateMem();
  
  for(int i=0; i<verN; i++){
    float* v = ver[i];
    fscanf_s(in, "%f %f %f", &v[0], &v[1], &v[2]);
  }
  
  for(int i=0; i<triN; i++){
    int* t = tri[i];
    fscanf_s(in, "%d %d %d", &t[0], &t[1], &t[2]);
  }
  
  fclose(in);
}
  
void normalizeSize(float length){
  // Compute the bounding box
  float max[3], min[3];
  float size[3], mid[3];
  for(int i=0; i<3; i++){
    max[i] = ver[0][i];
    min[i] = ver[0][i];
    for(int j=1; j<verN; j++){
      float v = ver[j][i];
      if(v > max[i])
        max[i] = v;
      else if(v < min[i])
        min[i] = v;
    }
    size[i] = max[i] - min[i];
    mid[i] = 0.5f*(max[i] + min[i]);
  }
  
  float s = size[0];
  if(size[1] > s)
    s = size[1];
  if(size[2] > s)
    s = size[2];
  float scale = length/s;
  
  for(int i=0; i<3; i++)
    for(int j=0; j<verN; j++)
      ver[j][i] = scale*(ver[j][i] - mid[i]);
  
  //辺の平均の長さを計算しておく
  double total = 0;
  for(int i=0; i<triN; i++){
    int* t = tri[i];
    for(int j=0; j<3; j++){
      float* p = ver[t[j]];
      float* q = ver[t[(j+1)%3]];
      total += sqrt((q[0]-p[0])*(q[0]-p[0]) + (q[1]-p[1])*(q[1]-p[1]) + (q[2]-p[2])*(q[2]-p[2]));
    }
  }
  aveE = total/(3*triN);
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
  case 'f':
    printf("フラットシェーディング切り替え\n");
    flat = !flat;
    if(flat){
      smooth = false;
      cur = false;
    }
    display();
    break;
  case 's':
    printf("スムースシェーディング切り替え\n");
    smooth = !smooth;
    if(smooth){
      flat = false;
      cur = false;
    }
    display();
    break;
  case 'l':
    printf("ラプラシアン平滑化\n");
    laplacianSmoothing();
    if(flat)
      computeTriangleNormals();
    else if(smooth)
      computeVertexNormals();
    else if(cur){
      computeVertexNormals();
      computeLaplacians();
    }
    display();
    break;
  case 'c':
    printf("凹凸表示切り替え\n");
    cur = !cur;
    if(cur){
      flat = false;
      smooth = false;
    }
    display();
    break;
  }
}


/**********************理解しなくとよいところここまで***********************/

int main(int argc, char** argv){
  //課題 1-2 : 色々メッシュのファイルを変えてみる
  //課題 1-3 と 2-3 : 自分で作ったピラミッドのファイルを読み込む
  //読み込めるデータは "round", "moai", "bunny", "hand", "dino", "igea", "armadillo"
  readMesh("data/bunny");
  
  normalizeSize(4); //メッシュの大きさを視界に合うように拡大縮小する (※ 実寸とは変わる)
  computeTriangleNormals(); //三角形法線を計算する
  computeVertexNormals(); //頂点法線を計算する
  computeLaplacians(); //ラプラシアンを計算する
  
  //回転などの初期化
  qw = 1;
  qx = qy =  qz = 0;
  zoom = 1;
  shiftX = shiftY = 0;
  wire = true;
  flat = false;
  cur = false;
  smooth = false;
  
  //ウィンドウの初期化
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //使用するバッファの設定
  glutInitWindowSize(800, 800); //ウィンドウの大きさ
  glutCreateWindow("prog3-1"); //ウィンドウのタイトル
  
  myinit(); //OpenGL の初期化
  glutReshapeFunc(&myReshape); //ウィンドウサイズが変更された時に呼ばれる関数を設定
  glutDisplayFunc(&display);  //ウィンドウの更新のための関数を設定
  glutMouseFunc(mouse); //マウスが操作されたときに呼ばれる関数を設定
  glutKeyboardFunc(keyboard);//キーボードが押されたときに呼ばれる関数を設定
  
  glutMainLoop(); //メインループ
  
  deleteMesh(); //*マークの配列で使っているメモリを開放する
  
  return 0;
}

