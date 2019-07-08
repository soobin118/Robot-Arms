#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"
#include "MyTarget.h"

MyCube cube;
MyPyramid pyramid;
MyTarget target(&cube);

GLuint program;
GLuint uMat;

mat4 CTM;

vec4 d;

bool bPlay = false;
bool bChasingTarget = false;
bool bDrawTarget = false;

float ang1 = 0;
float ang2 = 0;
float ang3 = 0;

float e ;
float paste;

int flag1 = 1;
int flag2 = 1;
int flag3 = 1;

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}

float time = 0;

void findposition(float ang1, float ang2, float ang3) {
	mat4 temp = CTM;
	d = vec4(0.5, 0, 0, 1);
	CTM = Translate(0, -0.4, 0) * RotateY(time * 30);
	CTM *= RotateZ(ang1);
	CTM *= Translate(0, 0.4, 0) * RotateZ(ang2); 
	CTM *= Translate(0, 0.4, 0)	* RotateZ(ang3);
	d = CTM*Scale(0.4,0,0)*d;
	CTM = temp;
}

void drawRobotArm(float ang1, float ang2, float ang3)
{
	mat4 temp = CTM;
	// BASE-1
	mat4 M(1.0);
	CTM = Translate(0, -0.4, 0) * RotateY(time * 30);
	M = Translate(0, 0, 0.08)*Scale(0.3, 0.3, 0.06);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	pyramid.Draw(program);

	// BASE-2
	M = Translate(0, 0, -0.08)*Scale(0.3, 0.3, 0.06);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	pyramid.Draw(program);

	// BASE-Joint
	M = Scale(0.05, 0.05, 0.25);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	// Upper Arm
	CTM *= RotateZ(ang1);
	M = Translate(0, 0.2, 0) *  Scale(0.1, 0.5, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	// Lower Arm-1
	CTM *= Translate(0, 0.4, 0) * RotateZ(ang2);
	M = Translate(0, 0.2, -0.075) * Scale(0.1, 0.5, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	// Lower Arm-2
	M = Translate(0, 0.2, 0.075) * Scale(0.1, 0.5, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	// Arm-Joint
	M = Scale(0.05, 0.05, 0.25);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	// Hand
	CTM *= Translate(0, 0.4, 0)	* RotateZ(ang3);
	M =  Scale(0.4, 0.15, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	// Hand-Joint
	M = Scale(0.05, 0.05, 0.25);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	findposition(ang1, ang2, ang3);
	//Debugging
	/*M = Translate(d)*Scale(0.1, 0.1, 0.1);
	glUniformMatrix4fv(uMat, 1, true, M);
	cube.Draw(program);*/
	CTM = temp;
}

float err(vec3 p)
{
	findposition(ang1, ang2, ang3);
	if ((int)(time * 30) % 360 >= 90.0 && (int)(time * 30) % 360 <= 270.0) {
			return ((d.x + p.x)*(d.x + p.x)) + ((d.y + 0.4 - p.y)*(d.y + 0.4 - p.y)) + ((d.z - p.z)*(d.z - p.z));
	}
	else
		return ((d.x - p.x)*(d.x - p.x)) + ((d.y + 0.4 - p.y)*(d.y + 0.4 - p.y)) + ((d.z - p.z)*(d.z - p.z));
}

void computeAngle()
{
	vec3 tposition = target.GetPosition(time);
	e = err(tposition);

	for (int i = 0; i < 10; i++)
	{
		if (e < 0.01)
			break;

		/*-------ang1-------*/
		if (flag1 == 1)
			ang1 += 0.57;
		else
			ang1 -= 0.57;

		paste = e;
		e = err(tposition);
		if (e - paste > 0)
		{
			if (flag1 == 1)
			{
				flag1 = 0;
				ang1 -= 1.14;
				paste = e;
				e = err(tposition);
			}
			else
			{
				flag1 = 1;
				ang1 += 1.14;
				paste = e;
				e = err(tposition);
			}
		}

		if (e < 0.01)
			break;

		/*-------ang2-------*/
		if (flag2 == 1)
			ang2 += 0.28;
		else
			ang2 -= 0.28;

		paste = e;
		e = err(tposition);
		if (e - paste > 0)
		{
			if (flag2 == 1)
			{
				flag2 = 0;
				ang2 -= 0.57;
				paste = e;
				e = err(tposition);
			}
			else
			{
				flag2 = 1;
				ang2 += 0.57;
				paste = e;
				e = err(tposition);
			}
		}

		if (e < 0.01)
			break;

		/*-------ang3-------*/
		if (flag3 == 1)
			ang3 += 0.14;
		else
			ang3 -= 0.14;

		paste = e;
		e = err(tposition);
		if (e - paste > 0)
		{
			if (flag3 == 1)
			{
				flag3 = 0;
				ang3 -= 0.28;
				paste = e;
				e = err(tposition);
			}
			else
			{
				flag3 = 1;
				ang3 += 0.28;
				paste = e;
				e = err(tposition);
			}
		}

	}
}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint uColor = glGetUniformLocation(program, "uColor");
	glUniform4f(uColor, -1, -1, -1, -1);


	uMat = glGetUniformLocation(program, "uMat");
	CTM = Translate(0, -0.4, 0) * RotateY(time * 30);
	drawRobotArm(ang1, ang2, ang3);


	glUniform4f(uColor, 1, 0, 0, 1);
	if (bDrawTarget == true)
		target.Draw(program, CTM, time);

	glutSwapBuffers();
}

void myIdle()
{
	if (bPlay)
	{
		time += 1 / 60.0f;
		Sleep(1 / 60.0f * 1000);

		if (bChasingTarget == false)
		{
			ang1 = 45 * sin(time * 3.141592);
			ang2 = 60 * sin(time * 2 * 3.141592);
			ang3 = 30 * sin(time * 3.141592);
		}
		else
		{
			computeAngle();
		}
		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char c, int x, int y)
{

	switch (c)
	{
	case '1':
		bChasingTarget = !bChasingTarget;
		break;
	case '2':
		bDrawTarget = !bDrawTarget;
		break;
	case '3':
		target.toggleRandom();
		break;
	case ' ':
		bPlay = !bPlay;
		break;
	default:
		break;
	}
}


int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Simple Robot Arm");

	printf("SIMPLE INTELLIGENT ROBOT ARM\n");
	printf("Programming Assignment #2 for Computer Graphics 2\n");
	printf("Department of Digital Contents, Sejong University\n");
	printf("\n----------------------------------------------------------------\n");
	printf("'1' key: Toggle Between Chasing the target or not\n");
	printf("'2' key: Toggle Between Showing the target position or not\n");
	printf("'3' key: Toggle Between Randomly Positioning the target or not\n");
	printf("\nSpacebar: start/stop Playing\n");
	printf("----------------------------------------------------------------");

	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}