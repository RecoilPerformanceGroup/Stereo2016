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
    
    
    
    
    ofPushMatrix();
    
    ofPoint anchor = box.getPosition() + ofVec3f(0, box.getHeight()/2, 0) + pivotOffset;
    
    ofDrawSphere(anchor + pivotOffset, 10);
    
    ofTranslate(anchor + pivotOffset);
    
    ofRotate(ofMap(pivotOffset.get().x, 100, -100, -45, 45), 0, 0, 1);
    ofRotate(ofMap(pivotOffset.get().z, -100, 100, -45, 45), 1, 0, 0);
    
    ofTranslate(-(anchor + pivotOffset));
    
    
    //ofMatrix4x4 m = ofGetCurrentNormalMatrix();
    //ofMatrix4x4 m = ofGetCurrentNormalMatrix();
    
    box.draw();

    
    ofPopMatrix();
    
    

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
    
    /*clusterRotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
                                                      clusterRotationAxis.get().x*
                                                      clusterRotationSpeed.get(),
                                                      clusterRotationAxis.get().y*
                                                      clusterRotationSpeed.get(),
                                                      clusterRotationAxis.get().z
                                                      *clusterRotationSpeed.get());
    */
    //cluster.setOrientation(clusterRotation);
    
    cluster.setGlobalPosition(clusterOrigin);
    //
    //cluster.rotateAround(<#float degrees#>, <#const ofVec3f &axis#>, <#const ofVec3f &point#>)
    
    cluster.setScale(clusterScale.get());
    
    for(auto c : cluster.getChildren()) {
        
        c->setScale(clusterScaleCells);
        
        for(auto cc : c->getChildren()) {
            
            cc->setScale(clusterScaleCells);
        }
    }
    
    //cluster.rotateAround(sin(ofGetElapsedTimef()/100.0)*180, ofVec3f(0,0,1), pivotAround);
    
    //cluster.lookAt(pivotAround, ofVec3f(0,0,1));

    
    
    matCrystal.setDiffuseColor(crystalColor.get());
    matCluster.setDiffuseColor(clusterColor.get());
    matCrystal.setAmbientColor(ofFloatColor(0,0,0,0));
    
    box.setScale(2,1,2);
    box.setPosition(clusterOrigin);
    
    ofVec3f p = box.getPosition();
    //box.setPosition(pivotAround);
    
    //box.setOrientation(ofVec3f(0,0,45));
    
    box.setPosition(p);
    
    //box.setTransformMatrix(<#const ofMatrix4x4 &m44#>)
    
    //box.rotateAround(sin(ofGetElapsedTimef())*45, pivotAround, box.getPosition()+ofVec3f(0, box.getHeight() /2, 0));
    
    
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
    
    

    
    box.draw();
    
    
}


