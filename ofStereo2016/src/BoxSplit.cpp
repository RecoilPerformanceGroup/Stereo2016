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
        ofDrawEllipse(boxLeftWall.getGlobalPosition().x, boxLeftWall.getHeight()*0.5, 15, 15);
        
        
        
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
        ofDrawEllipse(boxRightWall.getGlobalPosition().x, boxRightWall.getHeight()*0.5, 15, 15);

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
    boxLeftFloor.set(_s.x*0.5, boxw, _s.z, 4,2,4);
    
    boxRightFloor.setParent(floorCenter);
    boxRightFloor.set(_s.x*0.5, boxw, _s.z, 4,2,4);
    
    
    wallCenter.setPosition(0, _s.y/2, -boxw/2);
    
    boxLeftWall.setParent(wallCenter);
    boxLeftWall.set(_s.x*0.5, _s.y, boxw, 4,2,4);
    
    boxRightWall.setParent(wallCenter);
    boxRightWall.set(_s.x*0.5, _s.y, boxw, 4,2,4);

}

void BoxSplit::update() {
    
    reconstruct();
    
    float collectiveLength = boxRightWall.getHeight() + boxRightFloor.getDepth();
    
    float currentLength = collectiveLength*downAnimation;
    
    boxLeftWall.setScale(boxwidth, ofMap(currentLength, 0, boxLeftWall.getHeight(), 0, 1, true), 1);
    boxLeftFloor.setScale(boxwidth, 1, ofMap(currentLength, boxRightWall.getHeight(), collectiveLength, 0, 1, true));

    boxRightWall.setScale(boxwidth, ofMap(currentLength, 0, boxLeftWall.getHeight(), 0, 1, true), 1);
    boxRightFloor.setScale(boxwidth, 1, ofMap(currentLength, boxRightWall.getHeight(), collectiveLength, 0, 1, true));
 
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


