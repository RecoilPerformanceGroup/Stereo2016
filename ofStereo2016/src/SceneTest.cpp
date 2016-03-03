//
//  sceneTest.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 25/02/2016.
//
//

#include "SceneTest.hpp"


void SceneTest::setup() {
    name = "Test Scene";
    oscAddress = "/SceneTest";
    light.setPointLight();
    light.setPosition(-2, -2, -2);
}

void SceneTest::draw() {
    
    
    // A scene can draw to multiple surfaces
    light.enable();
    
    ofSetColor(255,255,255);
    ofPushMatrix();
    
    //ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofRotateX(ofGetElapsedTimef()*10);
    ofDrawSphere(200);
    
    
    ofPopMatrix();
    light.disable();
    
}

void SceneTest::update() {
}

/*
void SceneTest::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
}

void SceneTest::receiveOsc(ofxOscMessage * m, string rest) {
}

void SceneTest::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
    int kind = e.getKind();
    //cout << "got event from: " << name << endl;
    
}

*/