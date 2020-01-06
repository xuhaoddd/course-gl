#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//�i�q�ׂ̍���
#define CELLN 50

//�Z���̃f�[�^
int cell[CELLN][CELLN][CELLN];

//�ȗ����O�̃��b�V���̃f�[�^
int ver0N; 
float (*ver0)[3];
int tri0N;
int (*tri0)[3];

float cell_verN[CELLN][CELLN][CELLN];
float cell_ver_sum[CELLN][CELLN][CELLN][3];


//�ȗ�����̃��b�V���̃f�[�^
int verN;
float (*ver)[3]; 
int triN;
int (*tri)[3];

//�ۑ� 3-1 : ���_��ǉ�����֐�
void addVertex(float p[3]){
	ver[verN][0] = p[0];
	ver[verN][1] = p[1];
	ver[verN][2] = p[2];
	verN++;
}

//�ۑ�3-2 : �O�p�`��ǉ�����֐�
void addTriangle(int a, int b, int c){
	tri[triN][0] = a;
	tri[triN][1] = b;
	tri[triN][2] = c;
	triN++;

}

//id �Ԗڂ̒��_���܂ރZ���̒��_�������ď���������֐�
int cellVertexID(int id){
  float* p = ver0[id];
  //���_�̓����Ă���Z����������
  //���_�� [-1,1]^3 �̗����̂ɓ����Ă���
  int cx = (int)(CELLN*(p[0] + 1.0f)/2.0f);
  int cy = (int)(CELLN*(p[1] + 1.0f)/2.0f);
  int cz = (int)(CELLN*(p[2] + 1.0f)/2.0f);

  //for(int j=0;j<3;j++)
  //cell_V[cz][cy][cx][j] += p[j];
  cell_verN[cz][cy][cx]++;

  for (int i = 0; i < 3; i++) {
	  cell_ver_sum[cz][cy][cx][i] += p[i];
  }
  
  if(cell[cz][cy][cx] < 0){
    cell[cz][cy][cx] = verN;
    addVertex(p);
  }
  else {
	  for (int i = 0; i < 3; i++)
		  ver[cell[cz][cy][cx]][i] = cell_ver_sum[cz][cy][cx][i] / cell_verN[cz][cy][cx];
  }

  return cell[cz][cy][cx];
}

//�ȗ����̊֐�
void simplify(){
  verN = 0;
  triN = 0;
  for(int i=0; i<CELLN; i++)
  for(int j=0; j<CELLN; j++)
  for(int k=0; k<CELLN; k++)
    cell[i][j][k] = -1;

  for (int i = 0; i < CELLN; i++)
	  for (int j = 0; j < CELLN; j++)
		  for (int k = 0; k < CELLN; k++)
			  cell_verN[i][j][k] = 0;
  
  for (int i = 0; i < CELLN; i++)
	  for (int j = 0; j < CELLN; j++)
		  for (int k = 0; k < CELLN; k++)
			  for (int l = 0; l < 3; l++)
			  cell_ver_sum[i][j][k][l] = 0;

  for(int i=0; i<tri0N; i++){
    int* t0 = tri0[i];
    int a = cellVertexID(t0[0]);
    int b = cellVertexID(t0[1]);
    int c = cellVertexID(t0[2]);
    
    if(a!=b && b!=c && c!=a)
      addTriangle(a, b, c);
  }
}

/************ ���̉��͉ۑ�ɂ͊֌W�̂Ȃ��֐� (�݂Ȃ��Ă悢) *****************/

void backOriginal(){
  verN = ver0N;
  triN = tri0N;
  
  for(int i=0; i<ver0N; i++){
    ver[i][0] = ver0[i][0];
    ver[i][1] = ver0[i][1];
    ver[i][2] = ver0[i][2];
  }
  
  for(int i=0; i<tri0N; i++){
    tri[i][0] = tri0[i][0];
    tri[i][1] = tri0[i][1];
    tri[i][2] = tri0[i][2];
  }
}

//�ϐ����낢��
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire;
int width, height;

//�O�p�`�̖@�� (�T�C�Y�� triN �~ 3)
float (*norT)[3];


//���C���[�ŕ\������
void drawWire(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_LIGHTING); //���C�g�� OFF
  
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

//�t���b�g�V�F�[�f�B���O
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

void display(void){ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();
  
  glTranslatef(shiftX, shiftY, 0);
  float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
  if(s != 0)
    glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
  
  //���b�V���̃����_�����O
  flatShading();
  if(wire)
    drawWire();
  
  glFlush(); 
  glutSwapBuffers(); 
} 

