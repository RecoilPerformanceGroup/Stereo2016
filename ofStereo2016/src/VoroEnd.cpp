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
    ofEnableLighting();
    ofSetColor(255);
    
    mat.begin();
    wall.draw(&mat);
    ocean.draw(&mat);
    mat.end();
    
    ofSetColor(0, 0, 0, fadeOutFlor*255);
    ofTranslate(0, 0, getWorldSize().z/2);
    ofDrawBox(getWorldSize().x, 10, getWorldSize().z);
    
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
    
    ocean.setPosition(oceanOrigin.get() - ofVec3f( 0, 0, ocean.boundingBox.getDepth() /2));
    
    ofMatrix4x4 m;
    m.makeIdentityMatrix();
    ofVec3f rotCenter(0, wall.boundingBox.getHeight()/2/*_s.y/2*/, 0);
    
    m.translate(-rotCenter);
    m.rotate(openWall.get(), 1, 0, 0);
    m.translate(rotCenter);
    
    m.translate(wallOrigin.get() - ofVec3f(0, wall.boundingBox.getHeight()/2, 0));
    m.translate(0, -fallWall.get()*_s.y, 0);
    wall.setTransformMatrix(m);
    
    // TODO: - optimisation for apply noise
    // dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
    auto applyNoise = [this](VoroNode & node, float & time, float speed, ofVec3f amount) {
        
        time += speed/100.0;
        for( auto c : node.getChildren()) {
            
            float n = ofSignedNoise((c->getPosition().x + (time)));
            
            float edgeStill = 1;
            
            if(openWall.get() > 45) {
            float edgeAt = 0;
            float fullValueAtDistInCM = stillAtEdgeDist.get();
            edgeStill =  ofClamp(abs(c->getGlobalPosition().z - edgeAt) / fullValueAtDistInCM, 0, 1);
            
            if(edgeStill < 1) {
                edgeStill *= (1 - stillAtEdge.get()) ;
            }
            }
            
            ofVec3f nP(n, n, n);
            nP *= amount * edgeStill;
            c->renderPosOffset = nP;
        }
    };
    
    applyNoise(ocean, oceanNoiseTime, oceanNoiseDisplaceSpeed, ofVec3f(0, oceanNoiseDisplaceAmount, 0));
    applyNoise(wall, wallNoiseTime, wallNoiseDisplaceSpeed, ofVec3f(0, 0, wallNoiseDisplaceAmount));   
}

void VoroEnd::reconstructWall(){
    ofVec3f _s = getWorldSize();
    
    ofSeedRandom(wallSeed.get());
    wall.setupFromBoundingBox(_s.x*1.0, _s.y*1.0, 10, wallNumCells, false,false,false);
    wall.setParent(wallCenter);
}

void VoroEnd::reconstructOcean(){
    ofVec3f _s = getWorldSize();
    
    ofSeedRandom(oceanSeed.get());
    ocean.setupFromBoundingBox(_s.x, 10, _s.z-200, oceanNumCells, true,false,true);
    ocean.setParent(floorCenter);
}

void VoroEnd::drawModel() {
    ofSetColor(255,75);
    
    ocean.draw();
    wall.draw();
    


    
}


