#include "ofApp.h"
#include "SceneTest.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    fadeManager = make_shared<ParameterFadeManager>();
    //guiManager = make_shared<GuiManager>();
    /*
     int resolutionX = 1920;
     int resolutionY = 1080;
     */
    int resolutionX = ofGetWidth()/2;
    int resolutionY = (resolutionX * 9) / 16;
    
    ofEnableAlphaBlending();
    ofSetCircleResolution(66);
    
    ofxDatGuiFolder * projectorCalibrationFolder = gui->getFolder("Projector Calibration");

    world.addPlane(make_shared<ofxStereoscopy::Plane>(
                                                      "WALL",
                                                      800.0,
                                                      500.0,
                                                      ofVec3f(0,250,0),
                                                      ofQuaternion(0, ofVec3f(1,0,0)),
                                                      &world
                                                      ));
    
    projectorCalibrationFolder->addToggle("WALL");

    world.addPlane(make_shared<ofxStereoscopy::Plane>(
                                                      "FLOOR",
                                                      800.0,
                                                      800.0,
                                                      ofVec3f(0,0,400),
                                                      ofQuaternion(-90, ofVec3f(1,0,0)),
                                                      &world
                                                      ));

    projectorCalibrationFolder->addToggle("FLOOR");

    /*    world.addPlane(make_shared<ofxStereoscopy::Plane>(
                                                      "thing",
                                                      200.0,
                                                      120.0,
                                                      ofVec3f(0,60,700),
                                                      ofQuaternion(0, ofVec3f(1,0,0)),
                                                      &world
                                                      ));
*/
    
    scenes.push_back(make_shared<SceneTest>());
    
    stage_size_cm.addListener(this, &ofApp::stageResized);
    
}

void ofApp::setupGui(shared_ptr<ofAppBaseWindow> gW,shared_ptr<ofAppBaseWindow> mW) {
    
    guiWindow = gW;
    guiWindow = mW;
    
    worldModelCam.setFov(75);

    // Dat Gui
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setTheme(new ofxDatGuiThemeWireframe());
    
    gui->addFRM(1.0f);
    gui->addBreak();
    
    // Model View
    
    vector<string> views = {"Perspective Model View", "Camera Model View", "Free Model View"};
    ofxDatGuiDropdown * viewDropdown = gui->addDropdown("Model View", views);
    viewDropdown->select(0);
    viewDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    
    guiBindings.push_back(make_shared<SlidersVec3f>(world.physical_camera_pos_cm, gui));

    gui->addSlider(world.physical_eye_seperation_cm);
    
    gui->addBreak();

    // Projector Calibration
    
    ofxDatGuiFolder * projectorCalibrationFolder = gui->addFolder("Projector Calibration");
    
    projectorCalibrationFolder->onFolderEvent(this, &ofApp::onFolderEvent);
    
    // Stage size
    
    guiBindings.push_back(make_shared<SlidersVec3f>(stage_size_cm, gui));
    
    ofxDatGuiSlider * resolutionSlider = gui->addSlider(world.pixels_cm);
    resolutionSlider->onSliderEvent(this,&ofApp::onSliderEvent);
    
    gui->addBreak();
    
    // General

    guiBindings.push_back(make_shared<ColorPickerWithAlpha>(background_color, gui));

    // adding the optional header allows you to drag the gui around //
    gui->addHeader(":: STEREO 2016 ::");
    
    // adding the optional footer allows you to collapse/expand the gui //
    gui->addFooter();

    ofSetFrameRate(60);
    
    oscReceiver.setup(9999);
}

