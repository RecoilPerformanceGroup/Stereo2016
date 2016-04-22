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

    ofParameter<ofVec3f> dancer1Position {"one", ofVec3f(-0.2,0.0,0.5), ofVec3f(-1.0,0.0,0.0), ofVec3f(1.0,1.0,1.0) };
    ofParameter<ofVec3f> dancer2Position {"two", ofVec3f(0.2,0.0,0.5), ofVec3f(-1.0,0.0,0.0), ofVec3f(1.0,1.0,1.0) };
    
    ofParameterGroup dancerParams {"dancers",
        dancer1Position,
        dancer2Position
    };
    
    ofParameterGroup params {"roomScene",
        enabled,
        enabledDraw,
        enabledDrawModel,
        qlab
        dancerParams
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
    
    ofVec3f getDancerPositionInWorldCoordinates(int number);
    ofVec3f getDancerPositionNormalised(int number);
    
    ofBoxPrimitive floor;
    ofBoxPrimitive wall;
    
    OrganicMaterial matFloor;
    OrganicMaterial matWall;

    ofLight floorLight;
    ofLight wallLight;
    
    ofParameterGroup & getParamGroupInGroup(ofParameterGroup g, string n);
    
    float floorHeight;
    float wallThickness;
    float resolution;
    
};

