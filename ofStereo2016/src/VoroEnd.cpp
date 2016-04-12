//
//  VoroEnd.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/8/16.
//
//

#include "VoroEnd.hpp"

void VoroEnd::setup() {
    
    
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();
    
    wallCenter.setParent(world->origin);
    floorCenter.setParent(world->origin);
    
    wallCenter.setPosition(0, _s.y, 0);
    
    floorCenter.setPosition(0, 0, _s.z/2);
    
    oceanSeed.addListener(this, &VoroEnd::reconstructOcean<int>);
    oceanNumCells.addListener(this, &VoroEnd::reconstructOcean<int>);

    wallSeed.addListener(this, &VoroEnd::reconstructWall<int>);
    wallNumCells.addListener(this, &VoroEnd::reconstructWall<int>);
    
    reconstructWall();
    reconstructOcean();
}

void VoroEnd::draw() {
    
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();

    
    ofEnableAlphaBlending();
    ofSetColor(255);
    mat.begin();
    
    ofPushMatrix();
    ofTranslate(0, -_s.y/2, 0);
    ofRotateX(openWall.get());
    ofTranslate(0, _s.y/2, 0);
    
    wall.draw(&mat);
    
    
    ofPopMatrix();
    
    
    ocean.draw(&mat);
    mat.end();
    
}

void VoroEnd::update() {
    mat.setDiffuseColor(matcolor.get());
    mat.setAmbientColor(ofFloatColor(1.0,0,0,1.0));
    mat.updateParameters();
    
    wall.setPosition(wallOrigin.get());
    ocean.setPosition(oceanOrigin.get());
    
    
    // apply noise -
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
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();
    
    ofSeedRandom(wallSeed.get());
    wall.setupFromBoundingBox(_s.x, _s.y, 10, wallNumCells, false,false,false);
    wall.setParent(wallCenter);
}

void VoroEnd::reconstructOcean(){
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();
    
    ofSeedRandom(oceanSeed.get());
    ocean.setupFromBoundingBox(_s.x, 10, _s.z, oceanNumCells, false,false,false);
    ocean.setParent(floorCenter);
    
}

void VoroEnd::drawModel() {
    ofSetColor(255,75);
    
    ocean.draw();
    
}


