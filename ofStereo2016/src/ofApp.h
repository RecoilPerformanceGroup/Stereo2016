#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOsc.h"
#include "ParameterFade.hpp"
#include "ofxStereoscopy.hpp"
#include "ofxDatGuiParameterBindings.hpp"

class ofApp : public ofBaseApp{
    
public:
    
    ofParameter<float> float01{"float01", 1, 0, 1};
    ofParameter<ofColor> color01{"color01", ofColor::white,ofColor::black,ofColor::white};
    ofParameter<ofVec2f> vec201{"position1", ofVec3f{50,50,50}, ofVec3f{0,0,0}, ofVec3f{100,100,100}};
    ofParameter<ofVec3f> vec301{"position", ofVec3f{50,50,50}, ofVec3f{0,0,0}, ofVec3f{100,100,100}};
    ofParameter<float> float02;
    
    ofParameterGroup subSubParams{
        "subSubParams",
        vec301
    };
    
    ofParameterGroup subParams{
        "subParams",
        subSubParams,
        float01,
        float02,
        color01};
    
    ofParameterGroup mainParams{
        "mainParams",
        subParams,
        vec201
    };
    
    void setup();
    void setupGui();
    
    void update();
    
    void draw();
    void drawGui(ofEventArgs & args);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    shared_ptr<ParameterFadeManager> fadeManager;
    //shared_ptr<GuiManager> guiManager;
    GuiManager * guiManager;
    
    
    vector<shared_ptr<AbstractDatGuiParameterBinding>> guiBindings;
    ofxDatGui * gui = new ofxDatGui( 100, 100 );
    
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    
    // osc
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    // program variables
    float circleRadius;
    ofColor col;
    
    // stereoscopy
    std::shared_ptr<ofxStereoscopy::Plane> planeFloor;
    std::shared_ptr<ofxStereoscopy::Plane> planeWall;
    
    vector<shared_ptr<ofxStereoscopy::Plane>> planes;
    std::shared_ptr<ofxStereoscopy::Plane> activePlane;
    int activePlaneIndex = 0;
    
    vector<shared_ptr<ofxStereoscopy::Scene>> scenes;
    void drawScenes(int _surfaceId);
};
