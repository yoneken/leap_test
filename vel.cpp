#if defined(_WIN64)||defined(_WIN32)
#include <GL/glut.h>    			// Header File For The GLUT Library
#include <GL/gl.h>
#include <GL/glu.h>
#elif __APPLE__
#include <GLUT/glut.h>
#elif __linux
#include <GL/glut.h>    			// Header File For The GLUT Library
#endif /* OS */

#include <vector>
#include <deque>
#include <math.h>
#include <boost/thread.hpp>
#include "Leap.h"
#include "myglut.h"

using namespace Leap;
using namespace Eigen;
using namespace std;

/* ASCII code for the escape key. */
#define KEY_ESCAPE 27
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 580

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

int window;									// The number of our GLUT window
int last_time,timeprev=0;						// For calculating elapsed time
bool fullScreen=false;						// toggle for fullscreen mode
int screen_width = DEFAULT_WIDTH;
int screen_height = DEFAULT_HEIGHT;
double slowMotionRatio = 10.0;				// slowMotionRatio Is A Value To Slow Down The Simulation, Relative To Real World Time
double timeElapsed = 0.0;					// Elapsed Time In The Simulation (Not Equal To Real World's Time Unless slowMotionRatio Is 1
const static long font=(long)GLUT_BITMAP_HELVETICA_10;		// For printing bitmap fonts
boost::mutex mtx;							// Exclusive control

Vector3fv vfinger;
Matrix3fv mhand;
vector<float> rhand;
Vector3fd vhand(640, Vector3f::Zero());
//Vector3fv vhand;
Vector3fd vvhand(640, Vector3f::Zero());

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
		rhand.clear();
		vhand.clear();
		mhand.clear();
		vfinger.clear();

		// set new datas
		Vector3f v, x, y, z;
		Matrix3f m;
		Vector tmp, normal, direction;

		const HandList hlist = frame.hands();
//		for(int i=0;i<hlist.count();i++){
		int i=0;
			rhand.push_back(hlist[i].sphereRadius());
			tmp = hlist[i].palmPosition();
			v << -tmp[2]/1000.0f, -tmp[0]/1000.0f, tmp[1]/1000.0f;
			vhand.pop_front();
			vhand.push_back(v);

			tmp = hlist[i].palmVelocity();
			v << -tmp[2]/1000.0f, -tmp[0]/1000.0f, tmp[1]/1000.0f;
			vvhand.pop_front();
			vvhand.push_back(v);

			normal = hlist[i].palmNormal();
			direction = hlist[i].direction();
			y << -normal[2], -normal[0], normal[1];
			z << -direction[2], -direction[0], direction[1];
			z = -z;
			x = y.cross(z);
			z = x.cross(y);
			m << x[0], x[1], x[2],
					 y[0], y[1], y[2],
					 z[0], z[1], z[2];
			mhand.push_back(m);
//		}
		
		// Check if the hand has any fingers
		const FingerList fingers = frame.fingers();
		for(int i=0;i<fingers.count();i++){
			tmp = fingers[i].tipPosition();
			v << -tmp[2]/1000.0f, -tmp[0]/1000.0f, tmp[1]/1000.0f;
			vfinger.push_back(v);
		}
	}
}

// Create a sample listener and controller
SampleListener listener;
Controller controller;

void CleanupExit()
{
	// Remove the sample listener when done
	controller.removeListener(listener);

	exit (1);
}

