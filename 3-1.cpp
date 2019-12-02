#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//���_���ƒ��_���W�̔z�� (�������̓t�@�C����ǂ񂾎��Ɋm�ۍς�)
int verN;
float (*ver)[3];

//�O�p�`���ƎO�p�`�̒��_�ԍ��̔z�� (�������̓t�@�C����ǂ񂾎��Ɋm�ۍς�)
int triN;
int (*tri)[3];

//�O�p�`�̖@�� (�T�C�Y�� triN �~ 3)
//�ۑ�2-2�Ōv�Z���� (�������̓t�@�C����ǂ񂾎��Ɋm�ۍς�)
float (*norT)[3];

//���_�̖@�� (�T�C�Y�� verN �~ 3)
//�ۑ�2-3�Ōv�Z���� (�������̓t�@�C����ǂ񂾎��Ɋm�ۍς�)
float (*norV)[3];

//���v���V�A�� (�T�C�Y�� verN �~ 3)
//�ۑ�3 �Ōv�Z���� (�������̓t�@�C����ǂ񂾎��Ɋm�ۍς�)
float (*lap)[3];
//�אڒ��_�𐔂��邽�߂̔z��
int *neiN;


//�ۑ�2-2 : �O�p�`�̖@�����v�Z����֐�
void computeTriangleNormals(){
  for(int i=0; i<triN; i++){
    //�z��̓Y�����������Ȃ�̂ŕϐ��ɑ�����Ă���
    int* t = tri[i];
    float* A = ver[ t[0] ]; // A[0] �� x���W�AA[1] �� y���W�AA[2] �� z���W�ɂȂ�
    float* B = ver[ t[1] ];
    float* C = ver[ t[2] ];
    
    //�ۑ�͈ȉ��𐳂�������
    //�x�N�g�� AB �� �x�N�g�� AC �̊O��
    float cx = 0;//??
    float cy = 0;//??
    float cz = 0;//??
    
    //�������v�Z���Đ��K������
    float l = sqrt(cx*cx + cy*cy + cz*cz);
    if(l != 0){ //����Z���Ė�����ɂȂ�Ȃ��悤�ɂ���
      norT[i][0] = cx/l;
      norT[i][1] = cy/l;
      norT[i][2] = cz/l;
    }
  }
}

//�ۑ�2-4 : ���_�@�����v�Z����֐�
void computeVertexNormals(){
  //���ׂĂ̒��_�@�����[���x�N�g���ɂ���
  for(int i=0; i<verN; i++){
    //??
  }
  
  //�e�O�p�`�ɂ�����
  for(int i=0; i<triN; i++){
     //�O�p�`��2�ӂ̊O�ς��v�Z����
    //??
    
    //����ꂽ�O�σx�N�g�������̎O�p�`���Ȃ�3���_�̖@���֑�������
    //??
  }
  
  //���ׂĂ̒��_�@���𐳋K������
  for(int i=0; i<verN; i++){
    //??
  }
}

//�ۑ� 3-1 : ���v���V�A���̌v�Z
void computeLaplacians(){
  //�S�Ă̒��_�ɂ�����
  //���v���V�A�� lap[i][0], lap[i][1], lap[i][2] ���[���ɂ���
  //�אڒ��_�� neiN[i] ���[���ɂ���
  //??
  
  //�e�O�p�`�ɂ�����
  for(int i=0; i<triN; i++){
    //�e�ӂ̃x�N�g�����n�_�̃��v���V�A���ɑ���
    //�e���_�̗אڒ��_�����P���₷
    //???
  }
  
  //�S�Ă̒��_�ɂ����ă��v���V�A����אڒ��_���Ŋ���
  //??
}

//�ۑ� 3-2 : ���v���V�A��������
void laplacianSmoothing(){
  //���v���V�A�����v�Z����
  computeLaplacians();
  
  //�ۑ� 3-3 �ł͂��̒l�𕉂ɂ��Ă݂�
  float dt = 0.5; //0�`1�̒l������ 
  
  //�e���_�� dt �~ ���v���V�A���𑫂� (�����𖄂߂�̂��ۑ� 3-2)
  //??
}


