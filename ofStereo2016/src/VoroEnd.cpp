//
//  VoroEnd.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/8/16.
//
//

#include "VoroEnd.hpp"

void VoroEnd::setup() {
    
    seed.addListener(this, &VoroEnd::reconstruct<int>);
    numCells.addListener(this, &VoroEnd::reconstruct<int>);
    
    
    reconstruct();
    
}

void VoroEnd::draw() {
    
    
    ofEnableAlphaBlending();
    ofSetColor(255);
    /*ofEnableAlphaBlending();
     ofSetColor(255);
     mat.begin();
     yClipped.draw(&mat);
     mat.end();*/
    
    
    ofDrawSphere(subtractOffset, 10);
    
    ofPushMatrix();
    
    //ofPoint anchor = box.getPosition() + ofVec3f(0, box.getHeight()/2, 0) + pivotOffset;
    
    /*ofDrawSphere(anchor + pivotOffset, 10);
     
     ofTranslate(anchor + pivotOffset);
     
     ofRotate(ofMap(pivotOffset.get().x, 100, -100, -45, 45), 0, 0, 1);
     ofRotate(ofMap(pivotOffset.get().z, -100, 100, -45, 45), 1, 0, 0);
     
     ofTranslate(-(anchor + pivotOffset));*/
    //box.draw();
    
    ofPopMatrix();
    
    /* of3dPrimitive newBox = box;
     
     for(auto c : unClipped.getChildrenInSphere(dp(0), 80)) {
     ofxCSG::meshDifference(newBox.getMesh(), c->getBakedMesh(), newBox.getMesh());
     //c->draw();
     }
     */
    ofEnableLighting();
    
    mat.begin();
    mat.setWorldMatrix(ofMatrix4x4::newIdentityMatrix());
    newBox.draw();
    mat.end();
    
}

void VoroEnd::update() {
    
    mat.setDiffuseColor(clusterColor.get());
    mat.setAmbientColor(ofFloatColor(1.0,0,0,1.0));
    mat.updateParameters();
    
    
    yClipped.setPosition(subtractOffset);
    
    
    newBox = VoroNode::bakeMesh(box.getMesh(), box);
    for(auto c : yClipped.getNearestChildren(dp(0)+subtractOffset.get(), 2)) {
        ofxCSG::meshDifferencePostFlipB(newBox, c->getBakedMesh(), newBox);
        newBox.setupIndicesAuto();
        //c->draw();
    }
    //ofMesh::
    
    //crystalBoulder->setOrientation(crystalRotation);
    //cluster->setScale(crystalSize/crystalBoulder->boundingBox.getWidth());
    //crystalOrigin = (dp(1)+dp(2))/2.0;
    //cluster.setGlobalPosition(origin);
    
    /*clusterRotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
     clusterRotationAxis.get().x*
     clusterRotationSpeed.get(),
     clusterRotationAxis.get().y*
     clusterRotationSpeed.get(),
     clusterRotationAxis.get().z
     *clusterRotationSpeed.get());
     */
    //cluster.setOrientation(clusterRotation);
    
    //
    //cluster.rotateAround(<#float degrees#>, <#const ofVec3f &axis#>, <#const ofVec3f &point#>)
    
    /*yClipped.setScale(scale.get());
     
     for(auto c : cluster.getChildren()) {
     c->setScale(scaleCells);
     for(auto cc : c->getChildren()) {
     cc->setScale(scaleCells);
     }
     }*/
    
    //mat.setDiffuseColor(crystalColor.get());
    //mat.setDiffuseColor(clusterColor.get());
    //mat.setAmbientColor(ofFloatColor(0,0,0,0));
    
    // box.setScale(2,1,2);
    // box.setPosition(origin);
    
    //ofVec3f p = box.getPosition();
    //box.setPosition(pivotAround);
    
    //box.setOrientation(ofVec3f(0,0,45));
    
    //box.setPosition(p);
    
    //box.setTransformMatrix(<#const ofMatrix4x4 &m44#>)
    
    //box.rotateAround(sin(ofGetElapsedTimef())*45, pivotAround, box.getPosition()+ofVec3f(0, box.getHeight() /2, 0));
    
    
}

void VoroEnd::reconstruct(){
    
    ofVec3f _s = globalParams->getVec3f("stage_size_cm").get();
    
    box.setParent(world->origin);
    
    box.set(_s.x, _s.y*0.4, _s.z, 4,4,4);
    box.setScale(1,1,1);
    box.setPosition( ofVec3f(0, -box.getHeight()/2, box.getDepth()/2) );
    
    scale.set(1);
    origin.set(ofVec3f(0,0,0));
    
    ofSeedRandom(seed.get());
    yClipped.setupFromBoundingBox(box.getWidth(), box.getHeight(), box.getDepth(), numCells, true,false,true);
    yClipped.setParent(box);
    
    ofSeedRandom(seed.get());
    unClipped.setupFromBoundingBox(box.getWidth(), box.getHeight(), box.getDepth(), numCells, true,true,true);
    unClipped.setParent(box);
    
    ofSeedRandom(seed.get());
    yClippedCopy.setupFromBoundingBox(box.getWidth(), box.getHeight(), box.getDepth(), numCells, true,false,true);
    yClippedCopy.setParent(box);
    
}

void VoroEnd::drawModel() {
    
    //box.draw();
    //unClipped.draw();
    //yClipped.draw();
    //yClippedCopy.draw();
    
    //of3dPrimitive newBox = box;
    
    ofSetColor(255,75);
    newBox.drawWireframe();
    
}


