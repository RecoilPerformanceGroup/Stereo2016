#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.hpp"

//========================================================================
int main( ){
    ofGLWindowSettings settings;
    settings.width = 600;
    settings.height = 600;
    settings.setPosition(ofVec2f(300,0));
    auto mainWindow = ofCreateWindow(settings);
    
    settings.width = 300;
    settings.height = 300;
    settings.setPosition(ofVec2f(0,0));
    auto guiWindow = ofCreateWindow(settings);
    
    auto mainApp = make_shared<ofApp>();
    auto guiApp = make_shared<GuiApp>();
    
    ofRunApp(guiWindow, guiApp);
    ofRunApp(mainWindow, mainApp);
    
    ofRunMainLoop();

}
