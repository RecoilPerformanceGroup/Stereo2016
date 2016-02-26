#include "ofApp.h"
#include "SceneTest.hpp"


/**

 Structure
 
 A parameter is an ofParameter
 
 All ofParameters can be controlled by OSC usign their name 
 
 Optionally an ofParameter may also have a gui element
 The gui element is instatiated seperately 
 
 Add shorthand methods to bind gui elements to other things than sliders
 
 Use addSlider ofParameter constructor for sliders
 
 ofParameter can fit to different gui elelemts
 
 maybe a slider is saturation of color but ofParamter might still be an ofColor ... how to do this ?
 
 PArams are saved using ofxXmlSettings ? json ?
 
 
 osc is case insensitive
 
 
 // if a fade is running can a message override? YEs I think.
 // New message cancel fade
 
 
 **/


//--------------------------------------------------------------
void ofApp::setup(){
    
    fadeManager = new ParameterFadeManager();
    
/*  
    int resolutionX = 1920;
    int resolutionY = 1080;
*/
    int resolutionX = ofGetWidth()/2;
    int resolutionY = (resolutionX * 9) / 16;
    
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
    
    scenes.push_back(make_shared<SceneTest>());

}


void ofApp::setupGui() {
    
    gui->addFRM(1.0f);
    
    gui->addSlider(float01);
    new ColorPickerFromParameter(color01, gui, true);
    
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
                            fadeManager->add(new ParameterFade<int>(p, msg.getArgAsInt32(0), fadeTime));
                        } else {
                            p->cast<int>() = msg.getArgAsInt32(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
                        
                        if(fadeValue) {
                            fadeManager->add(new ParameterFade<float>(p, msg.getArgAsFloat(0), fadeTime));
                        } else {
                            p->cast<float>() = msg.getArgAsFloat(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                        
                        if(fadeValue) {
                            fadeManager->add(new ParameterFade<double>(p, msg.getArgAsDouble(0), fadeTime));
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
                                fadeManager->add(new ParameterFade<ofColor>(p, target.get(), fadeTime));
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
    
    for(auto p : planes) {
//      p->cam.setPhysicalEyeSeparation(6.5);
        p->update();
    }
    
    for(auto s : scenes) {
        s->updateScene();
    }

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
    ofDrawCircle(100,100, float01.get()*800);
    
    ofSetColor(255);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
    // draw scenes to surfaces, they are kept in the cameras fbo
    
    for(int i=0;i < planes.size(); i++) {
        planes[i]->beginLeft();
        drawScenes(i);
        planes[i]->endLeft();
        
        planes[i]->beginRight();
        drawScenes(i);
        planes[i]->endRight();
    }
    
    ofDisableDepthTest();
    
    /* 
     if(drawChessboards) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawChessboards();
        }
    }
    */
    
//    if(drawGrids) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawGrids();
        }
//    }
    
        ofSetColor(255);
        ofFill();
        for(int i=0; i<planes.size(); i++) {
            planes[i]->draw();
        }
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
