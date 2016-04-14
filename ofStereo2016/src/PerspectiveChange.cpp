//
//  PerspectiveChange.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/14/16.
//
//

#include "PerspectiveChange.hpp"


void PerspectiveChange::setup() {
    
    
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();
    
    wallCenter.setParent(world->origin);
    floorCenter.setParent(world->origin);
    
    float boxw = 0.01;
    
    wallCenter.setPosition(0, _s.y/2, -boxw/2);
    
    cyl1.setParent(floorCenter);
    cyl1.set(30, _s.y/2, 8, 2);
    
    cyl2 = cyl1;
    cyl3 = cyl1;
    
    
    cyl2.setPosition(150, -20, 100);
    cyl3.setPosition(-150, 40, -80);
    
}

void PerspectiveChange::draw() {
    
    //ofBackground(ofColor::black);
    
    
    ofDisableLighting();
    ofEnableAlphaBlending();
    //ofDisableDepthTest();
    
    ofSetColor(255);
    
    /*
    cyl1.draw();
    cyl2.draw();
    cyl3.draw();
    */
}

void PerspectiveChange::update() {
    
    
    floorCenter.setPosition(cylpos);
    
    
    if(ofGetFrameNum() % frameInterval == 0) {
        //ofVec3f camPos = ofVec3f(ofRandom(world->audienceBackRight.get), ofRandom(world->audienceFrontLeft), ofRandom(audienc));
        
        
        ofVec3f min = world->audienceFrontLeft;
        ofVec3f max = world->audienceBackRight;
        
        ofVec3f camPos = min.getInterpolated(max, ofRandom(1.0)) * ofVec3f (0,1,1);
        
//        camPos.x = ofMap(((ofGetFrameNum()/frameInterval) % frameInterval)*1.0/frameInterval, 0.0, 1.0, min.x, max.x);
  
        
        camPos.x = ofRandom( min.x, max.x);

        //camPos.map(world->audiencePlane.getGlobalPosition(), world->audiencePlane.getXAxis(), world->audiencePlane.getYAxis(), world->audiencePlane.getZAxis());
        //world->audiencePlane.
        
        world->physical_camera_pos_cm.set(camPos);
    }
    

    
}

void PerspectiveChange::drawModel() {
    
    ofSetColor(255,75);
    //box.drawWireframe();
    ofSetColor(255,20);
    
    //box1.draw();
    //box2.draw();
/*
    cyl1.draw();
    cyl2.draw();
    cyl3.draw();
*/
}


