//
//  BoxSplit.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/14/16.
//
//

#include "BoxSplit.hpp"


void BoxSplit::setup() {

    reconstruct();
    
}

void BoxSplit::draw() {
    
    //ofBackground(ofColor::black);
    
    ofPushStyle();
    
    ofDisableLighting();
    ofEnableAlphaBlending();
    ofDisableDepthTest();
    
    ofSetColor(255);
    
    if(world->isDrawingLeft()) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        //matLeftLeft.begin();
        
        ofSetColor(leftLeftColor.get());
        
        if(floorOn)
        boxLeftFloor.draw();

        if(wallOn)
        boxLeftWall.draw();
        
        //matLeftLeft.end();

        
        //matRightLeft.begin();
        
        ofSetColor(rightLeftColor.get());
        if(floorOn)
            boxRightFloor.draw();
        if(wallOn)
            boxRightWall.draw();
        
         //matRightLeft.end();
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        ofSetColor(dotColor.get());
        ofDrawEllipse(boxLeftWall.getGlobalPosition().x, boxLeftWall.getHeight()*0.7, 15, 15);
        
        ofSetColor(255,255,255,127);
        
        ofDrawBox(boxLeftWall.getGlobalPosition().x, 0, 20, 2, 0.01, 10);

        
    } else {
        
        ofEnableBlendMode(OF_BLENDMODE_ADD);

       // matLeftRight.begin();
        
        
        ofSetColor(leftRightColor.get());
        if(floorOn)
        boxLeftFloor.draw();
        if(wallOn)
        boxLeftWall.draw();
        
       // matLeftRight.end();
        
        
        //matRightRight.begin();
        
        ofSetColor(rightRightColor.get());
        if(floorOn)
        boxRightFloor.draw();
        if(wallOn)
        boxRightWall.draw();
        
        //matRightRight.end();

        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        ofSetColor(dotColor.get());
        ofDrawEllipse(boxRightWall.getGlobalPosition().x, boxRightWall.getHeight()*0.7, 15, 15);

        
        ofSetColor(255,255,255,127);
        
        ofDrawBox(boxRightWall.getGlobalPosition().x, 0, 20, 2, 0.01, 10);
    }
    
    ofPopStyle();
    
}

void BoxSplit::reconstruct(){
    ofVec3f _s = getWorldSize();
    
    wallCenter.setParent(world->origin);
    floorCenter.setParent(world->origin);
    
    float boxw = 0.01;
    
    floorCenter.setPosition(0, -boxw/2, _s.z/2);
    
    boxLeftFloor.setParent(floorCenter);
    boxLeftFloor.set(boxwidth, boxw, _s.z, 4,2,4);
    
    boxRightFloor.setParent(floorCenter);
    boxRightFloor.set(boxwidth, boxw, _s.z, 4,2,4);
    
    
    wallCenter.setPosition(0, _s.y/2, -boxw/2);
    
    boxLeftWall.setParent(wallCenter);
    boxLeftWall.set(boxwidth, _s.y, boxw, 4,2,4);
    
    boxRightWall.setParent(wallCenter);
    boxRightWall.set(boxwidth, _s.y, boxw, 4,2,4);

}

void BoxSplit::update() {
    
    reconstruct();
    
    boxLeftWall.setScale(1, ofMap(downAnimation, 0, 0.5, 0, 1, true), 1);
    boxLeftFloor.setScale(1, 1, ofMap(downAnimation, 0.5, 1.0, 0, 1, true));

    boxRightWall.setScale(1, ofMap(downAnimation, 0, 0.5, 0, 1, true), 1);
    boxRightFloor.setScale(1, 1, ofMap(downAnimation, 0.5, 1.0, 0, 1, true));
 
    floorCenter.setPosition(0,  floorCenter.getPosition().y, boxLeftFloor.getDepth()*0.5*boxLeftFloor.getScale().z);

    wallCenter.setPosition(0, boxLeftWall.getHeight()*(1.0-(boxLeftWall.getScale().y*0.5)), wallCenter.getPosition().z);

    boxLeftFloor.setPosition(-split, 0, 0);
    boxRightFloor.setPosition(split, 0, 0);
    
    boxLeftWall.setPosition(-split, 0, 0);
    boxRightWall.setPosition(split, 0, 0);
    
    
   }

void BoxSplit::drawModel() {
    
    ofSetColor(255,75);
    ofSetColor(255,20);
    
    boxLeftFloor.draw();
    boxRightFloor.draw();
    
    boxLeftWall.draw();
    boxRightWall.draw();
    
}


