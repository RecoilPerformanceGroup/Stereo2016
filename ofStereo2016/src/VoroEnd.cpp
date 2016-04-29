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
    
    wallSize.addListener(this, &VoroEnd::reconstructWall<ofVec3f>);
    
    wallCenter.setParent(world->origin);
    floorCenter.setParent(world->origin);
    
    reconstruct();
}

void VoroEnd::draw() {
    
    ofEnableAlphaBlending();
    ofEnableDepthTest();
    ofEnableLighting();
    ofSetColor(255);
    
    wall.draw(&mat);
    ocean.draw(&mat);
    //oceanHorizon.draw(&mat);
    
    if(fadeOutFloor>0) {
        ofSetColor(0, fadeOutFloor*255.0);
        ofTranslate(0, 0, getWorldSize().z/2);
        ofDrawBox(getWorldSize().x, 1, getWorldSize().z);
    }
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
    
    //oceanHorizon.setOrientation(ofVec3f(-90+openWall.get(),0,0));
    
    //oceanHorizon.setPosition(horizonOrigin.get() - ofVec3f( 0, 0, oceanHorizon.boundingBox.getDepth()/2 + wall.boundingBox.getHeight()*3));
    
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
            
            c->renderScale = scaleCells.get();
            c->bRenderScaleSet = true;
            
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
            
            // dp still
            float dist = dp().distance(c->getGlobalPosition());
            float pers = 1;
            
            if(dist < dpPersistRadius) {
                pers = 1-dpPersist;
            } else {
                pers = ofMap(dist, dpPersistRadius, dpPersistRadius+150, 1-dpPersist, 1, true);
            }
            
            ofVec3f nP(n, n, n);
            nP *= amount * edgeStill * pers;
            
            c->renderPosOffset = nP;
            
            float cc = ofMap(nP.length(), 0, max<float>(amount.length()/2, minDarkFade.get()), 1.0, 0, true);
            
            c->setTint(ofFloatColor(cc,1));
        }
    };
    
    applyNoise(ocean, oceanNoiseTime, oceanNoiseDisplaceSpeed, ofVec3f(0, oceanNoiseDisplaceAmount, 0));
    
    applyNoise(wall, wallNoiseTime, wallNoiseDisplaceSpeed, ofVec3f(0, 0, wallNoiseDisplaceAmount));
    
    //applyNoise(oceanHorizon, oceanNoiseTime, oceanNoiseDisplaceSpeed, ofVec3f(0, oceanNoiseDisplaceAmount, 0));
    
    for( auto c : ocean.getChildren()) {
        
        float dist = dp().distance(c->getGlobalPosition());
        
        float pers = 1;
        
        if(dist < dpPersistRadius) {
            pers = 1-dpPersist;
        } else {
            pers = ofMap(dist, dpPersistRadius, dpPersistRadius+150, 1-dpPersist, 1, true);
        }
        
        c->renderPosOffset.y += (oceanFall*pers);
        
        
        c->setTint(c->tint * ofMap(oceanFall.get()*pers, 0, -400, 1, 0, true));
        
        c->setTint(c->tint * 1-fadeOutFloor);
        
        /*if(c->getGlobalPosition()) {
        }
            
            float fullValueAtDistInCM = stillAtEdgeDist.get();
        edgeStill =  ofClamp(abs(c->getGlobalPosition().z - edgeAt) / fullValueAtDistInCM, 0, 1);*/
        
    }
    
    /*for( auto c : oceanHorizon.getChildren()) {
        
        ofVec3f r = c->getGlobalOrientation().asVec3();
        
        c->setOrientation(ofVec3f());
        // rotate angle to match wall open
        
    
    }*/

    
    
    
}

void VoroEnd::reconstructWall(){
    ofVec3f _s = getWorldSize();
    
    ofSeedRandom(wallSeed.get());
    
    wall.setupFromBoundingBox(_s.x*wallSize.get().x, _s.y*wallSize.get().y, 10*wallSize.get().z, wallNumCells, false,false,false);
    flagResetTransitionReferences = true;
    
    wall.setParent(wallCenter);
}

void VoroEnd::reconstructOcean(){
    ofVec3f _s = getWorldSize();
    
    ofSeedRandom(oceanSeed.get());
    ocean.setupFromBoundingBox(_s.x*1, 10, _s.z/*-200*/, oceanNumCells, false,false,false);
    ocean.setParent(floorCenter);
    
    //oceanHorizon.setupFromBoundingBox(_s.x*3, 10, _s.z*3, oceanNumCells*1.2, true,true,true);
    //oceanHorizon.setParent(floorCenter);
}

void VoroEnd::drawModel() {
    
    ofEnableDepthTest();
    ofSetColor(255,75);
    
    ocean.draw();
    wall.draw();
    //oceanHorizon.draw();
    
}


