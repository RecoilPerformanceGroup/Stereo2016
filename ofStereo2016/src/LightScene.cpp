//
//  LightScene.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/15/16.
//
//

#include "LightScene.hpp"

void LightScene::setup() {

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
    ofSetColor(255,127);
    spotLight.draw();
    directionalLight.draw();
    areaLight.draw();
}
