#if defined(_WIN64)||defined(_WIN32)
#include <GL/glut.h>    			// Header File For The GLUT Library
#include <GL/gl.h>
#include <GL/glu.h>
#elif __APPLE__
#include <GLUT/glut.h>
#elif __linux
#include <GL/glut.h>    			// Header File For The GLUT Library
#endif /* OS */

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <boost/thread.hpp>
#include <math.h>
#include "Leap.h"

using namespace Leap;
using namespace std;

/* ASCII code for the escape key. */
#define KEY_ESCAPE 27
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

int window;									// The number of our GLUT window
bool fullScreen=true;						// toggle for fullscreen mode
int screen_width = DEFAULT_WIDTH;
int screen_height = DEFAULT_HEIGHT;
const static long font=(long)GLUT_BITMAP_HELVETICA_10;		// For printing bitmap fonts
char s[30];
boost::mutex mtx;							// Exclusive control

Controller controller;
Screen screen;

vector<Vector> vpoint;

class SampleListener : public Listener {
	public:
		virtual void onInit(const Controller&);
		virtual void onConnect(const Controller&);
		virtual void onDisconnect(const Controller&);
		virtual void onExit(const Controller&);
		virtual void onFrame(const Controller&);
};

void SampleListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
/*	std::cout << "Frame id: " << frame.id()
						<< ", timestamp: " << frame.timestamp()
						<< ", hands: " << frame.hands().count()
						<< ", fingers: " << frame.fingers().count()
						<< ", tools: " << frame.tools().count() << std::endl;
*/
	if (!frame.hands().empty()) {
		// get mutex
		boost::mutex::scoped_lock lk(mtx);
		
		// clear last datas
		vpoint.clear();

		const PointableList plist = frame.pointables();
		for(int i=0;i<plist.count();i++){
			Vector v = screen.intersect(plist[i], true);
			vpoint.push_back(v);
		}
		
	}
}

// Create a sample listener and controller
SampleListener listener;

void CleanupExit()
{
	// Remove the sample listener when done
	controller.removeListener(listener);

	exit (1);
}

void glPrint(float x, float y, void *font, char *string) {
	char *c;
	glRasterPos2f(x, y);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void DrawGLScene()
{
	if (fullScreen)
		glutFullScreen();
	else
		glutReshapeWindow(screen_width, screen_height);

	glClear(GL_COLOR_BUFFER_BIT);

	glViewport( 0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3ub(255, 255, 255);								// Draw In White

	// get mutex
	boost::mutex::scoped_lock lk(mtx);
	
	if(!vpoint.empty()){
		glPointSize(10);
		glBegin(GL_POINTS);
		for(int i=0;i<vpoint.size();i++){
			glVertex2f(vpoint[i][0], vpoint[i][1]);
		}
		glEnd();

		sprintf(s,"Point: %.2f : %.2f", vpoint[0][0], vpoint[0][1]);
		glPrint(0.1f, 0.88f, (void *)font, s);	// Print slowMotionRatio
	}
	
	glFlush ();					// Flush The GL Rendering Pipeline
	glutSwapBuffers();	// swap buffers to display, since we're double buffered.
}

void Idle(void)
{
	// Display the frame
	glutPostRedisplay();
}

/*
 * @brief The function called whenever a normal key is pressed.
 */
void NormalKeyPressed(unsigned char keys, int x, int y)
{
	switch(keys){
	  case KEY_ESCAPE:
		CleanupExit();
	}
}

/*
 * @brief The function called whenever a special key is pressed.
 */
void SpecialKeyPressed(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_F1:
			fullScreen = !fullScreen;								// Toggle Fullscreen Mode
			break;

		case GLUT_KEY_F2:
			break;

		case GLUT_KEY_F3:
			break;

	}
}

/*
 * @brief The function called when our window is resized (which shouldn't happen, because we're fullscreen)
 */
void ReSizeGLScene(int Width, int Height)
{
	if(Height==0)						// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;
	screen_width = Width;
	screen_height = Height;
}

int main(int argc , char ** argv) {
	screen = controller.calibratedScreens()[0];
	if(!screen.isValid()){
		cout << "This screen has not been calibrated." << endl;
		return 0;
	}

	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("LEAP intersection Test");
	glutDisplayFunc(&DrawGLScene);
	glutIdleFunc(&Idle);
	glutReshapeFunc(&ReSizeGLScene);
	glutKeyboardFunc(&NormalKeyPressed);
	glutSpecialFunc(&SpecialKeyPressed);

	glDisable(GL_DEPTH_TEST);								// Disables Depth Testing
	glShadeModel(GL_SMOOTH);								// Enables Smooth Color Shading
	
	glutMainLoop();

	return 0;
}
