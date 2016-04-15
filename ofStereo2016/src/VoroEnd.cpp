//
//  VoroEnd.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/8/16.
//
//

#include "VoroEnd.hpp"

void VoroEnd::setup() {

    oceanSeed.addListener(this, &VoroEnd::reconstructOcean<int>);
    oceanNumCells.addListener(this, &VoroEnd::reconstructOcean<int>);

    wallSeed.addListener(this, &VoroEnd::reconstructWall<int>);
    wallNumCells.addListener(this, &VoroEnd::reconstructWall<int>);
    
    reconstruct();
}

void VoroEnd::draw() {
    
    ofEnableAlphaBlending();
    ofSetColor(255);
    mat.begin();
    
    wall.draw(&mat);
    
    ocean.draw(&mat);
    mat.end();
}

void VoroEnd::update() {
    
    
    if(bOceanReconstruct) {
        reconstructOcean();
        bOceanReconstruct = false;
    }
    
    if(bWallReconstruct) {
        reconstructWall();
        bWallReconstruct = false;
        
    }
    
    ofVec3f _s = getWorldSize();

    mat.setDiffuseColor(matcolor.get());
    mat.setAmbientColor(matcolor.get()*0.5);
    mat.updateParameters();
    
    ocean.setPosition(oceanOrigin.get());
    
    ofMatrix4x4 m;
    m.makeIdentityMatrix();
    ofVec3f rotCenter(0, _s.y/2, 0);
    
    //m.translate(wallOrigin.get());
    
    m.translate(-rotCenter);
    m.rotate(openWall.get(), 1, 0, 0);
    m.translate(rotCenter);
    
    m.translate(0, -fallWall.get()*_s.y, 0);
    
    wall.setTransformMatrix(m);
    
    
    // apply noise
    // dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
    
    auto applyNoise = [](VoroNode & node, float & time, float speed, ofVec3f amount) {
        
        time += speed/100.0;
        
        for( auto c : node.getChildren()) {
            
            float n = ofSignedNoise((c->getPosition().x + (time)));
            
            ofVec3f nP(n, n, n);
            nP *= amount;
            c->renderPosOffset = nP;
        }
    };
    
    applyNoise(ocean, oceanNoiseTime, oceanNoiseDisplaceSpeed, ofVec3f(0, oceanNoiseDisplaceAmount, 0));
    applyNoise(wall, wallNoiseTime, wallNoiseDisplaceSpeed, ofVec3f(0, 0, wallNoiseDisplaceAmount));   
}

void VoroEnd::reconstructWall(){
    ofVec3f _s = getWorldSize();
    
    ofSeedRandom(wallSeed.get());
    wall.setupFromBoundingBox(_s.x, _s.y, 10, wallNumCells, false,false,false);
    wall.setParent(wallCenter);
}

void VoroEnd::reconstructOcean(){
    ofVec3f _s = getWorldSize();
    
    ofSeedRandom(oceanSeed.get());
    ocean.setupFromBoundingBox(_s.x, 10, _s.z, oceanNumCells, false,false,false);
    ocean.setParent(floorCenter);
}

void VoroEnd::drawModel() {
    ofSetColor(255,75);
    
    ocean.draw();
    wall.draw();
}


