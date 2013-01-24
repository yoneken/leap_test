#include "myglut.h"
#include <GLUT/glut.h>

/*
 * @brief Function for displaying bitmap fonts on the screen
 */
void glPrint(float x, float y, void *font, char *string) {

	char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void glDrawSpherer(float x, float y, float z, float radius){
	glPushMatrix();
		glTranslatef(x, z, -y);
		glutSolidSphere(radius, 6, 6);
	glPopMatrix();
}
void glDrawSpherer(Eigen::Vector3f v, float radius){
	glDrawSpherer(v(0), v(1), v(2), radius);
}

/*
 * @brief Function for displaying pipe
 */
void glDrawPiped(double x0, double y0, double z0,
 double x1, double y1, double z1,
 double r)
{
	GLUquadricObj *arrow;
	double x2, y2, z2, len, ang;

	x2 = x1-x0; y2 = y1-y0; z2 = z1-z0;
	len = sqrt(x2*x2 + y2*y2 + z2*z2);
	if(len != 0.0){
		ang = acos(z2*len/(sqrt(x2*x2+y2*y2+z2*z2)*len))/M_PI*180.0;

		glPushMatrix();
			glTranslated( x0, y0, z0);
			glRotated( ang, -y2*len, x2*len, 0.0f);
			arrow = gluNewQuadric();
			gluQuadricDrawStyle(arrow, GLU_FILL);
			gluCylinder(arrow, r, r, len, 8, 8);
		glPopMatrix();
	}
}
void glDrawPiped(Eigen::Vector3d start, Eigen::Vector3d finish, double thickness)
{
	glDrawPiped(start(0), start(1), start(2), finish(0), finish(1), finish(2), thickness);
}
void glDrawPipedr(Eigen::Vector3d start, Eigen::Vector3d finish, double thickness)
{
	glDrawPiped(start(0), start(2), -start(1), finish(0), finish(2), -finish(1), thickness);
}	

/*
 * @brief Function for displaying arrow
 */
void glDrawArrowd(double x0, double y0, double z0,
 double x1, double y1, double z1)
{
	GLUquadricObj *arrows[2];
	double x2, y2, z2, len, ang;

	x2 = x1-x0; y2 = y1-y0; z2 = z1-z0;
	len = sqrt(x2*x2 + y2*y2 + z2*z2);
	if(len != 0.0){
		ang = acos(z2*len/(sqrt(x2*x2+y2*y2+z2*z2)*len))/M_PI*180.0;

		glPushMatrix();
			glTranslated( x0, y0, z0);
			glRotated( ang, -y2*len, x2*len, 0.0);
			arrows[0] = gluNewQuadric();
			gluQuadricDrawStyle(arrows[0], GLU_FILL);
			gluCylinder(arrows[0], len/80, len/80, len*0.9, 8, 8);
			glPushMatrix();
				glTranslated( 0.0, 0.0, len*0.9);
				arrows[1] = gluNewQuadric();
				gluQuadricDrawStyle(arrows[1], GLU_FILL);
				gluCylinder(arrows[1], len/30, 0.0f, len/10, 8, 8);
			glPopMatrix();
		glPopMatrix();
	}
}
void glDrawArrowd(Eigen::Vector3d start, Eigen::Vector3d finish)
{
	glDrawArrowd(start(0), start(1), start(2), finish(0), finish(1), finish(2));
}
void glDrawArrowdr(Eigen::Vector3d start, Eigen::Vector3d finish)
{
	glDrawArrowd(start(0), start(2), -start(1), finish(0), finish(2), -finish(1));
}	

/*
 * @brief Function for displaying axis
 */
void glDrawAxis(float length, void *font) {
	glBegin(GL_LINES);
		// Draw Axis
		glVertex3f(-length, 0, 0);
		glVertex3f( length, 0, 0);
		glVertex3f( 0, length, 0);
		glVertex3f( 0,-length, 0);
		glVertex3f( 0, 0, length);
		glVertex3f( 0, 0,-length);
	glEnd();

	glPrint(length+0.1f, 0.0f, font, "X");
	glPrint(0.0f, length+0.1f, font, "Y");
	glPrint(-length-0.2f, -0.4f, font, "Z");

	glPushMatrix();
		glTranslatef(length, 0.0f, 0.0f);
		glutSolidSphere(0.02f, 6, 6);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, length,0.0f);
		glutSolidSphere(0.02f, 6, 6);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, length);
		glutSolidSphere(0.02f, 6, 6);
	glPopMatrix();
}
void glDrawAxisr(float length, void *font) {
	Eigen::Vector3d x, y, z, g;
	x << length, 0.0, 0.0;
	y << 0.0, length, 0.0;
	z << 0.0, 0.0, length;
	g << 0.0, 0.0, 0.0;

	glDrawArrowdr(g, x);
	glDrawArrowdr(g, y);
	glDrawArrowdr(g, z);

	glPrint(length+0.1f, 0.0f, font, "X");
	glPrint(0.0f, length+0.1f, font, "Z");
	//glPrint(-length-0.2f, -0.4f, font, "Z");
}

void glDrawMatrix3fr(Eigen::Matrix3f m, Eigen::Vector3f origin){
	Eigen::Vector3f x, y, z;
	x << m(0), m(1), m(2);
	y << m(3), m(4), m(5);
	z << m(6), m(7), m(8);

	glColor3ub(255,  60,  60);
	glDrawArrowdr(origin.cast<double>(), x.cast<double>() + origin.cast<double>());
	glColor3ub( 60, 255,  60);
	glDrawArrowdr(origin.cast<double>(), y.cast<double>() + origin.cast<double>());
	glColor3ub( 60,  60, 255);
	glDrawArrowdr(origin.cast<double>(), z.cast<double>() + origin.cast<double>());
}
void glDrawMatrix3fr(Eigen::Matrix3f m){
	Eigen::Vector3f g = Eigen::Vector3f::Zero();
	glDrawMatrix3fr(m, g);
}
	
void gluLookAtr(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ){
	gluLookAt(eyeX, eyeZ, -eyeY, centerX, centerZ, -centerY, upX, upZ, -upY);
}
