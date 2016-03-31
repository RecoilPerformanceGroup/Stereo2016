//
//  RoomScene.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 25/02/2016.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "OrganicMaterial.hpp"

class RoomScene : public ofxStereoscopy::Scene {
    
public:
    
    
    ofParameter<ofColor> floorColor {"color", ofColor(0,0,0,0), ofColor(0,0,0,0), ofColor(255,255,255,255) };
    ofParameter<ofColor> floorLightColor {"light", ofColor(255,255,255), ofColor(0,0,0), ofColor(255,255,255) };
    
    ofParameterGroup floorParams {"floor",
        floorColor,
        floorLightColor
    };
    
    ofParameter<ofColor> wallColor {"color", ofColor(0,0,0,0), ofColor(0,0,0,0), ofColor(255,255,255,255) };
    ofParameter<ofColor> wallLightColor {"light", ofColor(255,255,255), ofColor(0,0,0), ofColor(255,255,255) };
    
    ofParameterGroup wallParams {"wall",
        wallColor,
        wallLightColor
    };
    
    ofParameterGroup params {"roomScene",
        enabled,
        qlab
    };
    
    RoomScene() {
        floorParams.add(matFloor.params);
        params.add(floorParams);
        wallParams.add(matWall.params);
        params.add(wallParams);
        ofxStereoscopy::Scene::params = params;
    }

    void draw();
    void update();
    void setup();
    void drawModel();
    void onStageSize(ofVec3f& vec);
    
    ofBoxPrimitive floor;
    ofBoxPrimitive wall;
    
    OrganicMaterial matFloor;
    OrganicMaterial matWall;

    ofLight floorLight;
    ofLight wallLight;
    
    float floorHeight;
    float wallThickness;
    float resolution;
    
};

