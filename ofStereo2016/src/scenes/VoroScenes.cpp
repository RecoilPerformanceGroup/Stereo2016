//
//  VoroScenes.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/7/16.
//
//

#include "VoroScenes.hpp"

void VoroScenes::setup() {
    
    seed.addListener(this, &VoroScenes::reconstruct<int>);
    numCells.addListener(this, &VoroScenes::reconstruct<int>);
    
    reconstruct();
    
}

void VoroScenes::draw() {

    
    ofSetColor(255);
    ofEnableAlphaBlending();
    ofEnableLighting();
    
    mat.begin();
    mainNode.draw(&mat);
    mat.end();
    
}

void VoroScenes::update() {
    
    if(doReconstruct){
        reconstruct();
        doReconstruct = false;
    }
    
    mat.setDiffuseColor(clusterColor.get());
    mat.setAmbientColor(clusterColor.get()*0.5);
    mat.updateParameters();
    box.setPosition(origin);
    
}

void VoroScenes::reconstruct(){
    
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

void VoroScenes::drawModel() {
    
    ofSetColor(255,75);
    box.drawWireframe();
    ofSetColor(255,20);
    mainNode.draw();
    
}


