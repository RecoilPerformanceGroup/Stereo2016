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
#include "qLabController.hpp"

// scenes

#include "LightScene.hpp"
#include "MountainScene.hpp"



class ofApp : public ofBaseApp{
    
public:
    
    ofApp() {
        
        sceneParams.setName("scenes");
        scenes.push_back(make_shared<MountainScene>());
        scenes.push_back(make_shared<LightScene>());
        
        for( auto s : scenes) {
            sceneParams.add(s->getParameters());
        }
        
        scenes[0]->enabled = true;
        scenes[1]->enabled = true;
        
    }
    
    ofParameter<ofVec3f> stage_size_cm{"stageSize", ofVec3f{800,500,800}, ofVec3f{600,300,600}, ofVec3f{1000,800,1000}};
    ofParameter<bool> calibrate_projector{"Projector Calibration", false, false, true};
    ofParameter<bool> calibrate_camera{"Camera Calibration", false, false, true};
    ofParameter<ofColor> background_color{"Background Color", ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)};
    ofParameter<bool> show_model_on_second_screen{"Second Screen Model", false, false, true};
    
    ofParameter<bool> swap_left_right{"swapLeftRight", true};
    
    
    
    ofParameterGroup sceneParams;
    
    ofParameterGroup globalParams{
        "stereo",
        stage_size_cm,
        calibrate_projector,
        calibrate_camera,
        background_color,
        show_model_on_second_screen,
        sceneParams
    };
    
    vector<ofxPanel *> scenePanels;
    
    void setup();
    
    void update();
    
    void draw();
    
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
    
    void setupGui(shared_ptr<ofAppBaseWindow> gW, shared_ptr<ofAppBaseWindow> mW);
    void drawGui(ofEventArgs & args);

    shared_ptr<ParameterFadeManager> fadeManager;
    
    vector<shared_ptr<AbstractDatGuiParameterBinding>> guiBindings;
    ofxDatGui* gui;
    
    virtual void windowResizedGui(ofResizeEventArgs & resize){
        windowResizedGui(resize.width,resize.height);
    }
    
    virtual void keyPressedGui( ofKeyEventArgs & key ){
        keyPressedGui(key.key);
    }
    virtual void keyReleasedGui( ofKeyEventArgs & key ){
        keyReleasedGui(key.key);
    }
    
    virtual void mouseMovedGui( ofMouseEventArgs & mouse ){
        mouseX=mouse.x;
        mouseY=mouse.y;
        mouseMovedGui(mouse.x,mouse.y);
    }
    virtual void mouseDraggedGui( ofMouseEventArgs & mouse ){
        mouseX=mouse.x;
        mouseY=mouse.y;
        mouseDraggedGui(mouse.x,mouse.y,mouse.button);
    }
    virtual void mousePressedGui( ofMouseEventArgs & mouse ){
        mouseX=mouse.x;
        mouseY=mouse.y;
        mousePressedGui(mouse.x,mouse.y,mouse.button);
    }
    virtual void mouseReleasedGui(ofMouseEventArgs & mouse){
        mouseX=mouse.x;
        mouseY=mouse.y;
        mouseReleasedGui(mouse.x,mouse.y,mouse.button);
    }
    virtual void mouseScrolledGui( ofMouseEventArgs & mouse ){
        mouseScrolledGui(mouse.x, mouse.y, mouse.scrollX, mouse.scrollY);
    }
    virtual void mouseEnteredGui( ofMouseEventArgs & mouse ){
        mouseEnteredGui(mouse.x,mouse.y);
    }
    virtual void mouseExitedGui( ofMouseEventArgs & mouse ){
        mouseExitedGui(mouse.x,mouse.y);
    }
    virtual void draggedGui(ofDragInfo & drag){
        dragEventGui(drag);
    }
    virtual void messageReceivedGui(ofMessage & message){
        gotMessageGui(message);
    }
    
    void keyPressedGui(int key);
    void keyReleasedGui(int key);
    void mouseMovedGui(int x, int y );
    void mouseDraggedGui(int x, int y, int button);
    void mousePressedGui(int x, int y, int button);
    void mouseReleasedGui(int x, int y, int button);
    void mouseEnteredGui(int x, int y);
    void mouseExitedGui(int x, int y);
    void mouseScrolledGui(int x, int y, int scrollX, int scrollY);

    void windowResizedGui(int w, int h);
    void dragEventGui(ofDragInfo dragInfo);
    void gotMessageGui(ofMessage msg);
    
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
    qLabController qlab;
    
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
    ofVec3f screenToCalibrator(ofVec2f v);
    ofRectangle outputScreensRectangle;
    
    void stageResized(ofVec3f& v);
    void updateStage();
    bool flagStageResized = false;
    
    void receiveOscParameter(ofxOscMessage & msg, ofAbstractParameter * p);
    
    void saveParameters(ofParameterGroup & params);
    void loadParameters(ofParameterGroup & params);
    void loadAllParameters();
    void saveAllParameters();
    
    shared_ptr<ofAppBaseWindow> guiWindow;
    shared_ptr<ofAppBaseWindow> mainWindow;
    
    ofAbstractParameter * lastChangedParam = nullptr;
    
    vector<vector<string>> indirectParams;
    
    void paramsChanged(ofAbstractParameter & p) {

        vector<string> groupHierachy = p.getGroupHierarchyNames();

        // handle qlab save
        if(p.type()==typeid(ofParameter<bool>).name()){
            ofParameter<bool> & pBool = static_cast<ofParameter<bool>&>(p);
            if(pBool.getName() == "add to qlab" && pBool.get()){
                pBool.set(false);
                
                ofParameterGroup g = pBool.getFirstParent();
                
                cout << "should save " << g.getName() << endl;
                
                qlab.newGroupWithOscCuesFromParameterGroup(g);
                
                return;
            }
        }
        
        // skip indirect params
        for(auto blacklistName : indirectParams ) {
            bool firstElementFound = false;
            for( auto pName : groupHierachy ) {
                if(pName == blacklistName.front()){
                    firstElementFound = true;
                }
                if(firstElementFound){
                    if(pName == blacklistName.back()){
                        // this path is blacklisted as an indirect parameter
                        // we simply return
                        return;
                    }
                }
            }
        }
        lastChangedParam = &p;
    }
    
};
