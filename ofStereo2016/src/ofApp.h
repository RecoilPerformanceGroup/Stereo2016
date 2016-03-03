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

#include "SceneTest.hpp"
#include "CrystalScene.hpp"

class ofApp : public ofBaseApp{
    
public:
    
    ofApp() {
        
        scenes.push_back(make_shared<SceneTest>());
        scenes.push_back(make_shared<CrystalScene>());
        
        for( auto s : scenes) {
            s->setupScene();
            mainParams.add(s->getParameters());
        }
        
        //scenes[0]->enabled = true;
        scenes[1]->enabled = true;
        
        
    }
    
    ofParameter<ofVec3f> stage_size_cm{"Stage Size", ofVec3f{800,500,800}, ofVec3f{600,300,600}, ofVec3f{1000,800,1000}};
    ofParameter<bool> calibrate_planes{"Calibrate Planes", false, false, true};
    ofParameter<ofColor> background_color{"Background Color", ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)};

    ofParameterGroup mainParams{
        "mainParams",
        stage_size_cm,
        calibrate_planes,
        background_color
    };
    
    ofxPanel panel;
    
    void setup();
    void setupGui(shared_ptr<ofAppBaseWindow> gW, shared_ptr<ofAppBaseWindow> mW);
    
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
    void drawScenes();
    
    ofCamera calibrationCamera;
    ofVec3f calibrationCameraToScreen(ofVec3f v);
    ofVec3f screenToCalibrationCamera(ofVec3f v);
    ofRectangle outputScreensRectangle;
    
    void stageResized(ofVec3f& v);
    void updateStage();
    bool flagStageResized = false;
    
    void saveParameters(ofParameterGroup & params);
    void loadParameters(ofParameterGroup & params);
    
    shared_ptr<ofAppBaseWindow> guiWindow;
    shared_ptr<ofAppBaseWindow> mainWindow;
    
    
};
