#include <CoreGraphics/CGEvent.h>
#include <CoreGraphics/CGEventTypes.h>
#include <CoreGraphics/CGDirectDisplay.h>
#include <vector>
#include <iostream>
#include "Leap.h"
using namespace std;
using namespace Leap;

Controller controller;
Screen screen;

unsigned int screen_width;
unsigned int screen_height;
vector<Vector> vpoint;
vector<Vector> vel;

void PostMouseMoveEvent(CGMouseButton button, const CGPoint point){
	CGEventRef eve = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, point, button);
	CGEventSetType(eve, kCGEventMouseMoved);
	CGEventPost(kCGHIDEventTap, eve);
	CFRelease(eve);
}

void PostMouseLeftClickEvent(CGMouseButton button, const CGPoint point){
	CGEventRef eve = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, point, button);
	CGEventSetType(eve, kCGEventLeftMouseDown);
	CGEventPost(kCGHIDEventTap, eve);
	CFRelease(eve);
	
	eve = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, point, button);
	CGEventSetType(eve, kCGEventLeftMouseUp);
	CGEventPost(kCGHIDEventTap, eve);
	CFRelease(eve);
}

void PostMouseRightClickEvent(CGMouseButton button, const CGPoint point){
	CGEventRef eve = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, point, button);
	CGEventSetType(eve, kCGEventRightMouseDown);
	CGEventPost(kCGHIDEventTap, eve);
	CFRelease(eve);
	
	eve = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, point, button);
	CGEventSetType(eve, kCGEventRightMouseUp);
	CGEventPost(kCGHIDEventTap, eve);
	CFRelease(eve);
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
/*  std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", fingers: " << frame.fingers().count()
            << ", tools: " << frame.tools().count() << std::endl;
*/
	if (!frame.hands().empty()) {
		// clear last datas
		vpoint.clear();
		vel.clear();

		const PointableList plist = frame.pointables();
		for(int i=0;i<plist.count();i++){
			Vector v = screen.intersect(plist[i], true);
			vpoint.push_back(v);

			Vector vp = plist[i].tipVelocity();
			vel.push_back(vp);
		}
		
		if(!vpoint.empty()){
			Vector sum( 0.0f, 0.0f, 0.0f);
			Vector vsum( 0.0f, 0.0f, 0.0f);
			for(int i=0;i<vpoint.size();i++){
				sum += vpoint[i];
				vsum += vel[i];
			}
			
			CGPoint point;
			point.x = (CGFloat)screen_width * sum[0] / (float)vpoint.size();
			point.y = (CGFloat)screen_height - (CGFloat)screen_height * sum[1] / (float)vpoint.size();
			CGMouseButton button = kCGMouseButtonLeft;
			CGEventType type = kCGEventMouseMoved;
		
			if(vsum[2]/(float)vpoint.size() < -50){
				if(vpoint.size() == 1){
					PostMouseLeftClickEvent(button, point);
				}else if(vpoint.size() == 2){
					PostMouseRightClickEvent(button, point);
				}
			}else{
				PostMouseMoveEvent(button, point);
			}
		}
	}
}

// Create a sample listener and controller
SampleListener listener;

int main(int argc, const char *argv[]){
	screen_width = (unsigned int)CGDisplayPixelsWide(kCGDirectMainDisplay);
	screen_height = (unsigned int)CGDisplayPixelsHigh(kCGDirectMainDisplay);
	CGPoint current_point = CGEventGetLocation(CGEventCreate(nil));

	screen = controller.calibratedScreens()[0];
	if(!screen.isValid()){
		cout << "This screen has not been calibrated." << endl;
		return 0;
	}
	
	// Create a sample listener and controller
	SampleListener listener;
	
	// Have the sample listener receive events from the controller
	controller.addListener(listener);
	
	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();
	
	// Remove the sample listener when done
	controller.removeListener(listener);
	
	return 0;
}
