#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

#define PI 3.1415926535897932384626433832795

#define N 50 //�_�̐�
float px[N][N]; //x ���W
float py[N][N]; //y ���W
float pz[N][N]; //z ���W

//�Ȗʏ�̓_���v�Z����֐�
void generatePoints(){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      float u = 2*PI*(float)i/(N-1); //u: 0�`2��
      float v = 2*PI*(float)j/(N-1); //v: 0�`2��
      
      //��]�ʂŕ`���̂Œf�ʂ̉~���܂��v�Z
      //���S (3,0,0) �Ŕ��a 1 �̉~
      float cx = 3 + cos(v);
      float cy = 0;
      float cz = sin(v);
      //z �̎��̎���ɉ�]
      px[i][j] = cos(u)*cx - sin(u)*cy;
      py[i][j] = sin(u)*cx + cos(u)*cy;
      pz[i][j] = cz;
      
      //�������ȉ��̂悤�Ɍv�Z��̎��ł��悢
      //px[i][j] = cos(u)*(3+cos(v));
      //py[i][j] = sin(u)*(3+cos(v));
      //pz[i][j] = sin(v);
      
      /* //�ۑ� 1-2 : �ȉ��̎����~�ɕς���
      float u = 2*PI*(float)i/(N-1); //u �́A�o�x�Ȃ̂� 0�`2��
      float v = PI*((float)j/(N-1)-0.5); //v �͈ܓx�Ȃ̂� -0.5�΁`0.5��
      px[i][j] = cos(u)*cos(v);
      py[i][j] = sin(u)*cos(v);
      pz[i][j] = sin(v);
      */
    }
  }
}

//�Ȗʂ����C���t���[���ŕ`���֐�
void drawWire(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_LIGHTING); //���C�g�� OFF
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(1);
  
  for(int i=0; i<N-1; i++){
    for(int j=0; j<N-1; j++){
      //�ۑ� 1-1 : �ȉ��� OpenGL �̖��߂Ŏl�ӌ`��`��
      glBegin(GL_POLYGON);
      glVertex3f(px[i  ][j  ], py[i  ][j  ], pz[i]  [j  ]);
      glVertex3f(px[i  ][j+1], py[i  ][j+1], pz[i  ][j+1]);
      glVertex3f(px[i+1][j+1], py[i+1][j+1], pz[i+1][j+1]);
      glVertex3f(px[i+1][j  ], py[i+1][j  ], pz[i+1][j  ]);
      glEnd();
    }
  }
}

float nx[N][N]; //�@���x�N�g���� x ����
float ny[N][N]; //�@���x�N�g���� y ����
float nz[N][N]; //�@���x�N�g���� z ����

//�@���x�N�g�����v�Z����֐�
void computeNormals(){
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      float u = 2*PI*(float)i/(N-1); //u: 0�`2��
      float v = 2*PI*(float)j/(N-1); //v: 0�`2��
      
      //u �����̔���
      float ux = -sin(u)*(3+cos(v));
      float uy = cos(u)*(3+cos(v));
      float uz = 0;
      
      //v �����̔���
      float vx = -sin(v)*cos(u);
      float vy = -sin(v)*sin(u);
      float vz = cos(v);      
      
      /* //�ۑ� 2-2: �ȉ��̎����~�̎��̕Δ����ɕς���
      float u = 2*3.1412*(float)i/(N-1); //u �́A�o�x�Ȃ̂� 0�`2��
      float v = 3.1412*((float)j/(N-1)-0.5); //v �͈ܓx�Ȃ̂� -0.5�΁`0.5��
      
      //u �����̔���
      float ux = -sin(u)*cos(v);
      float uy = cos(u)*cos(v);
      float uz = 0;
      
      //v �����̔���
      float vx = -cos(u)*sin(v);
      float vy = -sin(u)*sin(v);
      float vz = cos(v);
      */
      
      //�O�ς��v�Z
      float cx = uy*vz - uz*vy;
      float cy = uz*vx - ux*vz;
      float cz = ux*vy - uy*vx;
      
      //�O�ς̒���
      float l = sqrt(cx*cx + cy*cy + cz*cz);
      
      nx[i][j] = cx/l;
      ny[i][j] = cy/l;
      nz[i][j] = cz/l;
    }
  }
}

//�Ȗʂ��X���[�X�V�F�[�f�B���O�ŕ`���֐�
void drawSurface(){
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glShadeModel(GL_SMOOTH); 
  glEnable(GL_LIGHTING);
  
  for(int i=0; i<N-1; i++){
    for(int j=0; j<N-1; j++){
      //�ۑ� 2-1: �e���_�Ŗ@�����w�肵�Ȃ���l�ӌ`��`��
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

/************ ���̉��͉ۑ�ɂ͊֌W�̂Ȃ��֐� (�݂Ȃ��Ă悢) *****************/

//�ϐ����낢��
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
  
  //�����_�����O
  if(wire)
    drawWire();
  else
    drawSurface();
  
  glDisable(GL_LIGHTING); //���C�g�� OFF
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
  }
}

/**********************�������Ȃ��Ƃ悢�Ƃ��낱���܂�***********************/

int main(int argc, char** argv){
  generatePoints(); //�Ȗʏ�̓_�𐶐�����
  computeNormals(); //�@�����v�Z����
  
  //��]�Ȃǂ̏�����
  qw = 1;
  qx = qy =  qz = 0;
  zoom = 1;
  shiftX = shiftY = 0;
  wire = true;
  
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
  
  return 0;
}

