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
    ofParameter<ofVec3f> NAME ## Position {"position", ofVec3f(0,-100,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) }; \
    ofParameter<ofVec3f> NAME ## LookAt {"lookAt", ofVec3f(0,0,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) }; \
    ofParameterGroup NAME ## Params{#NAME, \
        NAME ## DiffuseColor, \
        NAME ## Position, \
        NAME ## LookAt, ##__VA_ARGS__ \
    };

#define updateLight(NAME) \
    NAME.setDiffuseColor(ofFloatColor(NAME ## DiffuseColor.get())); \
    NAME.setPosition(NAME ## Position); \
    NAME.lookAt(NAME ## LookAt);


// ofAddListener(guiWindow->events() . NAME , mainApp.get(), &ofApp::NAME ## Gui);


#pragma once
#include "ofMain.h"

#include "ofxStereoscopy.hpp"

class LightScene : public ofxStereoscopy::Scene {
    
public:
    void draw();
    void update();
    void setup();
    
    LightScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    ofParameter<float> testLightSpotConcentration {"spotConcentration", 1.0, 0.0, 100.0};
    ofParameter<ofColor> testLightAmbientColor {"ambientColor", ofColor::white, ofColor::black, ofColor::white};

    makeLight(testLight, testLightSpotConcentration, testLightAmbientColor);
    
    ofLight areaLight;
    
    ofParameter<float> areaLightWidth {"width", 100, 0, 10000};
    ofParameter<float> areaLightHeight {"height", 100, 0, 10000};
    ofParameter<ofColor> areaLightDiffuseColor {"diffuseColor",
        ofColor::white,
        ofColor::black,
        ofColor::white
    };
    ofParameter<ofVec3f> areaLightPos {"position", ofVec3f(0,0,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) };
    ofParameter<ofVec3f> areaLightLookAt {"lookAt", ofVec3f(0,0,0), ofVec3f(-10000,-10000,-10000),ofVec3f(10000,10000,10000) };

    ofParameterGroup areaLightParams{"areaLight",
        areaLightWidth,
        areaLightHeight,
        areaLightPos,
        areaLightLookAt
    };
 
    
    
    ofParameterGroup params {"Lights",
        enabled,
        areaLightParams,
        testLightParams,
    };
    
};

