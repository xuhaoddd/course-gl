#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//�`��̂��߂̓_�̐�
#define N 100

//�Ȑ���� x ���W �� y ���W
float px[N];
float py[N];

//����_�̍��W
float cx[4] = {-0.75, -0.5, 0.5,  0.75};
float cy[4] = {-0.5,   0.5,  0.5,  -0.5};

//���W���v�Z����֐�
void generatePoints(){
  for(int i=0; i<N; i++){
    //�ۑ�2-1: �ȉ��𖄂߂�3���̃x�W�G�Ȑ���̓_�� px[i] �� py[i] �֒l��������
    //t ��B0,B1,B2,B3 �܂ł̒l�͕ϐ��ֈ�x��������ق����y
    //float B0 = ??; �ȂǂƂ���B
	  float t = (float)i / (N - 1); //�o�����[�^ t

	  float B0 = (1 - t) * (1 - t) * (1 - t);
	  float B1 = 3 * (1 - t) * (1 - t) * t;
	  float B2 = 3 * t * t * (1 - t);
	  float B3 = t * t * t;
	  px[i] = B0 * cx[0] + B1 * cx[1] + B2 * cx[2] + B3 * cx[3];
	  py[i] = B0 * cy[0] + B1 * cy[1] + B2 * cy[2] + B3 * cy[3];
  }
}

//�܂����`���֐�
void drawCurve(){
  glLineWidth(3); //����
  glColor3f(0,0,0); //�F�͍�
  
  //�ۑ�1-2 ����R�s�[���� : �J��Ԃ����g���Đ�����`���Ă���
  glBegin(GL_LINES);
  for (int i = 0; i < N - 1; i++) {
	  glVertex3f(px[i], py[i], 0);
	  glVertex3f(px[i + 1], py[i + 1], 0);
  }
  glEnd();

  //�ۑ�1-2 (���܂�): �_�̐F��ς��ĕ`���Ă݂�
  glPointSize(3);
  glBegin(GL_POINTS);
  for (int i = 0; i < N; i++) {
	  if (i < N / 2)
		  glColor3f(1, 0, 2.0 * i / (N - 1));
	  else
		  glColor3f(1.0 - 2.0 * (i - N / 2) / (N - 1), 0, 1);
	  glVertex3f(px[i], py[i], 0);
  }
  glEnd();
}

void moveControlPoints(float x, float y){
  //�ۑ�2-2 : ����_�̈ړ�������
  
  int k = 0; //k �͍ł��߂�����_�̔ԍ� (�b��I�� 0 �ԂƂȂ��Ă���)
  //�ȉ��� (x,y) �� (cx[0], cy[0]) �̊Ԃ̋�����2��
  float min = (x-cx[0])*(x-cx[0]) + (y-cy[0])*(y-cy[0]);
  
  //�ۑ�͈ȉ��𖄂߂� (���̐���_�Ƃ̋������v���� k ���X�V����)
  //for ���͎g���Ă�����Ȃ��Ă��悢
  for (int i = 1; i < 4; i++) {
	  float d = (x - cx[i]) * (x - cx[i]) + (y - cy[i]) * (y - cy[i]);
	  if (d < min) {
		  min = d;
		  k = i;
	  }
  }
  //����_�̍X�V
  cx[k] = x;
  cy[k] = y;
}

//�Ȑ���̖@���x�N�g�� (������ 1 �Ƃ���)
float nx[N];
float ny[N];

void computeNormals(){
  for(int i=0; i<N; i++){
    //�ۑ�3-1: �ȉ��𖄂߂�3���̃x�W�G�Ȑ���̖@���� nx[i] �� ny[i] �֒l��������
	  float t = (float)i / (N - 1);

	  //���֐��̔���
	  float dB0 = -3 * (1 - t) * (1 - t);
	  float dB1 = 3 * (1 - t) * (1 - 3 * t);
	  float dB2 = 3 * t * (2 - 3 * t);
	  float dB3 = 3 * t * t;

	  float dx = dB0 * cx[0] + dB1 * cx[1] + dB2 * cx[2] + dB3 * cx[3];
	  float dy = dB0 * cy[0] + dB1 * cy[1] + dB2 * cy[2] + dB3 * cy[3];

	  float l = sqrt(dx * dx + dy * dy);
	  //�@���x�N�g���͐ڐ��x�N�g����90�x��]�����x�N�g��
	  nx[i] = -dy / l;
	  ny[i] = dx / l;
  }
}

