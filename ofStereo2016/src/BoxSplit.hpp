//
//  BoxSplit.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/14/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "OrganicMaterial.hpp"
#include "ofxEasing.h"

class BoxSplit : public ofxStereoscopy::Scene {
    
public:
    
    // push down position = dancer position
    
    // select nearest 3
    
    // float push down amount
    
    
    
    ofParameter<ofVec3f> origin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    
    ofParameter<float> split {"split", 0, 0, 400};
    ofParameter<float> boxwidth {"width", 0, 0, 2};
    ofParameter<float> downAnimation {"downAnimation", 0, 0, 1};
    
    ofParameter<bool> floorOn {"floorOn", true};
    ofParameter<bool> wallOn {"wallOn", true};

    
    ofParameter<ofFloatColor> leftRightColor {"left right eye", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameter<ofFloatColor> leftLeftColor {"left left eye", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameter<ofFloatColor> rightRightColor {"right right eye", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameter<ofFloatColor> rightLeftColor {"right left eye", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameter<ofFloatColor> dotColor {"dotColor", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};

    ofParameterGroup boxp {"box",
        split,
        boxwidth,
        leftLeftColor,
        leftRightColor,
        rightLeftColor,
        rightRightColor,
        dotColor,
        downAnimation
    };
    
    ofParameterGroup params {"boxsplit",
        enabled,
        qlab,
        floorOn,
        wallOn
    };
    
    BoxSplit() {
        //voroNodeParams.add(mat.params);
        params.add(boxp);
        
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    void reconstruct();
    
    ofNode floorCenter, wallCenter;
    
    ofBoxPrimitive boxLeftFloor;
    ofBoxPrimitive boxRightFloor;
    
    ofBoxPrimitive boxLeftWall;
    ofBoxPrimitive boxRightWall;
    
    
    //ofMesh intersect;
    //ofMesh outMesh;
    ofVec3f clusterRotation;
    
    /*OrganicMaterial matRightLeft;
    OrganicMaterial matRightRight;
    
    OrganicMaterial matLeftLeft;
    OrganicMaterial matLeftRight;*/
    
    
    
};



