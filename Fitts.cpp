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
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <fstream>
#include "Leap.h"

using namespace Leap;
using namespace std;

// Parameters to test Fitt's Law
#define TEST_FOR Leap
//#define TEST_FOR Mouse
const static int test_count = 100;
int target_width[test_count];			// [pixel]
int target_distance[test_count];	// [pixel]
long target_time[test_count];			// [ms]

int test_c = 0;
long start_time = 0;
char new_target = 1;
int target_x, target_y;
char blank[1000];
vector<Pointable> start_position;
char status = 0;

/* ASCII code for the escape key. */
#define KEY_ESCAPE 27

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

int window;									// The number of our GLUT window
bool fullScreen=true;						// toggle for fullscreen mode
int screen_width = 640;
int screen_height = 480;
const static long font=(long)GLUT_BITMAP_HELVETICA_10;		// For printing bitmap fonts
char s[30];

Controller controller;
Screen screen;

vector<Pointable> vpoint;

void CleanupExit();

void glutDrawCirclei(int x, int y, int rad){
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<32;i++){
		float rate = (float)i / 32;
		float rx = rad / 2.0f * cos(2.0f*M_PI*rate) + x;
		float ry = rad / 2.0f * sin(2.0f*M_PI*rate) + y;
		glVertex2f(rx, ry);
	}
	glEnd();
}

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
	const PointableList plist = frame.pointables();
	if (!plist.empty()) {
		// clear last datas
		vpoint.clear();

		for(int i=0;i<plist.count();i++){
			vpoint.push_back(plist[i]);

			Vector t = plist[i].tipVelocity();
			float speed = t.x*t.x + t.y*t.y + t.z*t.z;
			if(speed < 10.0f){
				Vector v = screen.intersect(plist[i], true);
				float dist = sqrt(pow(target_x-v.x*screen_width, 2) + pow(target_y-v.y*screen_height, 2));
				if(dist <= (float)target_width[test_c]){
					for(unsigned int j=0;j<start_position.size();j++){
						if(start_position[j].id() == plist[i].id()){
							Vector sv = screen.intersect(start_position[j], true);
							target_time[test_c] = glutGet(GLUT_ELAPSED_TIME) - start_time;
							target_distance[test_c] = (int)sqrt(pow((v.x - sv.x)*screen_width, 2) + pow((v.y - sv.y)*screen_height, 2));
							test_c++;
							break;
						}
					}
					new_target = 1;
				}
			}
		}
	}
}

// Create a sample listener and controller
SampleListener listener;

void CleanupExit()
{
	ofstream ofs("result.csv");
	ofs << "Distance to target [pixels], Target Width [pixels], Elapsed time [ms]" << endl;

	for(int i=0;i<test_count;i++){
		ofs << target_distance[i] << "," << target_width[i] << "," << target_time[i] << endl;
	}

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

	glViewport( 0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screen_width, 0.0, screen_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);

	if(!vpoint.empty()){
		glPointSize(10);
		Vector v;
		glBegin(GL_POINTS);
		for(unsigned int i=0;i<vpoint.size();i++){
			v = screen.intersect(vpoint[i], true);
			glVertex2i((int)(screen_width*v.x), (int)(screen_height*v.y));
		}
		glEnd();

		sprintf(s,"Cursor Position: %d, %d", (int)(screen_width*v.x), (int)(screen_height*v.y));
		glPrint( 10, screen_height - 40, (void *)font, s);	// Print timeElapsed

		Vector t = vpoint[0].tipVelocity();
		float speed = t.x*t.x + t.y*t.y + t.z*t.z;
		sprintf(s,"Cursor Speed: %.2f", speed);
		glPrint( 10, screen_height - 55, (void *)font, s);	// Print timeElapsed
	}

	glColor3ub(255, 255, 255);								// Draw In White
	sprintf(s,"Test count: %d / %d", test_c, test_count);
	glPrint( 10, screen_height - 10, (void *)font, s);	// Print timeElapsed
	sprintf(s,"Target Position: %d, %d", target_x, target_y);
	glPrint( 10, screen_height - 25, (void *)font, s);	// Print timeElapsed

	if(test_c < test_count){
		if(new_target == 1){
			target_x = (int)((float)rand()/(float)RAND_MAX * screen_width);
			target_y = (int)((float)rand()/(float)RAND_MAX * screen_height);
			target_width[test_c] = 10+(int)((float)rand()/(float)RAND_MAX*50);
			start_time = glutGet(GLUT_ELAPSED_TIME);
			start_position = vpoint;
			new_target = 0;
		}

		glutDrawCirclei( target_x, target_y, target_width[test_c]);
	}else{
		CleanupExit();
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

	srand((unsigned int)time(NULL));

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
