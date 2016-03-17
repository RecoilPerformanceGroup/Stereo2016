//
//  LightScene.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/15/16.
//
//

#include "LightScene.hpp"

void LightScene::setup() {

    areaLight.enable();
    testLight.enable();
    testLight.setSpotlight();
}

void LightScene::update() {
    updateLight(testLight);
    testLight.setSpotConcentration(testLightSpotConcentration);
    testLight.setAmbientColor(testLightAmbientColor.get());
    areaLight.setAreaLight(areaLightWidth, areaLightHeight);
    areaLight.setPosition(areaLightPos);
    areaLight.lookAt(areaLightLookAt);
}

void LightScene::draw() {
    //areaLight.enable();
    
}

