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
// C++에는 이렇게 자료구조들을 인클루드 할수 있다.

void drawHand(float angle, mat4 CTM,int level = 1)
{
	if (level < 1)
		return;
	std::stack<mat4> M_stk;
	//3.lower arm
	
	mat4 m3 = Translate(0, 0.05, 0)*RotateZ(-90)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//항상 스케일링->회전->이동
	cube.Draw(program);

	M_stk.push(CTM);

	//4. hand - finger 1
	CTM *= Translate(0.2, 0.0, 0)*RotateZ(angle);
	mat4 m4 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//항상 스케일링->회전->이동
	cube.Draw(program);

	CTM = M_stk.top();//top은 제일 위에있는 것을 return
	M_stk.pop();//pop은 return 하지않고 제일 위에있는 것을 없앤다.
				//5. hand - finger 2
	CTM *= Translate(-0.2, 0.0, 0)*RotateZ(-angle);
	mat4 m5 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m5);//항상 스케일링->회전->이동
	cube.Draw(program);

	level = level - 1;
	M_stk.push(CTM);
	CTM *= Translate(0, 0.4, 0)*RotateZ(0);
	drawHand(angle, CTM,level);

	CTM = M_stk.top();//top은 제일 위에있는 것을 return
	M_stk.pop();
	CTM *= Translate(0, 0.4, 0)*RotateZ(0);
	drawHand(angle, CTM,level);
}
void drawRobotArm2(float angle[])//base는 고정이고 upper arm과 lower arm과 hand가 얼마나 z축 회전 되었는지 받기위하여
{
	std::stack<mat4> M_stk;//mat4(data type)를 저장할 수 있는 stack이라는 뜻
							//templete이라고 하는 문법이다.
	
	glUseProgram(program);

	mat4 CTM = mat4(1.0);					// current transform matrix

											//1.base
	CTM *= Translate(0, -0.5, 0)*RotateY(time * 90);	//모델을 원하는 위치에 가져다 주는것
	mat4 m1 = Scale(0.3, 0.2, 0.2);			//모양을 잡아주는 것
	glUniformMatrix4fv(uMat, 1, true, CTM*m1);
	pyramid.Draw(program);

	//2.upper arm
	CTM *= RotateZ(angle[0]);//위의 관절에 비해 나는 얼마만큼 틀어져있는가로 생각한다.
	mat4 m2 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m2);//항상 스케일링->회전->이동
	cube.Draw(program);

	/*//3.lower arm
	CTM *= Translate(0, 0.4, 0)*RotateZ(angle[1]);
	mat4 m3 = Translate(0, 0.05, 0)*RotateZ(-90)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//항상 스케일링->회전->이동
	cube.Draw(program);

	M_stk.push(CTM);

	//4. hand - finger 1
	CTM *= Translate(0.2,0.0, 0)*RotateZ(angle[2]);
	mat4 m4 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//항상 스케일링->회전->이동
	cube.Draw(program);
	{
		//3.lower arm
	CTM *= Translate(0, 0.4, 0)*RotateY(90)*RotateZ(angle[1]);
	mat4 m3 = Translate(0, 0.05, 0)*RotateZ(-90)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//항상 스케일링->회전->이동
	cube.Draw(program);

	M_stk.push(CTM);

	//4. hand - finger 1
	CTM *= Translate(0.2, 0.0, 0)*RotateZ(angle[2]);
	mat4 m4 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//항상 스케일링->회전->이동
	cube.Draw(program); 

	CTM = M_stk.top();//top은 제일 위에있는 것을 return
	M_stk.pop();//pop은 return 하지않고 제일 위에있는 것을 없앤다.
				//5. hand - finger 2
	CTM *= Translate(-0.2, 0.0, 0)*RotateZ(-angle[2]);
	mat4 m5 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m5);//항상 스케일링->회전->이동
	cube.Draw(program);
	}// 변수 충돌할까봐 중괄호 안에 넣어줌
	CTM = M_stk.top();//top은 제일 위에있는 것을 return
	M_stk.pop();//pop은 return 하지않고 제일 위에있는 것을 없앤다.
	//5. hand - finger 2
	CTM *= Translate(-0.2, 0.0, 0)*RotateZ(-angle[2]);
	mat4 m5 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m5);//항상 스케일링->회전->이동
	cube.Draw(program);*/
	
	
	//3.hand
	CTM *= Translate(0, 0.4, 0)*RotateZ(angle[2]);
	drawHand(angle[2], CTM,2);
}
void drawRobotArm(float angle[])//base는 고정이고 upper arm과 lower arm과 hand가 얼마나 z축 회전 되었는지 받기위하여
{
	glUseProgram(program);

	mat4 CTM = mat4(1.0);					// current transform matrix

	//1.base
	CTM *= Translate(0, -0.5, 0)*RotateY(time * 90);	//모델을 원하는 위치에 가져다 주는것
	mat4 m1 = Scale(0.3, 0.2, 0.2);			//모양을 잡아주는 것
	glUniformMatrix4fv(uMat, 1, true, CTM*m1);
	pyramid.Draw(program);

	//2.upper arm
	CTM *= RotateZ(angle[0]);//위의 관절에 비해 나는 얼마만큼 틀어져있는가로 생각한다.
	mat4 m2 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m2);//항상 스케일링->회전->이동
	cube.Draw(program);

	//3.lower arm
	CTM *= Translate(0, 0.4, 0)*RotateZ(angle[1]);
	mat4 m3 = Translate(0, 0.2, 0)*Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*m3);//항상 스케일링->회전->이동
	cube.Draw(program);

	//4. hand - finger 1
	CTM *= Translate(0, 0.2, 0)*RotateZ(angle[2]);
	mat4 m4 = Translate(0, 0.1, 0)*Scale(0.3, 0.2, 0.2);
	glUniformMatrix4fv(uMat, 1, true, CTM*m4);//항상 스케일링->회전->이동
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
							//다리로 하면 hop, knee, ankle
							//허리로 하면 spine, neck

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