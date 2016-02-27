#include "ofApp.h"
#include "SceneTest.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    fadeManager = make_shared<ParameterFadeManager>();
    //guiManager = make_shared<GuiManager>();
    guiManager = new GuiManager();
/*  
    int resolutionX = 1920;
    int resolutionY = 1080;
*/
    int resolutionX = ofGetWidth()/2;
    int resolutionY = (resolutionX * 9) / 16;
    
    /*
    planeFloor.reset(new ofxStereoscopy::Plane("floor"));
    planeFloor->setup(resolutionX, resolutionY);
    planeFloor->setViewPort(ofRectangle(-1, -1, 2, 2));
    planeFloor->pos = ofVec2f(0,0);
    planes.push_back(planeFloor);

    planeWall.reset(new ofxStereoscopy::Plane("wall"));
    planeWall->setup(resolutionX, resolutionY);
    planeWall->setViewPort(ofRectangle(-1, -1, 2, 2));
    planeWall->pos = ofVec2f(resolutionX,0);
    planes.push_back(planeWall);
    
    activePlaneIndex = 0;
    
    activePlane = planes[activePlaneIndex];
    */
    scenes.push_back(make_shared<SceneTest>());
}

void ofApp::setupGui() {
    
    gui->addFRM(1.0f);
    
    gui->addSlider(float01);
    
    
    new ColorPickerWithAlpha(color01, gui);
    
    //guiBindings.push_back(make_shared<ColorPickerWithAlpha>(color01, gui));
    guiBindings.push_back(make_shared<SlidersVec3f>(vec301, gui));
    guiBindings.push_back(make_shared<SlidersVec2f>(vec201, gui));
    guiBindings.push_back(make_shared<PadAndZ>(vec301, gui));
    guiManager->addPad2d(vec201, gui);
    
    oscReceiver.setup(9999);
}

void ofApp::drawGui(ofEventArgs &args) {
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
    
/* 
    for(auto p : planes) {
        p->cam.setPhysicalEyeSeparation(6.5);
        p->update();
    }
    
    for(auto s : scenes) {
        s->updateScene();
    }
*/
    
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
    ofSetColor(color01);
    ofDrawCircle(vec301.get().x,vec301.get().y, float01.get()*800);
    
    ofSetColor(255);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
    
    // we draw stereography world here
    
    
    ofDisableDepthTest();
    
}
 
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
