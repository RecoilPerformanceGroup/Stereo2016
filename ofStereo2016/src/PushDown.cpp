//
//  PushDown.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/12/16.
//
//


#include "PushDown.hpp"

void PushDown::setup() {
    
    seed.addListener(this, &PushDown::reconstruct<int>);
    numCells.addListener(this, &PushDown::reconstruct<int>);
    
    reconstruct();
    
}

void PushDown::draw() {
    
    
    ofSetColor(255);
    ofEnableAlphaBlending();
    ofEnableLighting();
    
    mat.begin();
    mainNode.draw(&mat);
    mat.end();
    
}

void PushDown::update() {
    
    if(doReconstruct){
        reconstruct();
        doReconstruct = false;
    }
    
    mat.setDiffuseColor(clusterColor.get());
    mat.setAmbientColor(clusterColor.get()*0.5);
    mat.updateParameters();
    box.setPosition(origin);
    
    
    // push section down
    
    
    
    
    for( auto & n : mainNode.getChildren()) {
        
        ofVec3f pos = n->getGlobalPosition() * ofVec3f(1,0,1);
        
        
        
        // gradual push
        float push = ofxeasing::map_clamp(pushRadius-(pos.distance(pushPos.get()*ofVec3f(1,0,1))), 0, pushRadius, 0, pushDown, ofxeasing::linear::easeOut);
        
        // gradual pull
        float pull = ofxeasing::map_clamp(pullRadius-(pos.distance(pullPos.get()*ofVec3f(1,0,1))), 0, pullRadius, 0, pullUp, ofxeasing::linear::easeOut);
        
        
        n->renderPosOffset.y = push + pull;
        
        
    }
    
    
    
    for( auto & n : mainNode.getChildren()) {
        
        ofVec3f pos = n->getGlobalPosition() * ofVec3f(1,0,1);
        

        
    }
    
    
}

void PushDown::reconstruct(){
    
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();
    
    box.setParent(world->origin);
    box.set(_s.x, _s.x*0.5, _s.z, 4,2,4);
    box.setScale(1,1,1);
    box.setGlobalPosition( ofVec3f(0, -box.getHeight()/2, box.getDepth()/2) );
    
    origin.set(box.getGlobalPosition());
    
    ofSeedRandom(seed.get());
    mainNode.setupFromBoundingBox(box.getWidth(), box.getHeight(), box.getDepth(), numCells, true,false,true);
    mainNode.setParent(box);
}

void PushDown::drawModel() {
    
    ofSetColor(255,75);
    //box.drawWireframe();
    ofSetColor(255,20);
    //mainNode.draw();
    
}


