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
    ofParameter<ofColor> NAME ## specularColor{"specularColor", ofColor::white, ofColor::black, ofColor::white}; \
    ofParameter<ofVec3f> NAME ## Position {"position", ofVec3f(0,-100,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) }; \
    ofParameter<ofVec3f> NAME ## LookAt {"lookAt", ofVec3f(0,0,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) }; \
    ofParameterGroup NAME ## Params{#NAME, \
        NAME ## DiffuseColor, \
        NAME ## AmbientColor, \
        NAME ## specularColor, \
        NAME ## Position, \
        NAME ## LookAt, ##__VA_ARGS__ \
    };

#define updateLight(NAME) \
    NAME.setDiffuseColor(ofFloatColor(NAME ## DiffuseColor.get())); \
    NAME.setGlobalPosition(NAME ## Position); \
    NAME.lookAt(-(NAME ## LookAt.get()));


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
    
    ofParameter<float> spotLightSpotConcentration {"spotConcentration", 1.0, 0.0, 100.0};

    makeLight(spotLight, spotLightSpotConcentration);
    
    ofParameter<float> areaLightWidth {"width", 100, 0, 10000};
    ofParameter<float> areaLightHeight {"height", 100, 0, 10000};

    makeLight(areaLight, areaLightWidth, areaLightHeight);
    
    makeLight(directionalLight);
    
    ofParameterGroup params {"Lights",
        enabled,
        directionalLightParams,
        spotLightParams,
        areaLightParams
    };
    
};

