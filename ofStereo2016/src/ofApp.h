#pragma once

//#define WEBPARAMS 1

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
#ifdef WEBPARAMS
#include "ofxSyncedParams.h"
#include "ofxLibwebsockets.h"
#endif /* WEBPARAMS */


// scenes

#include "LightScene.hpp"
#include "MountainScene.hpp"
#include "PlateauScene.hpp"
#include "CrystalScene.hpp"
#include "RoomScene.hpp"
#include "SketchScene.hpp"
#include "VoroScenes.hpp"
#include "OpeningScene.hpp"
#include "VoroEnd.hpp"
#include "TextScene.hpp"
#include "GameScene.hpp"
#include "PushDown.hpp"
#include "BoxSplit.hpp"
#include "PerspectiveChange.hpp"
#include "SoundField.hpp"

class ofApp : public ofBaseApp{
    
public:
    
    ofApp() {
        
        sceneParams.setName("scenes");
        //scenes.push_back(make_shared<VoroScenes>());
        scenes.push_back(make_shared<RoomScene>());
        scenes.push_back(make_shared<CrystalScene>());
        scenes.push_back(make_shared<LightScene>());
        // scenes.push_back(make_shared<PlateauScene>());
        scenes.push_back(make_shared<OpeningScene>());
        scenes.push_back(make_shared<SketchScene>());
        scenes.push_back(make_shared<TextScene>());
        scenes.push_back(make_shared<VoroEnd>());
        scenes.push_back(make_shared<GameScene>());
        scenes.push_back(make_shared<PushDown>());
        scenes.push_back(make_shared<PerspectiveChange>());
        scenes.push_back(make_shared<SoundField>());
        
        scenes.push_back(make_shared<BoxSplit>());
        
        for( auto s : scenes) {
            sceneParams.add(s->getParameters());
        }
        
        globalParams.add(sceneParams);
    }
    
    
    ofParameter<ofVec3f> stage_size_cm{"stageSize", ofVec3f{800,500,800}, ofVec3f{600,300,600}, ofVec3f{1000,800,1000}};
    ofParameter<bool> calibrate_projector{"Projector Calibration", false, false, true};
    ofParameter<bool> calibrate_camera{"Camera Calibration", false, false, true};
    ofParameter<ofFloatColor> background_color{"Background Color", ofFloatColor(1,1,1,1),ofFloatColor(0,0,0,0),ofFloatColor(1,1,1,1)};
    ofParameter<bool> show_model_on_second_screen{"Second Screen Model", false, false, true};
    
    ofParameter<bool> swap_left_right{"swapLeftRight", false};
    ofParameter<bool> save_hires{"save_hires", false};
    ofParameter<bool> drawSceneGuis{"drawSceneGuis", true};
    
    ofParameter<ofVec3f> input3D {"input3d", ofVec3f{0,0,0}, ofVec3f{-1,-1,-1}, ofVec3f{1,1,1} };
    
    
    ofParameterGroup sceneParams;
    
    ofParameterGroup globalParams{
        "stereo",
        stage_size_cm,
        calibrate_projector,
        calibrate_camera,
        background_color,
        show_model_on_second_screen,
        input3D,
        drawSceneGuis
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
    
#ifdef WEBPARAMS
    void parameterChanged( std::string & );
    void launchBrowser();
    ofxSyncedParams paramSync;
    ofxLibwebsockets::Server server;
    
    // websocket methods
    void onMessage( ofxLibwebsockets::Event& args );
    void onConnect( ofxLibwebsockets::Event& args ){}
    void onOpen( ofxLibwebsockets::Event& args ){}
    void onClose( ofxLibwebsockets::Event& args ){}
    void onIdle( ofxLibwebsockets::Event& args ){}
    void onBroadcast( ofxLibwebsockets::Event& args ){}
#endif /* WEBPARAMS */
    
    ofImage imageSaver;
    float imageSaverRealPixelsCm;
    
    shared_ptr<ofAppBaseWindow> guiWindow;
    shared_ptr<ofAppBaseWindow> mainWindow;
    
    ofAbstractParameter * lastChangedParam = nullptr;
    
    vector<vector<string>> indirectParams;

    void paramsChanged(ofAbstractParameter & p) {

        vector<string> groupHierachy = p.getGroupHierarchyNames();

        if(p.type()==typeid(ofParameter<bool>).name()){
            ofParameter<bool> & pBool = static_cast<ofParameter<bool>&>(p);
            
            // handle qlab save
            if(pBool.getName() == "add to qlab" && pBool.get()){
                pBool.set(false);
                
                ofParameterGroup g = pBool.getFirstParent();
                
                qlab.newGroupWithOscCuesFromParameterGroup(g);
                
                return;
            }

            // handle panel placement
            if(pBool.getName() == "enabled"){
                
                ofParameterGroup g = pBool.getFirstParent();
                
                int xDisabled = 0;
                int xEnabled = 0;
                
                int wWidth = guiWindow->getWidth();
                
                for(auto p : scenePanels){
                    
                    if(p->getToggle("enabled")){
                        xEnabled+=p->getWidth();
                        p->setPosition(wWidth-xEnabled,0);
                        p->maximize();
                        p->sizeChangedCB();
                    } else {
                        p->setPosition(xDisabled, guiWindow->getHeight()-65);
                        xDisabled+=p->getWidth()*0.5;
                        p->minimizeAll();
                        p->sizeChangedCB();
                    }
                }
                
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
