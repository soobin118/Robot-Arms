#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"

MyCube cube;
MyPyramid pyramid;

GLuint program;
GLuint uMat;

mat4 g_Mat = mat4(1.0);
float time = 0.0;

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	uMat = glGetUniformLocation(program, "uMat");


}

#include <stack>
// #include <list> - linked list
// #include <que> - queue
// C++���� �̷��� �ڷᱸ������ ��Ŭ��� �Ҽ� �ִ�.

void drawHand(float angle, mat4 CTM,int level = 1)
{
	if (level < 1)
		return;
	std::stack<mat4> M_stk;
	//3.lower arm
	
	mat4 m3 = Translate(0, 0.05, 0)*RotateZ(-90)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	M_stk.push(CTM);

	//4. hand - finger 1
	CTM *= Translate(0.2, 0.0, 0)*RotateZ(angle);
	mat4 m4 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	CTM = M_stk.top();//top�� ���� �����ִ� ���� return
	M_stk.pop();//pop�� return �����ʰ� ���� �����ִ� ���� ���ش�.
				//5. hand - finger 2
	CTM *= Translate(-0.2, 0.0, 0)*RotateZ(-angle);
	mat4 m5 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m5);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	level = level - 1;
	M_stk.push(CTM);
	CTM *= Translate(0, 0.4, 0)*RotateZ(0);
	drawHand(angle, CTM,level);

	CTM = M_stk.top();//top�� ���� �����ִ� ���� return
	M_stk.pop();
	CTM *= Translate(0, 0.4, 0)*RotateZ(0);
	drawHand(angle, CTM,level);
}
void drawRobotArm2(float angle[])//base�� �����̰� upper arm�� lower arm�� hand�� �󸶳� z�� ȸ�� �Ǿ����� �ޱ����Ͽ�
{
	std::stack<mat4> M_stk;//mat4(data type)�� ������ �� �ִ� stack�̶�� ��
							//templete�̶�� �ϴ� �����̴�.
	
	glUseProgram(program);

	mat4 CTM = mat4(1.0);					// current transform matrix

											//1.base
	CTM *= Translate(0, -0.5, 0)*RotateY(time * 90);	//���� ���ϴ� ��ġ�� ������ �ִ°�
	mat4 m1 = Scale(0.3, 0.2, 0.2);			//����� ����ִ� ��
	glUniformMatrix4fv(uMat, 1, true, CTM*m1);
	pyramid.Draw(program);

	//2.upper arm
	CTM *= RotateZ(angle[0]);//���� ������ ���� ���� �󸶸�ŭ Ʋ�����ִ°��� �����Ѵ�.
	mat4 m2 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m2);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	/*//3.lower arm
	CTM *= Translate(0, 0.4, 0)*RotateZ(angle[1]);
	mat4 m3 = Translate(0, 0.05, 0)*RotateZ(-90)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	M_stk.push(CTM);

	//4. hand - finger 1
	CTM *= Translate(0.2,0.0, 0)*RotateZ(angle[2]);
	mat4 m4 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);
	{
		//3.lower arm
	CTM *= Translate(0, 0.4, 0)*RotateY(90)*RotateZ(angle[1]);
	mat4 m3 = Translate(0, 0.05, 0)*RotateZ(-90)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	M_stk.push(CTM);

	//4. hand - finger 1
	CTM *= Translate(0.2, 0.0, 0)*RotateZ(angle[2]);
	mat4 m4 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program); 

	CTM = M_stk.top();//top�� ���� �����ִ� ���� return
	M_stk.pop();//pop�� return �����ʰ� ���� �����ִ� ���� ���ش�.
				//5. hand - finger 2
	CTM *= Translate(-0.2, 0.0, 0)*RotateZ(-angle[2]);
	mat4 m5 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m5);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);
	}// ���� �浹�ұ�� �߰�ȣ �ȿ� �־���
	CTM = M_stk.top();//top�� ���� �����ִ� ���� return
	M_stk.pop();//pop�� return �����ʰ� ���� �����ִ� ���� ���ش�.
	//5. hand - finger 2
	CTM *= Translate(-0.2, 0.0, 0)*RotateZ(-angle[2]);
	mat4 m5 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m5);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);*/
	
	
	//3.hand
	CTM *= Translate(0, 0.4, 0)*RotateZ(angle[2]);
	drawHand(angle[2], CTM,2);
}
void drawRobotArm(float angle[])//base�� �����̰� upper arm�� lower arm�� hand�� �󸶳� z�� ȸ�� �Ǿ����� �ޱ����Ͽ�
{
	glUseProgram(program);

	mat4 CTM = mat4(1.0);					// current transform matrix

	//1.base
	CTM *= Translate(0, -0.5, 0)*RotateY(time * 90);	//���� ���ϴ� ��ġ�� ������ �ִ°�
	mat4 m1 = Scale(0.3, 0.2, 0.2);			//����� ����ִ� ��
	glUniformMatrix4fv(uMat, 1, true, CTM*m1);
	pyramid.Draw(program);

	//2.upper arm
	CTM *= RotateZ(angle[0]);//���� ������ ���� ���� �󸶸�ŭ Ʋ�����ִ°��� �����Ѵ�.
	mat4 m2 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m2);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	//3.lower arm
	CTM *= Translate(0, 0.4, 0)*RotateZ(angle[1]);
	mat4 m3 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);

	//4. hand - finger 1
	CTM *= Translate(0, 0.2, 0)*RotateZ(angle[2]);
	mat4 m4 = Translate(0, 0.1, 0)*Scale(0.3, 0.2, 0.2);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//�׻� �����ϸ�->ȸ��->�̵�
	cube.Draw(program);
}
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	float angle[3];
	angle[0] = -30*sin(time*3);			// shoulder joint angle
	angle[1] = 60*sin(time*6);			// elbow joint angle
	angle[2] = 20*sin(time*6);			// wrist joint angle
							//�ٸ��� �ϸ� hop, knee, ankle
							//�㸮�� �ϸ� spine, neck

	drawRobotArm2(angle);

	glutSwapBuffers();
}
void myIdle()
{
	time += 0.033;
	Sleep(33);
	glutPostRedisplay();
}



int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Cube and Pyramid");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}