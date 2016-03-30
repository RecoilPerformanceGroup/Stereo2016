//
//  LightScene.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/15/16.
//
//

#include "LightScene.hpp"

void LightScene::setup() {

    lightModelMaterial.setShininess(0.8);
    lightModelMaterial.setColors(ofColor::white, ofColor::darkGray, ofColor::white, ofColor::black);
    
    directionalLight.enable();
    directionalLight.setParent(world->origin);
    directionalLight.setDirectional();

    spotLight.enable();
    spotLight.setParent(world->origin);
    spotLight.setSpotlight();
    areaLight.enable();
    areaLight.setParent(world->origin);
    
}

void LightScene::update() {
    updateLight(directionalLight);
    updateLight(spotLight);
    spotLight.setSpotConcentration(spotLightSpotConcentration);
    updateLight(areaLight);
    areaLight.setAreaLight(areaLightWidth, areaLightHeight);
}

void LightScene::draw() {
    //areaLight.enable();
    
}

void LightScene::drawModel(){
    ofPushStyle();
    ofFill();
    ofDisableDepthTest();

    ofPushStyle();
    ofEnableDepthTest();
    ofEnableLighting();
    lightModelMaterial.begin();
    ofDrawSphere(spotLightLookAt, 20);
    lightModelMaterial.end();
    ofPopStyle();
    ofDisableDepthTest();
    ofSetColor(spotLightDiffuseColor,63);
    spotLight.draw();
    spotLight.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,-(spotLight.getGlobalPosition()-spotLightLookAt.get()).length()+5), 5);
    spotLight.restoreTransformGL();
    
    ofPushStyle();
    ofEnableDepthTest();
    ofEnableLighting();
    lightModelMaterial.begin();
    ofDrawSphere(directionalLightLookAt, 20);
    lightModelMaterial.end();
    ofPopStyle();
    ofDisableDepthTest();
    ofSetColor(directionalLightDiffuseColor, 63);
    directionalLight.draw();
    directionalLight.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,(directionalLight.getGlobalPosition()-directionalLightLookAt.get()).length()-5), 5);
    directionalLight.restoreTransformGL();

    ofPushStyle();
    ofEnableDepthTest();
    ofEnableLighting();
    lightModelMaterial.begin();
    ofDrawSphere(areaLightLookAt, 20);
    lightModelMaterial.end();
    ofPopStyle();
    ofDisableDepthTest();
    ofSetColor(areaLightDiffuseColor,63);
    areaLight.draw();
    areaLight.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,-(areaLight.getGlobalPosition()-areaLightLookAt.get()).length()+5), 5);
    areaLight.restoreTransformGL();
    
    ofPopStyle();

}
