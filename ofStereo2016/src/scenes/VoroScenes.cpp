//
//  VoroScenes.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/7/16.
//
//

#include "VoroScenes.hpp"

void VoroScenes::setup() {
    
    voroSeed.addListener(this, &VoroScenes::reconstruct<int>);
    voroNumCells.addListener(this, &VoroScenes::reconstruct<int>);
    
    reconstruct();
    
}

void VoroScenes::draw() {

    
    ofSetColor(255);
    ofEnableAlphaBlending();
    ofEnableLighting();
    
    voroMat.begin();
    floorNode.draw(&voroMat);
    wallNode.draw(&voroMat);
    voroMat.end();
    
}

void VoroScenes::update() {
    
    if(doReconstruct){
        reconstruct();
        doReconstruct = false;
    }
    
    // update materials
    
    voroMat.setDiffuseColor(voroColor.get());
    voroMat.setAmbientColor(voroColor.get()*0.5);
    voroMat.updateParameters();
    
    // rotate wall downwards
    
    ofMatrix4x4 mWallRotation;
    ofQuaternion qWallRotation;

    qWallRotation.makeRotate(-90*animationsWallDown, 1.0, 0.0, 0.0 );
    
    mWallRotation.makeIdentityMatrix();
    mWallRotation.translate(wallBox.getGlobalPosition());
    mWallRotation.rotate(qWallRotation);
    mWallRotation.translate(-wallBox.getGlobalPosition());
    wallNode.setTransformMatrix(mWallRotation);
    
    // break wall apart
    int i = 0;
    for (auto node : wallNode.getChildren()) {
        ofNode & originalNode = wallNodeOriginal[i++];
        ofVec3f orgNodePos = originalNode.getPosition();
        float breakAmount = originalNode.getGlobalPosition().z * animationsWallDown;
        float breakAmountSq = breakAmount * animationsWallDown;
        
        ofVec3f nodePos = node->getGlobalPosition();
        node->setPosition(orgNodePos.x*(1.0+(2.0*animationsWallDown*animationsWallDown)), orgNodePos.y - (breakAmountSq * 5.0), orgNodePos.z + (0.04 * breakAmountSq) + (breakAmountSq * 0.02 * (ofSignedNoise(i*0.1) * 200.0)) - (2000.0 * powf(animationsWallDown, 4.0)));
        ofQuaternion q = originalNode.getOrientationQuat();
        float yFactor = originalNode.getGlobalPosition().y/wallBox.getHeight();
        float xFactor = originalNode.getGlobalPosition().x/wallBox.getHeight();
        q.slerp(yFactor*3.0, q, qWallRotation);
        ofQuaternion qYrot;
        qYrot.makeRotate(animationsWallDown*(xFactor*90), 0.0, 1.0, 0.0 );
        node->setOrientation(q*qYrot);
        node->setScale(1.0-(animationsWallDown*orgNodePos.y/floorBox.getHeight()));
    }
}

void VoroScenes::reconstruct(){
    
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();

    ofSeedRandom(voroSeed.get());
    
    floorBox.setParent(world->origin);
    floorBox.set(_s.x, _s.x*0.5, _s.z, 4,2,4);
    floorBox.setScale(1,1,1);
    floorBox.setGlobalPosition( ofVec3f(0, -floorBox.getHeight()/2, floorBox.getDepth()/2) );
    
    floorNode.setupFromBoundingBox(floorBox.getWidth(), floorBox.getHeight(), floorBox.getDepth(), voroNumCells, false,false,false);
    floorNode.setParent(floorBox);


    wallBox.setParent(world->origin);
    wallBox.set(_s.x, _s.y, _s.x*0.5, 4,4,2);
    wallBox.setScale(1,1,1);
    wallBox.setGlobalPosition( ofVec3f(0, wallBox.getHeight()/2, -wallBox.getDepth()/2) );
    
    ofSeedRandom(voroSeed.get()+147);
    wallNode.setupFromBoundingBox(wallBox.getWidth(), wallBox.getHeight(), wallBox.getDepth(), voroNumCells, false,false,false);
    wallNode.setParent(wallBox);

    wallNodeOriginal.clear();
    
    for(auto node : wallNode.getChildren()){
        ofNode newNode(*node);
        //newNode.clearParent(true);
        wallNodeOriginal.push_back(newNode);
    }
    
}

void VoroScenes::drawModel() {
    
    ofSetColor(voroColor.get(),20);
    floorBox.drawWireframe();
    floorNode.draw();
    ofSetColor(voroColor.get(),20);
    wallBox.drawWireframe();
    wallNode.draw();
    ofSetColor(255,255,0,25);
    for(auto node : wallNodeOriginal){
        ofDrawSphere(node.getGlobalPosition(), 10);
    }
    
}


