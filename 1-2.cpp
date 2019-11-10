#include <stdio.h>
#include <math.h>
#include <GL/glut.H>

//�����_�����O�̊֐�
void display(void){ 
  //�X�N���[���ƕϊ��s���������
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glLoadIdentity();
  
  //�ۑ�1��2�̃A�t�B���ϊ��͂����ɏ���
  //�ۑ�2�ł̓X�^�b�N�𗘗p����Ɗy
  
  //�e�B�[�|�b�g�������_�����O (0.5�͑傫��)
  glutSolidTeapot(0.5);
  
  glFlush(); //�X�N���[���֕\��
} 

//�E�B���h�E�̑傫�����ς�������ɌĂ΂��֐�
void myReshape(int w, int h){
  glViewport(0, 0, w, h); //�X�N���[���̑傫�������߂�
  
  //�ϊ��s���ݒ�
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  
  //���s���e�̂��߂̃p�����[�^��ݒ�
  //�ۑ�5�̓����ϊ��͈ȉ���u��������
  if (w < h) 
    glOrtho(-1.0, 1.0, - h/w, h/w, -10.0, 10.0); 
  else 
    glOrtho(-w/h, w/h, -1.0, 1.0, -10.0, 10.0);
  
  glMatrixMode(GL_MODELVIEW); //�}�`�`�惂�[�h�֐؂�ւ�
} 

//OpenGL �̏�����
void myinit(){
  //�����̓���
  //�ۑ�4�̌����̒ǉ��͈ȉ����R�s�[���ă��C�g��1�Ԃ�ON�ɂ���
  GLfloat light_ambient[]={1.0, 1.0, 1.0, 1.0};
  GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
  GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
  
  //�����̓����� OpenGL �֑���
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  
  glEnable(GL_LIGHTING); //���C�g���g��
  glEnable(GL_LIGHT0);   //���C�g0�Ԃ��I��
  
  //�I�u�W�F�N�g�̔��˓���
  //�ۑ�R�̔��˓����͈ȉ��̒l��ύX����
  GLfloat mat_specular[]={0.5, 0.5, 0.5, 1.0};
  GLfloat mat_diffuse[]={0.5, 0.5, 1.0, 1.0};
  GLfloat mat_ambient[]={0.1, 0.1, 0.2, 1.0}; 
  GLfloat mat_shininess=20.0;
  
  //�I�u�W�F�N�g�̔��˓����� OpenGL �֑���
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
  
  //���̑��̐ݒ�
  glShadeModel(GL_FLAT);   //�|���S���x�^�h��
  glEnable(GL_DEPTH_TEST); //�A�ʏ������s��
  
  glClearColor (1.0, 1.0, 1.0, 1.0); //�w�i�̐F
}

int main(int argc, char** argv){
  //�E�B���h�E�̏�����
  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //�g�p����o�b�t�@�̐ݒ�
  glutInitWindowSize(500, 500); //�E�B���h�E�̑傫��
  glutCreateWindow("1-2"); //�E�B���h�E�̃^�C�g��
  
  myinit();//OpenGL �̏�����
  glutReshapeFunc(myReshape);//�E�B���h�E�T�C�Y���ύX���ꂽ���ɌĂ΂��֐���ݒ�
  glutDisplayFunc(display); //�E�B���h�E�̍X�V�̂��߂̊֐���ݒ�
  glutMainLoop(); //���C�����[�v
  
  return 0;
}

