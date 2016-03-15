//
//  LightScene.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/15/16.
//
//

#pragma once
#include "ofMain.h"

#include "ofxStereoscopy.hpp"

class LightScene : public ofxStereoscopy::Scene {
    
public:
    void draw();
    void update();
    void setup();
    
    LightScene() {
        name = "Lights";
        ofxStereoscopy::Scene::params = params;
    }
    
    ofLight areaLight;
    
    ofParameter<float> areaLightWidth {"width", 100, 0, 200};
    ofParameter<float> areaLightHeight {"height", 100, 0, 200};
    
    ofParameter<ofVec3f> areaLightPos {"position", ofVec3f(0,0,0), ofVec3f(-1000,-1000,-1000),ofVec3f(1000,1000,1000) };

    
    ofParameterGroup params {"Lights",
        enabled,
        areaLightWidth,
        areaLightHeight,
        areaLightPos};
    
};