//�O�p�`�̖@�����v�Z����֐�
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
  ver =  new float[ver0N][3];
  ver0 = new float[ver0N][3];
  tri = new int[tri0N][3];
  tri0 = new int[tri0N][3];
  norT = new float[tri0N][3];
}

void deleteMesh(){
  delete[] ver;
  delete[] ver0;
  delete[] tri;
  delete[] tri0;
  delete[] norT;
}

void readMesh(const char* file_name){
  /* Read the mesh file */
	FILE* in;
	fopen_s(&in,file_name, "r");
  
  fscanf_s(in, "%d", &ver0N);
  fscanf_s(in, "%d", &tri0N);
  
  allocateMem();
  
  for(int i=0; i<ver0N; i++){
    float* v = ver0[i];
    fscanf_s(in, "%f %f %f", &v[0], &v[1], &v[2]);
  }
  
  for(int i=0; i<tri0N; i++){
    int* t = tri0[i];
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
    for(int j=1; j<ver0N; j++){
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
    for(int j=0; j<ver0N; j++)
      ver0[j][i] = scale*(ver0[j][i] - mid[i]);
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
    glOrtho(-1.0/zoom, 1.0/zoom,
            -1.0 * (GLfloat) h / (GLfloat) w /zoom, 
            1.0 * (GLfloat) h / (GLfloat) w /zoom,
            -10.0, 10.0); 
  else 
    glOrtho(-1.0 * (GLfloat) w / (GLfloat) h / zoom, 
            1.0 * (GLfloat) w / (GLfloat) h /zoom,
            -1.0/zoom, 1.0/zoom, -10.0, 10.0); 
  glMatrixMode(GL_MODELVIEW); 
} 

//�}�E�X����
void mouse(int btn, int state, int x, int y){ 
  if(state == GLUT_DOWN){
    mouseX = x;
    mouseY = y;
  }
  else if(btn==GLUT_LEFT_BUTTON){
    float mx = -0.0025f*(x - mouseX)/zoom;
    float my = 0.0025f*(y - mouseY)/zoom;
    
    //��]
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
  else if(btn==GLUT_RIGHT_BUTTON){ //�g��k��
    zoom -= 0.0025f*(y - mouseY);
    if(zoom > 20.0f) zoom = 20.0f;
    else if(zoom < 0.05f) zoom = 0.05f;
    myReshape(width, height);
    display();
  }
  else if(btn==GLUT_MIDDLE_BUTTON){ //���s�ړ�
    shiftX += 0.0025f*(x - mouseX)/zoom;
    shiftY += 0.0025f*(y - mouseY)/zoom;
    display();
  }
} 

//�L�[�������ꂽ���ɑΉ�����֐����Ăяo��
void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 'w':
    printf("���C���[�\���؂�ւ�\n");
    wire = !wire;
    display();
    break;
  case 's':
    printf("�ȗ�����\n");
    simplify();
    computeTriangleNormals();
    display();
    break;
  case 'o':
    printf("�ȗ����O\n");
    backOriginal();
    computeTriangleNormals();
    display();
    break;
  }
}


/**********************�������Ȃ��Ƃ悢�Ƃ��낱���܂�***********************/

int main(int argc, char** argv){
  readMesh("data/igea");
  
  normalizeSize(1.0); //���b�V���̑傫�������E�ɍ����悤�Ɋg��k������ (�� �����Ƃ͕ς��)
  backOriginal();
  computeTriangleNormals(); //�O�p�`�@�����v�Z����
  
  //��]�Ȃǂ̏�����
  qw = 1;
  qx = qy =  qz = 0;
  zoom = 1;
  shiftX = shiftY = 0;
  wire = false;
  
  //�E�B���h�E�̏�����
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //�g�p����o�b�t�@�̐ݒ�
  glutInitWindowSize(500, 500); //�E�B���h�E�̑傫��
  glutCreateWindow("prog3-1"); //�E�B���h�E�̃^�C�g��
  
  myinit(); //OpenGL �̏�����
  glutReshapeFunc(myReshape); //�E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ΂��֐���ݒ�
  glutDisplayFunc(display);  //�E�B���h�E�̍X�V�̂��߂̊֐���ݒ�
  glutMouseFunc(mouse); //�}�E�X�����삳�ꂽ�Ƃ��ɌĂ΂��֐���ݒ�
  glutKeyboardFunc(keyboard);//�L�[�{�[�h�������ꂽ�Ƃ��ɌĂ΂��֐���ݒ�
  
  glutMainLoop(); //���C�����[�v
  
  deleteMesh(); //*�}�[�N�̔z��Ŏg���Ă��郁�������J������
  
  return 0;
}

