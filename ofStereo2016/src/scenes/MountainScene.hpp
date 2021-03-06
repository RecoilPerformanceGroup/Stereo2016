//
//  MountainScene.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#ifndef MountainScene_hpp
#define MountainScene_hpp

#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"
#include "OrganicMaterial.hpp"
#include "dispatch/dispatch.h"

class MountainScene : public ofxStereoscopy::Scene {
    
public:
    
    // mountain
    
    VoroNode mountain;
    
    ofParameter<int> mountainRandomSeed {"randomSeed", 0,0,10000 };
    ofParameter<int> mountainCellCount {"cellCount", 2,1,1000 };
    ofParameter<ofVec3f> mountainSize {"size", ofVec3f(1000,1000,1000), ofVec3f(0,0,0), ofVec3f(100000,100000,100000) };
    ofParameter<ofVec3f> mountainPosition {"position", ofVec3f(0,-100000,-50000), ofVec3f(-10000,-10000,-100000), ofVec3f(10000,10000,100000) };
    ofParameter<ofVec3f> mountainRotation {"rotation", ofVec3f(0,0,0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360) };

    ofParameter<ofColor> mountainColor {"color", ofColor::white, ofColor(0,0,0,0), ofColor(255,255,255,255) };
    
    ofParameter<ofVec3f> selectPosition {"selectPosition", ofVec3f(0,-100000,-50000), ofVec3f(-10000,-10000,-100000), ofVec3f(10000,10000,100000) };
    ofParameter<float> selectRadius {"selectRadius", 2,0,2000};
    
    ofParameter<float> mountainSplit {"split", 2,0,2000};
    
    ofParameterGroup mountainParams {"mountain",
        mountainRandomSeed,
        mountainCellCount,
        mountainSize,
        mountainPosition,
        mountainRotation,
        mountainColor,
        selectRadius,
        selectPosition,
        mountainSplit
    };
    
    // scene
    
    ofParameter<ofColor> anaglyphColorLeft {"anaglyphColorLeft", ofColor::cyan, ofColor::black, ofColor::white };
    ofParameter<ofColor> anaglyphColorRight {"anaglyphColorRight", ofColor::red, ofColor::black, ofColor::white };
    
    ofParameter<float> anaglyphAmount {"anaglyphAmount", 0.0, 0.0, 1.0 };
    
    ofParameterGroup params {"mountainScene",
        enabled,
        qlab,
        anaglyphAmount,
        anaglyphColorLeft,
        anaglyphColorRight
    };
    
    MountainScene() {
        mountainParams.add(matMountain.params);
        params.add(mountainParams);
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    template<typename ParameterType>
    void reconstructMountain( ParameterType & p){
        reconstructMountain();
    }
    void reconstructMountain();
    
    ofxPanel panel;
    
    OrganicMaterial matMountain;
    OrganicMaterial matAnaglyphRight;
    OrganicMaterial matAnaglyphLeft;
    
};

#endif /* MountainScene_hpp */
