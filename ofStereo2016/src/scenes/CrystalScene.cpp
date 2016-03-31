//
//  CrystalScene.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/3/16.
//
//

#include "CrystalScene.hpp"


void CrystalScene::setup() {
    
    crystalSeed.addListener(this, &CrystalScene::reconstructCrystal<int>);
    clusterNumCells.addListener(this, &CrystalScene::reconstructCluster<int>);

    reconstructCrystal();
    reconstructCluster();
    
}

void CrystalScene::draw() {
    
    ofEnableAlphaBlending();
    ofSetColor(255);
        matCluster.begin();
        cluster.draw(&matCluster);
        matCluster.end();
    ofDisableDepthTest();
    matCrystal.begin();
    crystalBoulder->draw(&matCrystal);
    matCrystal.end();
    ofEnableDepthTest();
    
}

void CrystalScene::update() {
    
    matCrystal.updateParameters();
    matCluster.updateParameters();

    crystalBoulder->setScale(crystalSize/crystalBoulder->boundingBox.getWidth());
    crystalBoulder->setGlobalPosition(crystalOrigin);
    
    rotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
                                                      clusterRotationAxis.get().x*
                                                      clusterRotationSpeed.get(),
                                                      clusterRotationAxis.get().y*
                                                      clusterRotationSpeed.get(),
                                                      clusterRotationAxis.get().z
                                                      *clusterRotationSpeed.get());
    
    cluster.setOrientation(rotation);
    cluster.setGlobalPosition(clusterOrigin);
    cluster.setScale(clusterScale.get());
    
    for(auto c : cluster.getChildren()) {
        
        c->setScale(clusterScaleCells);
        
        for(auto cc : c->getChildren()) {
            
            cc->setScale(clusterScaleCells);
        }
    }
    
    matCrystal.setDiffuseColor(crystalColor.get());
    matCluster.setDiffuseColor(clusterColor.get());
    matCrystal.setAmbientColor(ofFloatColor(0,0,0,0));
}

void CrystalScene::reconstructCluster(){
    ofSeedRandom();
    cluster.setupFromBoundingBox(200,200,200,clusterNumCells, true, true, true);
    cluster.setGlobalPosition(clusterOrigin);
}

void CrystalScene::reconstructCrystal(){
    ofSeedRandom(crystalSeed);
    crystal.setupFromBoundingBox(crystalSize*3, crystalSize*3, crystalSize*3, 10, true, true, true);
    // TODO: get first boulder from center node into center pointer
    for (auto c : crystal.getChildren()) {
        crystalBoulder = c;
        break;
    }
    crystalBoulder->setGlobalPosition(crystalOrigin);
}

void CrystalScene::drawGui() {
    ofSetColor(255,75);
    crystalBoulder->draw();
    cluster.draw();
}


