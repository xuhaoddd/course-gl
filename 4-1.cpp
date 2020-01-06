#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//�_�̐�
#define N 21
//x ���W
float px[N];
//y ���W
float py[N];

//���W���v�Z����֐�
void generatePoints(){
  for(int i=0; i<N; i++){
    //�ۑ�1-1: �ȉ��𖄂߂Ĕz�� px[i] �� py[i] �֒l��������
    //float t = ??; �ȂǂƂ���Ɗy
	  float element = (float)i / (N-1);
	  px[i] = (-4) * pow(element, 2) + 4 *  element - 0.5;
	  py[i] = 4 * pow( element, 3) - 8 * pow( element, 2) + 3  * element + 0.5;
  }
}

//�܂����`���֐�
void drawCurve(){
  glLineWidth(3); //����
  glColor3f(0,0,0); //�F�͍�

  glBegin(GL_LINES);
  for (int i = 0; i < N-1; i++) {
	  glVertex3f(px[i], py[i], 0);
	  glVertex3f(px[i + 1], py[i + 1], 0);
  }
  glEnd();
  
  //�ۑ�1-2: �J��Ԃ����g���Đ�����`���Ă���
  //�ȉ��𖄂߂� (�O�̉ۑ�Ƃقړ���)
  glPointSize(10);
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

//�����_�����O�̊֐�
void display(void){ 
  //�X�N���[���ƕϊ��s���������
  glClear(GL_COLOR_BUFFER_BIT); 
  glLoadIdentity();
  
  //���W����`��
  glLineWidth(1); 
  glColor3f(0,0,1); 
  glBegin(GL_LINES);
    glVertex3f(-1, 0, 0);
    glVertex3f( 1, 0, 0);
    glVertex3f(0, -1, 0);
    glVertex3f(0,  1, 0);
  glEnd();
  
  //�Ȑ���`��
  drawCurve();
  
  glFlush(); //�X�N���[���֕\��
} 

//�E�B���h�E�̑傫�����ς�������ɌĂ΂��֐�
void myReshape(int w, int h){
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

int main(int argc, char** argv){
  generatePoints();
  
  //�E�B���h�E�̏�����
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //�g�p����o�b�t�@�̐ݒ�
  glutInitWindowSize(500, 500); //�E�B���h�E�̑傫��
  glutCreateWindow("1-1"); //�E�B���h�E�̃^�C�g��
  
  
  myinit();//OpenGL �̏�����
  glutReshapeFunc(myReshape);//�E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ΂��֐���ݒ�
  glutDisplayFunc(display); //�E�B���h�E�̍X�V�̂��߂̊֐���ݒ�
  glutMainLoop(); //���C�����[�v
  
  return 0;
}