/************ ���̉��͉ۑ�ɂ͊֌W�̂Ȃ��֐� (�݂Ȃ��Ă悢) *****************/

//�ϐ����낢��
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire, flat, smooth, cur;
int width, height;
float aveE; //�ӂ̕��ς̒��� (���ʂ̐F�Â��Ɏg��)

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

//�X���[�X�V�F�[�f�B���O
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
  glDisable(GL_LIGHTING); //���C�g��OFF
  
  for(int i=0; i<triN; i++){
    int *t = tri[i];
    glBegin(GL_POLYGON);
    for(int j=0; j<3; j++){
      float dot = lap[t[j]][0]*norV[t[j]][0] + lap[t[j]][1]*norV[t[j]][1] + lap[t[j]][2]*norV[t[j]][2];
      float c = -5.0*dot/aveE + 0.5; //�F���悭�t���悤�ɒl�������L�΂�
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
  
  //���b�V���̃����_�����O
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
  
  //�ӂ̕��ς̒������v�Z���Ă���
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
  case 'f':
    printf("�t���b�g�V�F�[�f�B���O�؂�ւ�\n");
    flat = !flat;
    if(flat){
      smooth = false;
      cur = false;
    }
    display();
    break;
  case 's':
    printf("�X���[�X�V�F�[�f�B���O�؂�ւ�\n");
    smooth = !smooth;
    if(smooth){
      flat = false;
      cur = false;
    }
    display();
    break;
  case 'l':
    printf("���v���V�A��������\n");
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
    printf("���ʕ\���؂�ւ�\n");
    cur = !cur;
    if(cur){
      flat = false;
      smooth = false;
    }
    display();
    break;
  }
}


/**********************�������Ȃ��Ƃ悢�Ƃ��낱���܂�***********************/

int main(int argc, char** argv){
  //�ۑ� 1-2 : �F�X���b�V���̃t�@�C����ς��Ă݂�
  //�ۑ� 1-3 �� 2-3 : �����ō�����s���~�b�h�̃t�@�C����ǂݍ���
  //�ǂݍ��߂�f�[�^�� "round", "moai", "bunny", "hand", "dino", "igea", "armadillo"
  readMesh("data/bunny");
  
  normalizeSize(4); //���b�V���̑傫�������E�ɍ����悤�Ɋg��k������ (�� �����Ƃ͕ς��)
  computeTriangleNormals(); //�O�p�`�@�����v�Z����
  computeVertexNormals(); //���_�@�����v�Z����
  computeLaplacians(); //���v���V�A�����v�Z����
  
  //��]�Ȃǂ̏�����
  qw = 1;
  qx = qy =  qz = 0;
  zoom = 1;
  shiftX = shiftY = 0;
  wire = true;
  flat = false;
  cur = false;
  smooth = false;
  
  //�E�B���h�E�̏�����
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //�g�p����o�b�t�@�̐ݒ�
  glutInitWindowSize(800, 800); //�E�B���h�E�̑傫��
  glutCreateWindow("prog3-1"); //�E�B���h�E�̃^�C�g��
  
  myinit(); //OpenGL �̏�����
  glutReshapeFunc(&myReshape); //�E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ΂��֐���ݒ�
  glutDisplayFunc(&display);  //�E�B���h�E�̍X�V�̂��߂̊֐���ݒ�
  glutMouseFunc(mouse); //�}�E�X�����삳�ꂽ�Ƃ��ɌĂ΂��֐���ݒ�
  glutKeyboardFunc(keyboard);//�L�[�{�[�h�������ꂽ�Ƃ��ɌĂ΂��֐���ݒ�
  
  glutMainLoop(); //���C�����[�v
  
  deleteMesh(); //*�}�[�N�̔z��Ŏg���Ă��郁�������J������
  
  return 0;
}

