//
//  BoxSplit.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/14/16.
//
//

#include "BoxSplit.hpp"


void BoxSplit::setup() {

    
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();

    wallCenter.setParent(world->origin);
    floorCenter.setParent(world->origin);
    
    float boxw = 0.01;
    
    wallCenter.setPosition(0, _s.y/2, -boxw/2);
    floorCenter.setPosition(0, -boxw/2, _s.z/4);
    
    
    boxLeftFloor.setParent(floorCenter);
    boxLeftFloor.set(_s.x*0.5, boxw, _s.z/2.0, 4,2,4);
 
    
    boxRightFloor.setParent(floorCenter);
    boxRightFloor.set(_s.x*0.5, boxw, _s.z/2.0, 4,2,4);
    
    
    boxLeftWall.setParent(wallCenter);
    boxLeftWall.set(_s.x*0.5, _s.y, boxw, 4,2,4);
    
    
    boxRightWall.setParent(wallCenter);
    boxRightWall.set(_s.x*0.5, _s.y, boxw, 4,2,4);
    
}

void BoxSplit::draw() {
    
    ofBackground(ofColor::black);
    
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

        ofSetColor(0,255);
        ofDrawEllipse(boxLeftWall.getGlobalPosition().x, boxLeftWall.getGlobalPosition().y, 10, 10);
        
        
        
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

        ofSetColor(0,255);
        ofDrawEllipse(boxRightWall.getGlobalPosition().x, boxRightWall.getGlobalPosition().y, 10, 10);

    }
    
    ofPopStyle();
    
}

void BoxSplit::update() {
    
    
    boxLeftFloor.setScale(boxwidth, 1, 1);
    boxRightFloor.setScale(boxwidth, 1, 1);
    
    boxLeftWall.setScale(boxwidth, 1, 1);
    boxRightWall.setScale(boxwidth, 1, 1);
    
    boxLeftFloor.setPosition(-split, 0, 0);
    boxRightFloor.setPosition(split, 0, 0);
   
    boxLeftWall.setPosition(-split, 0, 0);
    boxRightWall.setPosition(split, 0, 0);
    
    
    /*matRightRight.setDiffuseColor(rightRightColor);
    matRightRight.setAmbientColor(rightRightColor);
    matRightRight.updateParameters();
    
    matRightLeft.setDiffuseColor(rightLeftColor);
    matRightLeft.setAmbientColor(rightLeftColor);
    matRightLeft.updateParameters();
    
    matLeftRight.setDiffuseColor(leftRightColor);
    matLeftRight.setAmbientColor(leftRightColor);
    matLeftRight.updateParameters();
    
    matLeftLeft.setDiffuseColor(leftLeftColor);
    matLeftLeft.setAmbientColor(leftLeftColor);
    matLeftLeft.updateParameters();
    */
    
    
    
    // todo: fade color to same as split is 0
    
    /*ofColor mix = leftColor.get().getLerped(rightColor, 0.5);
    
    ofColor left = leftColor.get().getLerped(mix, (split.getMax()-split) / split.getMax());
    
    
    
     
    matLeft.setDiffuseColor(left);
    matLeft.setAmbientColor(left);
    matLeft.updateParameters();
    
    
    ofColor right = rightColor.get().getLerped(mix, (split.getMax()-split) / split.getMax());

    matRight.setDiffuseColor(right);
    matRight.setAmbientColor(right);
    matRight.updateParameters();*/
    
    //box1.setPosition(origin);
    // push section down
}

void BoxSplit::drawModel() {
    
    ofSetColor(255,75);
    //box.drawWireframe();
    ofSetColor(255,20);
    
    boxLeftFloor.draw();
    boxRightFloor.draw();
    
    boxLeftWall.draw();
    boxRightWall.draw();
    //box1.draw();
    //box2.draw();
    
}