void drawOffsetCurves(){
  glLineWidth(1); //����
  glColor3f(0,1,1); //�F�͐��F
  
  //�ۑ�3-2 �� 3-3 : �I�t�Z�b�g�Ȑ���`��
  glBegin(GL_LINES);
  for (int j = -10; j <= 10; j++) {
	  if (j != 0) {
		  for (int i = 0; i < N - 1; i++) {
			  glVertex3f(px[i] + 0.1 * j * nx[i], py[i] + 0.1 * j * ny[i], 0);
			  glVertex3f(px[i + 1] + 0.1 * j * nx[i + 1], py[i + 1] + 0.1 * j * ny[i + 1], 0);
		  }
	  }
  }
  glEnd();
  
}

//�ȗ����a
float R[N];

void computeCurvatureRadius(){
  for(int i=0; i<N; i++){
    //�ۑ�4-1: �ȉ��𖄂߂�3���̃x�W�G�Ȑ���̋ȗ����a�� R[i] �֒l��������
    
  }
}

//�k����`���֐�
void drawEvolue(){
  glLineWidth(2); //����
  glColor3f(1,0,0); //�F�͐ԐF
  
  //�ۑ�4-2 : �k����`��
  
}

//�����_�����O�̊֐�
void display(void){ 
  //�X�N���[���ƕϊ��s���������
  glClear(GL_COLOR_BUFFER_BIT); 
  glLoadIdentity();
  
  //���W����`��
  glLineWidth(1); 
  glColor3f(0,0,1); 
  glBegin(GL_LINES);
    glVertex3f(-1, 0, 0); glVertex3f( 1, 0, 0);
    glVertex3f(0, -1, 0); glVertex3f(0,  1, 0);
  glEnd();
  
  //����_��`��
  glLineWidth(2); 
  glColor3f(0,1,0); 
  glBegin(GL_LINE_STRIP);
    glVertex3f(cx[0], cy[0], 0);
    glVertex3f(cx[1], cy[1], 0);
    glVertex3f(cx[2], cy[2], 0);
    glVertex3f(cx[3], cy[3], 0);
  glEnd();
  
  glPointSize(10);
  glBegin(GL_POINTS);
    glVertex3f(cx[0], cy[0], 0);
    glVertex3f(cx[1], cy[1], 0);
    glVertex3f(cx[2], cy[2], 0);
    glVertex3f(cx[3], cy[3], 0);
  glEnd();

  //�Ȑ���`��
  drawCurve();
  
  //�I�t�Z�b�g��`��
  drawOffsetCurves();
  
  //�k����`��
  drawEvolue();
  
  glFlush(); //�X�N���[���֕\��
} 

//�E�B���h�E�̑傫�����ς�������ɌĂ΂��֐�
int width, height;
void myReshape(int w, int h){
  width = w;
  height = h;
  
  glViewport(0, 0, w, h); //�X�N���[���̑傫�������߂�
  
  //�ϊ��s���ݒ�
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  
  //���s���e�̂��߂̃p�����[�^��ݒ�
  //������̈�� (-1,-1,-1) �� (1,1,1) ���R�[�i�[�Ɏ������̂̒�
  glOrtho(-1, 1, //���E
          -1, 1, //����
          -1, 1 //��O��
          );
  
  glMatrixMode(GL_MODELVIEW); //�}�`�`�惂�[�h�֐؂�ւ�
} 

void myinit(){
  //�w�i�̐F��ݒ� (�S�Ԗڂ͓��ߓx�Œʏ�͕s�����Ȃ̂� 1 ��ݒ�)
  glClearColor (1, 1, 1, 1);

}

//�}�E�X����̊֐�
void mouse(int btn, int state, int x, int y){
  moveControlPoints(2*(float)x/width-1, 2*(float)(height-y)/height-1);
  
  //�Ȑ���̓_������l���Čv�Z����
  generatePoints();
  computeNormals();
  computeCurvatureRadius();
  
  //�ĕ`��
  display();
}

int main(int argc, char** argv){
  //�Ȑ��̓_������l���v�Z����
  generatePoints();
  computeNormals();
  computeCurvatureRadius();
  
  //�E�B���h�E�̏�����
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //�g�p����o�b�t�@�̐ݒ�
  glutInitWindowSize(500, 500); //�E�B���h�E�̑傫��
  glutCreateWindow("1-1"); //�E�B���h�E�̃^�C�g��
  
  myinit();//OpenGL �̏�����
  glutReshapeFunc(myReshape);//�E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ΂��֐���ݒ�
  glutMouseFunc(mouse); //�}�E�X�����삳�ꂽ�Ƃ��ɌĂ΂��֐���ݒ�
  glutDisplayFunc(display); //�E�B���h�E�̍X�V�̂��߂̊֐���ݒ�
  glutMainLoop(); //���C�����[�v
  
  return 0;
}

