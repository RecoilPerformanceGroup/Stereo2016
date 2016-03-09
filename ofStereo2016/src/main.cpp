#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.hpp"

//========================================================================
int main( ){
    
    // Get screen widths and heights from Quartz Services
    // See https://developer.apple.com/library/mac/documentation/GraphicsImaging/Reference/Quartz_Services_Ref/index.html
    
    CGDisplayCount displayCount;
    CGDirectDisplayID displays[32];
    
    // Grab the active displays
    CGGetActiveDisplayList(32, displays, &displayCount);
    int numDisplays= displayCount;
    
    // If two displays present, use the 2nd one. If one, use the first.
    int whichDisplay= numDisplays-1;
    
    int displayHeight= CGDisplayPixelsHigh ( displays[whichDisplay] );
    int displayWidth= CGDisplayPixelsWide ( displays[whichDisplay] );
    
    CGRect displayBounds= CGDisplayBounds ( displays[whichDisplay] );
    
    // print display info.
    cout<<numDisplays<<" display(s) detected."<<endl<<"Using display "<<whichDisplay<<" ("<<displayWidth<<"x"<<displayHeight<<")."<<endl;
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:

    
    ofGLFWWindowSettings settings;
    settings.width = displayWidth;
    settings.height = displayHeight;
    settings.setPosition(ofVec2f(displayBounds.origin.x,displayBounds.origin.y));
    settings.resizable = true;
    settings.setGLVersion(3, 2);
    //settings.multiMonitorFullScreen = true;
    settings.decorated = false;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    displayHeight= CGDisplayPixelsHigh ( displays[0] );
    displayWidth= CGDisplayPixelsWide ( displays[0] );
    
    displayBounds= CGDisplayBounds ( displays[0] );

    
    settings = ofGLFWWindowSettings();
    settings.width = displayWidth;
    settings.height = displayHeight-100;
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
