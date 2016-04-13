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
    
    directionalLight1.enable();
    directionalLight1.setParent(world->origin);
    directionalLight1.setDirectional();

    directionalLight2.enable();
    directionalLight2.setParent(world->origin);
    directionalLight2.setDirectional();

    spotLight.enable();
    spotLight.setParent(world->origin);
    spotLight.setSpotlight();
    areaLight.enable();
    areaLight.setParent(world->origin);
    
}

void LightScene::update() {
    updateLight(directionalLight1);
    updateLight(directionalLight2);
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
    ofSetColor(spotLightDiffuseColor.get(),63);
    spotLight.draw();
    spotLight.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,-(spotLight.getGlobalPosition()-spotLightLookAt.get()).length()+5), 5);
    spotLight.restoreTransformGL();
    
    ofPushStyle();
    ofEnableDepthTest();
    ofEnableLighting();
    lightModelMaterial.begin();
    ofDrawSphere(directionalLight1LookAt, 20);
    lightModelMaterial.end();
    ofPopStyle();
    ofDisableDepthTest();
    ofSetColor(directionalLight1DiffuseColor.get(), 63);
    directionalLight1.draw();
    directionalLight1.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,(directionalLight1.getGlobalPosition()-directionalLight1LookAt.get()).length()-5), 5);
    directionalLight1.restoreTransformGL();
    
    ofPushStyle();
    ofEnableDepthTest();
    ofEnableLighting();
    lightModelMaterial.begin();
    ofDrawSphere(directionalLight2LookAt, 20);
    lightModelMaterial.end();
    ofPopStyle();
    ofDisableDepthTest();
    ofSetColor(directionalLight2DiffuseColor.get(), 63);
    directionalLight2.draw();
    directionalLight2.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,(directionalLight2.getGlobalPosition()-directionalLight2LookAt.get()).length()-5), 5);
    directionalLight2.restoreTransformGL();
    
    ofPushStyle();
    ofEnableDepthTest();
    ofEnableLighting();
    lightModelMaterial.begin();
    ofDrawSphere(areaLightLookAt, 20);
    lightModelMaterial.end();
    ofPopStyle();
    ofDisableDepthTest();
    ofSetColor(areaLightDiffuseColor.get(),63);
    areaLight.draw();
    areaLight.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,-(areaLight.getGlobalPosition()-areaLightLookAt.get()).length()+5), 5);
    areaLight.restoreTransformGL();
    
    ofPopStyle();

}