void ofApp::drawGui(ofEventArgs &args) {
    
    if(ofGetFrameNum() == 2){
        worldModelCam.setPosition(ofVec3f(700, 600, 1500));
        worldModelCam.lookAt(ofVec3f(-600,50,0));
        worldModelCam.setNearClip(20);
        worldModelCam.disableMouseInput();
    }
    
    ofBackgroundGradient(ofColor(20), ofColor(40));
    
    // we draw stereography world here
    
    ofEnableDepthTest();

    if(calibrate_planes){
        ofPushMatrix();
//        ofScale(1.0/guiWindow->getWidth(), 1.0/width);
        
        ofPopMatrix();
    } else {
        worldModelCam.begin();
        world.drawModel(!(gui->getDropdown("Model View")->getSelected()->getLabel() == "CAMERA MODEL VIEW"));
        worldModelCam.end();
    }
    ofDisableDepthTest();

    gui->draw();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    fadeManager->update();
    
    while(oscReceiver.hasWaitingMessages()) {
        
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        ofAbstractParameter * p = &mainParams;
        
        cout<<"Received "<<msg.getAddress()<<endl;
        
        vector<string> address = ofSplitString(ofToLower(msg.getAddress()),"/",true);
        
        // fade
        
        // get fade method that takes the number of expected arguments before the keyword
        // 1 for sliders
        
        bool fadeValue = false;
        float fadeTime = 2.0; // optional fade time
        ofxeasing::function easeFn = ofxeasing::linear::easeIn;
        // add optional fade ease function - default linear
        // optional wait param
        
        // loop through all arguments
        // if string f - next argument is time if its a float
        // if string e - next argument matches a lookup for easing functions if its an int
        // if string from - next argument is the value to start the fade from
        
        for(int i=0; i<msg.getNumArgs(); i++) {
            if(msg.getArgType(i) == OFXOSC_TYPE_STRING) {
                if(msg.getArgAsString(i) == "fade") {
                    fadeValue = true;
                    
                    if(msg.getNumArgs() > i+1) {
                        if(msg.getArgType(i+1) == OFXOSC_TYPE_FLOAT) {
                            fadeTime = msg.getArgAsFloat(i+1);
                        } else if( msg.getArgType(i+1) == OFXOSC_TYPE_INT32) {
                            fadeTime = msg.getArgAsInt(i+1);
                        }
                    }
                } else if (fadeManager->hasEaseFunction(msg.getArgAsString(i))){
                    cout<<msg.getArgAsString(i)<<endl;
                    easeFn = fadeManager->getEaseFunction(msg.getArgAsString(i));
                }
            }
        }
        
        for(unsigned int i=0;i<address.size();i++){
            if(p) {
                
                cout<<p->getEscapedName()<<endl;
                if(address[i]==ofToLower(p->getEscapedName())){
                    if(p->type()==typeid(ofParameterGroup).name()){
                        if(address.size()>=i+1){
                            p = &static_cast<ofParameterGroup*>(p)->get(address[i+1]);
                        }
                    }else if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
                        
                        if(fadeValue) {
                            fadeManager->add(new ParameterFade<int>(p, msg.getArgAsInt32(0), fadeTime, easeFn));
                        } else {
                            p->cast<int>() = msg.getArgAsInt32(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
                        
                        if(fadeValue) {
                            fadeManager->add(new ParameterFade<float>(p, msg.getArgAsFloat(0), fadeTime, easeFn));
                        } else {
                            p->cast<float>() = msg.getArgAsFloat(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                        
                        if(fadeValue) {
                            fadeManager->add(new ParameterFade<double>(p, msg.getArgAsDouble(0), fadeTime, easeFn));
                        } else {
                            p->cast<double>() = msg.getArgAsDouble(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<bool>).name() && (msg.getArgType(0)==OFXOSC_TYPE_TRUE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FALSE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT32
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT64
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FLOAT
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_DOUBLE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_STRING
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
                        
                        // Bool doesn't fade
                        p->cast<bool>() = msg.getArgAsBool(0);
                        
                    }else if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
                        
                        if(fadeValue) {
                            
                            if(p->type()==typeid(ofParameter<ofColor>).name()) {
                                
                                ofParameter<ofColor> target;
                                target.fromString(msg.getArgAsString(0));
                                fadeManager->add(new ParameterFade<ofColor>(p, target.get(), fadeTime, easeFn));
                            }
                            
                            
                            
                        } else {
                            p->fromString(msg.getArgAsString(0));
                        }
                        
                        // todo: osc on individual components of complex params
                        // eg. x, y,z r, g, b, a, ...
                    }
                }
            }
        }
    }
    
    if(gui->getDropdown("Model View")->getSelected()->getLabel() == "CAMERA MODEL VIEW"){
        worldModelCam.setPosition(world.physical_camera_pos_cm);
        worldModelCam.lookAt(ofVec3f(0,0,0));
        worldModelCam.setNearClip(20);
    }
    
    updateStage();

}


void ofApp::drawScenes(int _surfaceId) {
    
    ofClear(ofColor::black);
    
    glPushMatrix();
    
    for(auto s : scenes) {
        s->beginSceneWorld(_surfaceId);
    }
    
    for(auto s : scenes) {
        s->drawScene(_surfaceId);
    }
    
    for(auto s : scenes) {
        s->endSceneWorld(_surfaceId);
    }
    
    glPopMatrix();
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    //    ofSetColor(color01);
    //    ofDrawCircle(vec301.get().x,vec301.get().y, float01.get()*800);
    
    if(calibrate_planes){
        world.renderProjectorCalibrations();
    } else {
        for(std::pair<string, shared_ptr<ofxStereoscopy::Plane>> p : world.planes){
            p.second->beginLeft();
            ofClear(background_color);
            p.second->endLeft();
            p.second->beginRight();
            ofClear(background_color);
            p.second->endRight();
        }
    }
    
    ofPushMatrix();
    
    float fboHeight = ofGetHeight()/world.planes.size();
    
    for(std::pair<string, shared_ptr<ofxStereoscopy::Plane>> p : world.planes){
        float fboWidth = p.second->fboLeft.getWidth()/p.second->fboLeft.getHeight()*fboHeight;
        p.second->fboLeft.draw(0, 0, fboWidth, fboHeight);
        p.second->fboRight.draw(fboWidth, 0, fboWidth, fboHeight);
        ofTranslate(0, fboHeight);
    }
    
    ofPopMatrix();
    
    //    ofDisableDepthTest();
    
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
    ;
}

void ofApp::onFolderEvent(ofxDatGuiFolderEvent e){
    if(e.target->getLabel() == "PROJECTOR CALIBRATION"){
        calibrate_planes.set(e.expanded);
    }
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    if(e.target->getLabel() == "PERSPECTIVE MODEL VIEW"){
        worldModelCam.setPosition(ofVec3f(700, 600, 1500));
        worldModelCam.lookAt(ofVec3f(-600,50,0));
        worldModelCam.setNearClip(20);
        worldModelCam.disableMouseInput();
    }
    if(e.target->getLabel() == "CAMERA MODEL VIEW"){
        worldModelCam.setPosition(world.physical_camera_pos_cm);
        worldModelCam.lookAt(ofVec3f(0,0,0));
        worldModelCam.setNearClip(20);
        worldModelCam.disableMouseInput();
    }
    if(e.target->getLabel() == "FREE MODEL VIEW"){
        worldModelCam.setPosition(ofVec3f(700, 600, 1500));
        worldModelCam.lookAt(ofVec3f(-600,50,0));
        worldModelCam.setNearClip(20);
        worldModelCam.enableMouseInput();
    }
    e.target->collapse();
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e){
    if(e.target->getLabel() == "PIXELS PR. CM"){
        flagStageResized = true;
    }
}

void ofApp::stageResized(ofVec3f& v){
    flagStageResized = true;
}

void ofApp::updateStage(){
    if(flagStageResized){
    world.getPlane("WALL")->ofPlanePrimitive::setHeight(stage_size_cm->y);
    world.getPlane("WALL")->ofPlanePrimitive::setWidth(stage_size_cm->x);
    world.getPlane("WALL")->ofPlanePrimitive::setGlobalPosition(0, stage_size_cm->y/2.0, 0);
    world.getPlane("WALL")->dimensionsChanged();
        world.getPlane("FLOOR")->ofPlanePrimitive::setHeight(stage_size_cm->z);
        world.getPlane("FLOOR")->ofPlanePrimitive::setWidth(stage_size_cm->x);
        world.getPlane("FLOOR")->ofPlanePrimitive::setGlobalPosition(0, 0, stage_size_cm->z/2.0);
        world.getPlane("FLOOR")->dimensionsChanged();
    flagStageResized = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key=='s') saveParameters(mainParams);
    
    if(key=='l') loadParameters(mainParams);
    
    if(key=='c') {
        worldModelCam.setGlobalPosition(world.physical_camera_pos_cm);
        worldModelCam.lookAt(ofVec3f(0,0,0));
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


// Recursively save parameters in parameter group
void ofApp::saveParameters(ofParameterGroup & params) {
    
    
    // cout<<params.toString()<<endl;
    
    // like this maybe: https://github.com/elliotwoods/ofxRulr/blob/master/Core/src/ofxRulr/Utils/Serializable.h
    
    
    
    //string json = params.toString();
    
    //string filename = "settings.txt";
    // ofFile output;
    
    ofxPanel panel(params);
    panel.saveToFile("settings.xml");
    
    
    //output.open(filename, ofFile::WriteOnly, false);
    //output << json;
    
    // todo json format
    
    //ofxPanel p;
    //p.saveToFile(<#const std::string &filename#>)
    
}

// Recursively load parameters in parameter group
void ofApp::loadParameters(ofParameterGroup & params) {
    
    
    //string filename = "settings.txt";
    //ofFile input;
    //input.open(filename, ofFile::ReadOnly, false);
    
    //ofBuffer buf = input.readToBuffer();
    
    //params.fromString(buf); // not implemented
    
    ofxPanel panel(params);
    panel.loadFromFile("settings.xml");
    
    
}
