//
//  PerspectiveChange.hpp
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

class PerspectiveChange : public ofxStereoscopy::Scene {
    
public:
    
    // push down position = dancer position
    
    // select nearest 3
    
    // float push down amount
    
    ofParameter<ofVec3f> cylpos {"pos", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    
    ofParameter<int> perspectiveSeed {"perspective seed", 0, 0, 100000};
    ofParameter<int> frameInterval {"frameInterval", 1, 1, 1000};
    
    ofParameterGroup params {"PerspectiveChange",
        enabled,
        qlab,
        cylpos,
        frameInterval,
        perspectiveSeed
    };
    
    PerspectiveChange() {
        //voroNodeParams.add(mat.params);
        
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    
    ofNode floorCenter, wallCenter;
    
    ofCylinderPrimitive cyl1;
    ofCylinderPrimitive cyl2;
    ofCylinderPrimitive cyl3;
    ofCylinderPrimitive cyl4;
    
    //ofMesh intersect;
    //ofMesh outMesh;
    ofVec3f clusterRotation;
    
    /*OrganicMaterial matRightLeft;
     OrganicMaterial matRightRight;
     
     OrganicMaterial matLeftLeft;
     OrganicMaterial matLeftRight;*/
    
};



