//
//  CrystalScene.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/3/16.
//
//

#include "CrystalScene.hpp"
#include "ofxEasing.h"

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
    //crystalOrigin = (dp(1)+dp(2))/2.0;
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
    
    ofVec3f camPos = world->physical_camera_pos_cm;

    int i = 0;
    
    for(auto c : cluster.getChildren()) {

        c->setScale(clusterScaleCells);

        // tint black when in front of camera
        
        float angleTowardsCamNormalised = (180.0-c->getGlobalPosition().angle(camPos))/180.0;
        c->setTint(clusterColor->getLerped(ofFloatColor::black, angleTowardsCamNormalised));

        if(clusterAvoid > 0.0){
        // avoid crystal
        
        float minDistanceToCrystal = 100+(crystalSize.get() - (c->boundingBox.getSize().length()*c->getScale().x));
        float minDistanceToCrystalSquared = minDistanceToCrystal * minDistanceToCrystal;
        
        float distToCrystalSquared = c->getGlobalPosition().squareDistance(crystalBoulder->getGlobalPosition());
        
        float distMultiplier = 4.0;
        
        if(!c->bOffsetPositionOveride){
            c->positionOverideOrigin = c->getPosition();
            c->positionOverideTarget = c->positionOverideOrigin.getScaled(minDistanceToCrystal*0.5);
            c->bOffsetPositionOveride = true;
            c->positionOverideAmount = 0.0;
        } else {
            float dist = (c->positionOverideOrigin*c->getParent()->getGlobalTransformMatrix()).distance(crystalBoulder->getGlobalPosition());
            c->positionOverideAmount = clusterAvoid*ofMap(dist, minDistanceToCrystal*(distMultiplier*0.85), 0, 0.0, 1.0);
            c->setPosition(c->positionOverideOrigin.getInterpolated(c->positionOverideTarget, ofxeasing::map_clamp(c->positionOverideAmount, 0.0, 1.0, 0.0, 1.0, & ofxeasing::quad::easeInOut)));
        }

        }
        
        for(auto cc : c->getChildren()) {
            
            cc->setScale(clusterScaleCells);
        }
        
        i++;
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
    
    /*for (auto c : cluster.getChildren()) {
        intersect = ofSpherePrimitive(60, 10).getMesh();
        
        //ofxCSG::meshDifference(<#ofMesh &a#>, <#ofMesh &b#>, <#ofMesh &outMesh#>)
        ofxCSG::meshDifference(c->mesh, intersect, c->mesh);

    }*/

// global
    
     intersect = ofSpherePrimitive(140, 10).getMesh();
    
    /*for (auto c : cluster.getChildren()) {
        
        ofMesh translated;
        translated = c->mesh;
        
        for (auto & v : translated.getVertices()) {
            ofVec3f p = v * c->getGlobalTransformMatrix();
            v.set( p );
        }
         
         ofxCSG::meshDifference(translated, intersect, c->mesh );
     
     }*/

}

void CrystalScene::reconstructCrystal(){
    //TODO: Fix inconsistent random
    ofSeedRandom(crystalSeed);
    crystal.setupFromBoundingBox(crystalSize*3, crystalSize*3, crystalSize*3, 15, true, true, true);
    crystalBoulder = &crystal;
    crystalBoulder->setGlobalPosition(crystalOrigin);
}

void CrystalScene::drawModel() {
    ofEnableDepthTest();
    ofSetColor(255,75);
    crystalBoulder->draw();
    cluster.draw();
    ofDisableDepthTest();
}


