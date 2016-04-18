//
//  SoundField.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/3/16.
//
//

#include "SoundField.hpp"


void SoundField::setup() {
    clusterNumCells.addListener(this, &SoundField::reconstructCluster<int>);
    reconstructCluster();
}

void SoundField::draw() {
    ofEnableAlphaBlending();
    ofSetColor(255);
    matCluster.begin();
    cluster.draw(&matCluster);
    matCluster.end();
}

void SoundField::update() {
    
    if(soundWaveCreate){
        soundWave sw;
        sw.type = soundWaveType;
        sw.position = sw.origin = soundWaveOriginNormalised.get() * getWorldSize();
        sw.velocity = soundWaveVelocity * soundWaveDirection;
        sw.force = soundWaveForce;
        soundWaves.push_back(sw);
        soundWaveCreate.set(false);
    }
    
    if(soundWaveClear){
        soundWaves.clear();
        soundWaveClear.set(false);
    }

    std::vector<soundWave>::iterator iter;
    for (iter = soundWaves.begin(); iter != soundWaves.end(); ) {
        iter->position += iter->velocity * ofGetLastFrameTime() * 60.0;
        if(iter->position.distance(world->origin.getPosition()) > 4000.0){
            iter = soundWaves.erase(iter);
        } else {
            ++iter;
        }
    }
    
    matCluster.updateParameters();
    
    clusterRotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
                                                             clusterRotationAxis.get().x*
                                                             clusterRotationSpeed.get(),
                                                             clusterRotationAxis.get().y*
                                                             clusterRotationSpeed.get(),
                                                             clusterRotationAxis.get().z
                                                             *clusterRotationSpeed.get());
    
    cluster.setOrientation(clusterRotation);
    cluster.setGlobalPosition(clusterOrigin);
    cluster.setScale(clusterScale.get());
    
    for(auto c : cluster.getChildren()) {
        c->setScale(clusterScaleCells);
        for(auto cc : c->getChildren()) {
            cc->setScale(clusterScaleCells);
        }
    }
    
    matCluster.setDiffuseColor(clusterColor.get());
    
}

void SoundField::reconstructCluster(){
    ofSeedRandom();
    cluster.setupFromBoundingBox(200,200,200,clusterNumCells, true, true, true);
    cluster.setGlobalPosition(clusterOrigin);
}

void SoundField::drawModel() {
    ofSetColor(255,15);
    cluster.draw();
    for(auto sw : soundWaves){
        if(sw.type == 0){
            ofPlanePrimitive p;
            p.set(1000,1000, 3, 3);
            p.setParent(world->origin);
            p.setGlobalPosition(sw.position);
            p.lookAt(sw.position+sw.velocity, ofVec3f(0,0,1));
            p.draw();
        }
        if(sw.type == 1){
            ofDrawSphere(sw.origin, sw.origin.distance(sw.position));
        }
    }
}


