#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.hpp"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.width = 1024*2;
    settings.height = 768;//round((settings.width/2.0)*(3.0/4.0));
    settings.setPosition(ofVec2f(1440,0));
    settings.resizable = true;
    settings.setGLVersion(3, 2);
    //settings.multiMonitorFullScreen = true;
    settings.decorated = false;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    
    settings = ofGLFWWindowSettings();
    settings.width = 1000;
    settings.height = 800;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    settings.shareContextWith = mainWindow;
    settings.setGLVersion(3, 2);
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    //guiWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    
    mainApp->setupGui(guiWindow, mainWindow);
    
    ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);
    
    // gui events to main app special functions
    
#define guiEventMacro(NAME)  ofAddListener(guiWindow->events() . NAME , mainApp.get(), &ofApp::NAME ## Gui);

    guiEventMacro(keyPressed);
    guiEventMacro(keyReleased);
    guiEventMacro(mouseMoved);
    guiEventMacro(mouseDragged);
    guiEventMacro(mousePressed);
    guiEventMacro(mouseReleased);
    guiEventMacro(mouseEntered);
    guiEventMacro(mouseExited);
    guiEventMacro(windowResized);
    
    ofRunApp(mainWindow, mainApp);

    ofRunMainLoop();

}
