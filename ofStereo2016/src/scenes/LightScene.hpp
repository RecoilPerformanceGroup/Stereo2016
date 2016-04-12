//
//  LightScene.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/15/16.
//
//



#define makeLight(NAME, ...) \
    ofLight NAME; \
    ofParameter<ofFloatColor> NAME ## DiffuseColor{"diffuseColor", ofFloatColor::white, ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)}; \
    ofParameter<ofFloatColor> NAME ## AmbientColor{"ambientColor", ofFloatColor::black, ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)}; \
    ofParameter<ofFloatColor> NAME ## SpecularColor{"specularColor", ofFloatColor::white, ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)}; \
    ofParameter<ofVec3f> NAME ## Position {"position", ofVec3f(0,700,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) }; \
    ofParameter<ofVec3f> NAME ## LookAt {"lookAt", ofVec3f(0,0,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) }; \
    ofParameterGroup NAME ## Params{#NAME, \
        NAME ## DiffuseColor, \
        NAME ## AmbientColor, \
        NAME ## SpecularColor, \
        NAME ## Position, \
        NAME ## LookAt, ##__VA_ARGS__ \
    }; \

#define updateLight(NAME) \
    NAME.setDiffuseColor(ofFloatColor(NAME ## DiffuseColor.get())); \
    NAME.setAmbientColor(ofFloatColor(NAME ## AmbientColor.get())); \
    NAME.setSpecularColor(ofFloatColor(NAME ## SpecularColor.get()));\
    NAME.setGlobalPosition(NAME ## Position); \
    if(NAME.getIsSpotlight() || NAME.getIsAreaLight()) \
        NAME.lookAt((NAME ## LookAt.get()), ofVec3f(0.0,1.0,0.0)); \
    if(NAME.getIsDirectional()) \
        NAME.lookAt(-(NAME ## LookAt.get()-(2.0*NAME.getGlobalPosition())), ofVec3f(0.0,1.0,0.0));



// ofAddListener(guiWindow->events() . NAME , mainApp.get(), &ofApp::NAME ## Gui);


#pragma once
#include "ofMain.h"

#include "ofxStereoscopy.hpp"

class LightScene : public ofxStereoscopy::Scene {
    
public:
    void draw();
    void drawModel();
    void update();
    void setup();
    
    void enable() {
        spotLight.enable();
        areaLight.enable();
        directionalLight1.enable();
        directionalLight2.enable();
    }
    
    void disable() {
        spotLight.disable();
        areaLight.disable();
        directionalLight1.disable();
        directionalLight2.disable();
    }
    
    LightScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    ofParameter<float> spotLightSpotConcentration {"spotConcentration", 1.0, 0.0, 100.0};

    makeLight(spotLight, spotLightSpotConcentration);
    
    ofParameter<float> areaLightWidth {"width", 100, 0, 10000};
    ofParameter<float> areaLightHeight {"height", 100, 0, 10000};

    makeLight(areaLight, areaLightWidth, areaLightHeight);
    
    makeLight(directionalLight1);
    makeLight(directionalLight2);
    
    ofParameterGroup params {"Lights",
        enabled,
        qlab,
        directionalLight1Params,
        directionalLight2Params,
        spotLightParams,
        areaLightParams
    };
    
    ofMaterial lightModelMaterial;
    
};