void DrawGLScene()
{
	if (fullScreen)
		glutFullScreen();
	else
		glutReshapeWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	last_time=glutGet(GLUT_ELAPSED_TIME);
	int milliseconds = last_time - timeprev;
	timeprev = last_time;

	float dt = milliseconds / 1000.0f;						// Let's Convert Milliseconds To Seconds

	dt /= slowMotionRatio;									// Divide dt By slowMotionRatio And Obtain The New dt
	timeElapsed += dt;										// Iterate Elapsed Time

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	glViewport(0, screen_height-480, screen_width, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width/(GLfloat)(480), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

// Position Camera 40 Meters Up In Z-Direction.
	// Set The Up Vector In Y-Direction So That +X Directs To Right And +Y Directs To Up On The Window.
	//gluLookAtr( 2.0, 0.5, 1.2, 0.0, 0.0, 0.4, 0, 0, 1);
	gluLookAtr(-1.0, 0.3, 0.8, 0.0, 0.0, 0.3, 0, 0, 1);

	// Drawing The Coordinate Plane Starts Here.
	glColor3ub(255, 255, 0);								// Draw In Yellow
	//glDrawAxis(0.5f, (void *)font);
	glDrawAxisr(0.5f, (void *)font);
	
	glColor3ub(255,	 0, 255);

	Vector3d orig;
	orig << 0.0, 0.0, 0.0;
	
	// get mutex
	boost::mutex::scoped_lock lk(mtx);
	
	//	for(int i=0;i<vhand.size();i++){
	int i=0;
		glDrawArrowdr(orig, vhand[i].cast<double>());
		//glDrawMatrix3fr(rhand[i] * mhand[i] / 500.0f, vhand[i]);
//	}

	glColor3ub(  0,	255, 255);
	for(int i=0;i<vfinger.size();i++){
		glDrawSpherer(vfinger[i], 0.01);
	}

/*
	glColor3ub(255, 255, 255);								// Draw In White
	sprintf(s,"Time elapsed (seconds): %.2f",timeElapsed);
	glPrint(-1.0f, 0.1, (void *)font, s);	// Print timeElapsed
	sprintf(s,"Slow motion ratio: %.2f", slowMotionRatio);
	glPrint(-0.9f, 0.1, (void *)font, s);	// Print slowMotionRatio
	glPrint(-0.8f, 0.1, (void *)font, "Press F2 for normal motion");
	glPrint(-0.7f, 0.1, (void *)font, "Press F3 for slow motion");
*/

	glViewport(0, 0, screen_width, screen_height - 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screen_width, 0.0, screen_height - 480);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw line
	glColor3ub(255, 255, 255);
	glBegin(GL_LINE_STRIP);
		glVertex2i(  0, 100);
		glVertex2i(640, 100);
	glEnd();
/*
	// Draw position
	if(!vhand.empty()){
		cout << "test " << (int(vhand[0][0]*300.0)+50) << endl;
		glColor3ub(255,   0,   0);
		for(int i=1;i<vhand.size();i++){
			glBegin(GL_LINE_STRIP);
				glVertex2i(i-1, int(vhand[i-1][0]*300.0)+50);
				glVertex2i(  i, int(vhand[i][0]*300.0)+50);
			glEnd();
		}
		glColor3ub(  0, 255,   0);
		for(int i=1;i<vhand.size();i++){
			glBegin(GL_LINE_STRIP);
				glVertex2i(i-1, int(vhand[i-1][1]*300.0)+50);
				glVertex2i(  i, int(vhand[i][1]*300.0)+50);
			glEnd();
		}
		glColor3ub(  0,   0, 255);
		for(int i=1;i<vhand.size();i++){
			glBegin(GL_LINE_STRIP);
				glVertex2i(i-1, int(vhand[i-1][2]*300.0)+50);
				glVertex2i(  i, int(vhand[i][2]*300.0)+50);
			glEnd();
		}
	}
*/
	// Draw velocity
	if(!vvhand.empty()){
		glColor3ub(255, 255,   0);
		for(int i=1;i<vvhand.size();i++){
			glBegin(GL_LINE_STRIP);
				glVertex2i(i-1, int(vvhand[i-1][0]*20+50));
				glVertex2i(  i, int(vvhand[i][0]*20+50));
			glEnd();
		}
		glColor3ub(  0, 255, 255);
		for(int i=1;i<vvhand.size();i++){
			glBegin(GL_LINE_STRIP);
				glVertex2i(i-1, int(vvhand[i-1][1]*20+50));
				glVertex2i(  i, int(vvhand[i][1]*20+50));
			glEnd();
		}
		glColor3ub(255,   0, 255);
		for(int i=1;i<vvhand.size();i++){
			glBegin(GL_LINE_STRIP);
				glVertex2i(i-1, int(vvhand[i-1][2]*20+50));
				glVertex2i(  i, int(vvhand[i][2]*20+50));
			glEnd();
		}
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
			slowMotionRatio = 1.0f;									// Set slowMotionRatio To 1.0f (Normal Motion)
			break;

		case GLUT_KEY_F3:
			slowMotionRatio = 10.0f;								// Set slowMotionRatio To 10.0f (Very Slow Motion)
			break;

	}
}

/*
 * @brief A general OpenGL initialization function.	Sets all of the initial parameters.
 */
void InitGL(int Width, int Height)					// We call this right after our OpenGL window is created.
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// This Will Clear The Background Color To Black
	glClearDepth(1.0);										// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);									// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
	glShadeModel(GL_SMOOTH);								// Enables Smooth Color Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		// Set Perspective Calculations To Most Accurate
/*
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
*/
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
/*
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
*/
}

int main(int argc , char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glutInitWindowPosition(0, 0);
	window = glutCreateWindow("LEAP Axis Test");
	glutDisplayFunc(&DrawGLScene);
	glutIdleFunc(&Idle);
	glutReshapeFunc(&ReSizeGLScene);
	glutKeyboardFunc(&NormalKeyPressed);
	glutSpecialFunc(&SpecialKeyPressed);
	InitGL(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	
	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	glutMainLoop();

	return 0;
}
