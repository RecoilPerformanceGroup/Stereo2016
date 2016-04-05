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
    matCrystal.begin();
    crystalBoulder->draw(&matCrystal);
    matCrystal.end();
    
}

void CrystalScene::update() {
    
    matCrystal.updateParameters();
    matCluster.updateParameters();

    crystalRotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
                                                             crystalRotationAxis.get().x*
                                                             crystalRotationSpeed.get(),
                                                             crystalRotationAxis.get().y*
                                                             crystalRotationSpeed.get(),
                                                             crystalRotationAxis.get().z
                                                             *crystalRotationSpeed.get());
    
    crystalBoulder->setOrientation(crystalRotation);
    crystalBoulder->setScale(crystalSize/crystalBoulder->boundingBox.getWidth());
    crystalBoulder->setGlobalPosition(crystalOrigin);
    
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
    
    matCrystal.setDiffuseColor(crystalColor.get());
    matCluster.setDiffuseColor(clusterColor.get());
    matCrystal.setAmbientColor(ofFloatColor(0,0,0,0));
}

void CrystalScene::reconstructCluster(){
    ofSeedRandom();
    cluster.setupFromBoundingBox(200,200,200,clusterNumCells, true, true, true);
    cluster.setGlobalPosition(clusterOrigin);
    
    // intersect with a mesh - recursively for children
    
    for (auto c : cluster.getChildren()) {
        intersect = ofSpherePrimitive(60, 10).getMesh();
        
        /*for(auto & v : intersect.getVertices()) {
            v += c->mesh.getCentroid().getMiddle(c->mesh.getVertex(0));
        }*/
        
        //ofxCSG::meshDifference(<#ofMesh &a#>, <#ofMesh &b#>, <#ofMesh &outMesh#>)
        ofxCSG::meshDifference(c->mesh, intersect, c->mesh);

    }

}

void CrystalScene::reconstructCrystal(){
    //TODO: Fix inconsistent random
    ofSeedRandom(crystalSeed);
    crystal.setupFromBoundingBox(crystalSize*3, crystalSize*3, crystalSize*3, 15, true, true, true);
    
/*    for (auto c : crystal.getChildren()) {
        crystalBoulder = c;
        break;
    }*/
    crystalBoulder = &crystal;
    crystalBoulder->setGlobalPosition(crystalOrigin);
}

void CrystalScene::drawModel() {
    ofSetColor(255,75);
    crystalBoulder->draw();
    cluster.draw();
}


