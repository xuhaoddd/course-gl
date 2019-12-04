#include <stdio.h>
#include <math.h>
#include <GL/glut.H>


/************ �ȉ��ό`�̃f�[�^ (�ۑ�1-2�͂�����ύX����) ***********/
//�ό`�x�N�g���̐� 
#define N 7

//�ό`�x�N�g��
float t[N][3] = {
  {0,0,0.75f}, //��
  {0.5f,0,0.3f}, {-0.5f,0,0.3f}, //��
  {-0.2f,0,-0.5f}, {0.2f,0,-0.5f}, //��
  {0,0.5f,0}, {0,0.5f,0} //��
};

// �ό`�̈�̒��S
float c[N][3] = {
  {0,0,1},
  {1,0,0}, {-1,0,0}, 
  {0.5f,0,-1}, {-0.5f,0,-1}, 
  {0.3f,-1,0.3f}, {-0.3f,-1,0.3f}  
};

//�ό`�̈�̔��a
float r[N] = {
  1.5f,
  0.4f, 0.4f,
  0.5f, 0.5f,
  0.25f, 0.25f
};
/************** �ό`�̃f�[�^�͂����܂� ***************************/


//���_���ƒ��_���W�̔z�� (�������̓t�@�C����ǂ񂾎��Ɋm�ۍς�)
int verN;
float (*ver0)[3]; //�ό`�O
float (*ver)[3]; //�ό`��

//�ۑ�1-1: �ό`�̊֐�
void deform(){
  for(int i=0; i<verN; i++){
    float s[3] = {0,0,0}; // �n�߃[���ɂ��Ēl�𑫂��Ă���
    for(int j=0; j<N; j++){
      // �����𖄂߂� (���̃��L���̒�)
    }
    ver[i][0] = ver0[i][0] + s[0];
    ver[i][1] = ver0[i][1] + s[1];
    ver[i][2] = ver0[i][2] + s[2];
  }
}

/************ ���̉��͉ۑ�ɂ͊֌W�̂Ȃ��֐� (�݂Ȃ��Ă悢) *****************/

//�ϐ����낢��
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire;
int width, height;

//�O�p�`���ƎO�p�`�̒��_�ԍ��̔z�� (�������̓t�@�C����ǂ񂾎��Ɋm�ۍς�)
int triN;
int (*tri)[3];

//�O�p�`�̖@�� (�T�C�Y�� triN �~ 3)
float (*norT)[3];

//�ό`�O�ɖ߂��֐�
void backOriginal(){
  for(int i=0; i<verN; i++){
    ver[i][0] = ver0[i][0];
    ver[i][1] = ver0[i][1];
    ver[i][2] = ver0[i][2];
  }
}

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
  
  //���b�V���̃����_�����O
  flatShading();
  drawDisps();
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
  case 'd':
    printf("�ό`��\n");
    deform();
    computeTriangleNormals();
    display();
    break;
  case 'o':
    printf("�ό`�O\n");
    backOriginal();
    computeTriangleNormals();
    display();
    break;
  }
}


/**********************�������Ȃ��Ƃ悢�Ƃ��낱���܂�***********************/

int main(int argc, char** argv){
  readMesh("data/sphere");
  
  normalizeSize(sqrt(3.0)); //���b�V���̑傫�������E�ɍ����悤�Ɋg��k������ (�� �����Ƃ͕ς��)
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

