#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.hpp"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings mainSettings;
    
    mainSettings.resizable = false;
    mainSettings.setGLVersion(4, 1);
    mainSettings.decorated = false;

    ofGLFWWindowSettings guiSettings;
    
    guiSettings.resizable = true;
    guiSettings.setGLVersion(4, 1);

    // Get screen widths and heights from Quartz Services
    // See https://developer.apple.com/library/mac/documentation/GraphicsImaging/Reference/Quartz_Services_Ref/index.html
    
    CGDisplayCount displayCount;
    CGDirectDisplayID displays[32];
    
    // Grab the active displays
    CGGetActiveDisplayList(32, displays, &displayCount);
    int numDisplays= displayCount;
    
    // print display info.
    cout<<numDisplays<<" display(s) detected."<<endl;
    
    for (int i = 0; i < displayCount; i++){
        cout << displays[i] << "\t(" << CGDisplayPixelsWide(displays[i]) << "x" << CGDisplayPixelsHigh(displays[i]) << ")"<< endl;
    }
    
    CGRect mainDisplayBounds= CGDisplayBounds ( displays[0] );

    if(numDisplays == 1 ){
        
        float divisions = 4.0;
        // one display: palce gui and mainWindow in a default arrangement
        
        mainSettings.width = round(mainDisplayBounds.size.width / divisions);
        mainSettings.height = round((mainSettings.width * (9.0 / 16.0)) / 2.0);
        mainSettings.setPosition(ofVec2f(round(mainDisplayBounds.size.width * (divisions-1) / divisions) ,mainDisplayBounds.origin.y));
        //rendering errors: mainSettings.decorated = true;
        
        guiSettings.width = round(mainDisplayBounds.size.width * (divisions-1) / divisions);
        guiSettings.height = mainDisplayBounds.size.height - 100;
        guiSettings.setPosition(ofVec2f(mainDisplayBounds.origin.x, mainDisplayBounds.origin.y));

        cout << "configured default one display setup" << endl;
        
    } else if (numDisplays > 1){
        
        // two displays: palce resizeable gui on first and fill second with undecorated mainWindow - will also work with dual head configuraions for projectors

        guiSettings.width = mainDisplayBounds.size.width;
        guiSettings.height = mainDisplayBounds.size.height - 100;
        guiSettings.setPosition(ofVec2f(mainDisplayBounds.origin.x, mainDisplayBounds.origin.y));

        CGRect secondDisplayBounds= CGDisplayBounds ( displays[1] );
        
        mainSettings.width = secondDisplayBounds.size.width;
        mainSettings.height = secondDisplayBounds.size.height;
        mainSettings.setPosition(ofVec2f(secondDisplayBounds.origin.x,secondDisplayBounds.origin.y));
        
//        CGError e = CGDisplaySetStereoOperation(displays[1], true, true, kCGConfigureForAppOnly);
        
//        if(e == kCGErrorRangeCheck) cout << "no hardware stereo" << endl;
//        if(e == kCGErrorSuccess) cout << "hardware stereo" << endl;
        
        if (numDisplays > 2 ) {
            
            // three or more displays: palce resizeable gui on first and fill second and third with undecorated mainWindow, if they are same size.
            
            CGRect thirdDisplayBounds= CGDisplayBounds ( displays[2] );
            if(secondDisplayBounds.size.width == thirdDisplayBounds.size.width && secondDisplayBounds.size.height == thirdDisplayBounds.size.height){
               
                mainSettings.width = secondDisplayBounds.size.width * 2;
                mainSettings.setPosition(ofVec2f(1,1));
                cout << "configured default three display setup" << endl;

            }
        } else {
            cout << "configured default two display setup" << endl;
        }
    }
    
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(mainSettings);
    guiSettings.shareContextWith = mainWindow;

    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(guiSettings);

    //guiWindow->setVerticalSync(false);
    
    shared_ptr<ofApp> mainApp(new ofApp);
    
    mainApp->setupGui(guiWindow, mainWindow);
    
    ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);
    
    // gui events to main app gui listener functions
    
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
