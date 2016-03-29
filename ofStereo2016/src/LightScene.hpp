//
//  LightScene.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/15/16.
//
//



#define makeLight(NAME, ...) \
    ofLight NAME; \
    ofParameter<ofColor> NAME ## DiffuseColor{"diffuseColor", ofColor::white, ofColor::black, ofColor::white}; \
    ofParameter<ofColor> NAME ## AmbientColor{"ambientColor", ofColor::black, ofColor::black, ofColor::white}; \
    ofParameter<ofColor> NAME ## SpecularColor{"specularColor", ofColor::white, ofColor::black, ofColor::white}; \
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
    
    LightScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    vector<ofLight*> lights;

    ofParameter<float> spotLightSpotConcentration {"spotConcentration", 1.0, 0.0, 100.0};

    makeLight(spotLight, spotLightSpotConcentration);
    
    ofParameter<float> areaLightWidth {"width", 100, 0, 10000};
    ofParameter<float> areaLightHeight {"height", 100, 0, 10000};

    makeLight(areaLight, areaLightWidth, areaLightHeight);
    
    makeLight(directionalLight);
    
    ofParameterGroup params {"Lights",
        enabled,
        qlab,
        directionalLightParams,
        spotLightParams,
        areaLightParams
    };
    
    ofMaterial lightModelMaterial;
    
};

