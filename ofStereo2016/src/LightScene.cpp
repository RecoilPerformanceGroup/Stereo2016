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
    

    
}

void LightScene::update() {

    areaLight.setAreaLight(areaLightWidth, areaLightHeight);
    
    areaLight.setPosition(areaLightPos);
    
    
        areaLight.lookAt(ofVec3f(0,0,0));
    
}

void LightScene::draw() {
    //areaLight.enable();
    
    
}

