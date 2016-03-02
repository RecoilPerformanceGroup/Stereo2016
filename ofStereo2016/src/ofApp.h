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
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    
    ofParameter<ofVec3f> stage_size_cm{"Stage Size", ofVec3f{800,500,800}, ofVec3f{600,300,600}, ofVec3f{1000,800,1000}};
    ofParameter<bool> calibrate_planes{"Calibrate Planes", false, false, true};
    ofParameter<ofColor> background_color{"Background Color", ofColor::white,ofColor::black,ofColor::white};

    ofParameterGroup mainParams{
        "mainParams",
        stage_size_cm,
        calibrate_planes,
        background_color
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
    
    vector<shared_ptr<AbstractDatGuiParameterBinding>> guiBindings;
    ofxDatGui* gui;
    
    void onFolderEvent(ofxDatGuiFolderEvent e);
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
    ofxStereoscopy::World world;
    ofEasyCam worldModelCam;
    
    vector<shared_ptr<ofxStereoscopy::Scene>> scenes;
    void drawScenes(int _surfaceId);
    
    void stageResized(ofVec3f& v);
    void updateStage();
    bool flagStageResized = false;
    
    void saveParameters(ofParameterGroup & params);
    void loadParameters(ofParameterGroup & params);
    
};
