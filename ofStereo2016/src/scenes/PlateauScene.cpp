//
//  PlateauScene.cpp
//  ofStereo2016
//
//

#include "PlateauScene.hpp"

void PlateauScene::setup(){
    
    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");
    
    ofSetSmoothLighting(true);
    
    globalParams->getVec3f("stage_size_cm").addListener(this, &PlateauScene::onStageSize);
    
    plateauRandomSeed.addListener(this, &PlateauScene::reconstructPlateau<int>);
    plateauCellCount.addListener(this, &PlateauScene::reconstructPlateau<int>);
    plateauSize.addListener(this, &PlateauScene::reconstructPlateau<ofVec3f>);
    /*
    float floorHeight = 10.0;
    float floorSize = WORLD_DIMENSION_MAX*2;
    
    floor.set(floorSize, floorHeight, floorSize, 2, 2, 2);
    floor.setParent(world->origin);
    floor.setGlobalPosition(0,-floorHeight/2.0, stage_size->z/2.0);
    
    matFloor.noiseDisplacementVelocity = ofVec3f(0.1,0.0,0.25);
    matPlateau.noiseDisplacementVelocity = ofVec3f(0.1,0.0,0.25);
    matFloor.setShininess(0.2);
    matFloor.setSpecularColor(ofColor::white);
    */
    reconstructPlateau();
}

void PlateauScene::update(){
    //matFloor.updateParameters();
    matPlateau.updateParameters();
    
    ofVec3f floorAlignedPlateauPosition(plateauPosition);
    floorAlignedPlateauPosition.y += -plateauSize->y / 2.0;

//    plateauPivotOrigin.set(ofVec3f(ofSignedNoise(ofGetElapsedTimef())*100.0, plateauPivotOrigin->y, ofSignedNoise(ofGetElapsedTimef()+0.33235)*100.0));
    
    ofVec3f plateauPivotOriginTranslated = plateauPivotOrigin.get() + plateauPosition.get();

    noiseTime += ofGetLastFrameTime() * plateauPivotNoiseSpeed;
    
    pivotNoise.set(ofSignedNoise(noiseTime+0.33253)*plateauPivotNoiseAmount, 0, ofSignedNoise(noiseTime+44.0523)*plateauPivotNoiseAmount);
    
    ofQuaternion q;
    q.makeRotate(
                 ofVec3f(plateauPosition.get() + ofVec3f(plateauPivotOrigin->x, 0, plateauPivotOrigin->z) + pivotNoise) - plateauPivotOriginTranslated,
                 dp(1) - plateauPivotOriginTranslated
                 );
    pivotMatrix.makeIdentityMatrix();
    pivotMatrix.translate(floorAlignedPlateauPosition);
    pivotMatrix.translate(-dp(1));
    pivotMatrix.rotate(q);
    pivotMatrix.translate(dp(1));
    plateau.setTransformMatrix(pivotMatrix);
//    plateau.pan(plateauRotation->y);
}

void PlateauScene::draw(){
    
    ofFloatColor eyeColor;
    
    if(world->bIsDrawingLeft)
        eyeColor = anaglyphColorLeft;
    else
        eyeColor = anaglyphColorRight;
    
    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");
    
    ofSetColor(1.0,1.0);
    
    ofEnableLighting();
    
    // compensate for nonlinear transition to eyecolor
    ofFloatColor plateauAnaColor = plateauColor->getLerped(ofFloatColor::black, 0.1*(1.0-anaglyphAmount));
    // transition to eyeColor
    plateauAnaColor = plateauAnaColor.getLerped(eyeColor , anaglyphAmount);
    
    matPlateau.begin();
    matPlateau.setAmbientColor(plateauAnaColor.getLerped(ofFloatColor::black, 0.9))  ;
    matPlateau.setWorldMatrix(plateau.getGlobalTransformMatrix());
    //floor.drawFaces();
    matPlateau.setDiffuseColor(plateauAnaColor);
    
    ofMatrix4x4 m;
    /*
    m.translate(<#float tx#>, <#float ty#>, <#float tz#>)
    
    ofPushMatrix();
    
    ofPoint anchor = plateau.getPosition() + ofVec3f(0, plateau.getHeight()/2, 0) + pivotOffset;
    
    ofDrawSphere(anchor + pivotOffset, 10);
    
    ofTranslate(anchor + pivotOffset);
    
    ofRotate(ofMap(pivotOffset.get().x, 100, -100, -45, 45), 0, 0, 1);
    ofRotate(ofMap(pivotOffset.get().z, -100, 100, -45, 45), 1, 0, 0);
    
    ofTranslate(-(anchor + pivotOffset));
    
    m = ofGetCurrentViewMatrix();
    
    ofPopMatrix();

    plateau.setTransformMatrix(m);
    */
     plateau.draw(&matPlateau);

    
    
    
    
    // split in a way where we get a new node hirearchy, doesnt work with shader stuff otherwise
    /*for( auto m : Plateau.getChildrenInSphere(selectPosition.get() - ofVec3f(selectRadius, 0, 0), selectRadius)) {
     
     ofPushMatrix();
     
     ofTranslate(-PlateauSplit, 0, 0);
     m->draw();
     
     ofPopMatrix();
     }
     
     
     for( auto m : Plateau.getChildrenInSphere(selectPosition.get() + ofVec3f(selectRadius, 0, 0), selectRadius)) {
     
     ofPushMatrix();
     
     ofTranslate(PlateauSplit, 0, 0);
     m->draw();
     
     ofPopMatrix();
     }*/
    
    matPlateau.end();
}

void PlateauScene::reconstructPlateau(){
    ofSeedRandom(plateauRandomSeed);
    
    plateau.setupFromBoundingBox(plateauSize->x, plateauSize->y, plateauSize->z, plateauCellCount, true,false,true);
    
    plateau.setParent(world->origin);

    ofSeedRandom(plateauRandomSeed);

    plateauPivotOrigin.set(ofVec3f(0,-globalParams->getVec3f("stage_size_cm")->z, globalParams->getVec3f("stage_size_cm")->z/2.0));

}

void PlateauScene::onStageSize(ofVec3f& stageSize){
    //reconstructPlateau();
}

void PlateauScene::drawModel(){
    ofSetColor(255,255,0,16);
    plateau.draw();
    ofSetColor(255,255,0,255);
    ofDrawSphere(plateau.getGlobalPosition(), 5);

    ofVec3f plateauPivotOriginTranslated = plateauPivotOrigin.get() + plateauPosition.get();

    ofSetColor(127,0,127,255);
    ofDrawSphere(plateauPivotOriginTranslated, 20);
    ofDrawArrow(plateauPivotOriginTranslated, plateauPivotOriginTranslated+pivotNoise, 5);
    ofSetColor(255,0,255,255);
    ofDrawSphere(plateauPivotOriginTranslated+pivotNoise, 20);
    ofDrawArrow(plateauPivotOriginTranslated+pivotNoise, dp(1), 5);

    ofPushMatrix();
    ofMultMatrix(pivotMatrix);
    ofDrawAxis(20);
    ofPopMatrix();
  
    
    // floor.drawWireframe();
}