//
//  SoundField.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/3/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"
#include "OrganicMaterial.hpp"
#include "ofxCSG.h"

class SoundField : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<int> clusterNumCells {"cells", 2, 0, 2000};
    ofParameter<ofVec3f> clusterOrigin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    ofParameter<ofVec3f> clusterRotationAxis {"rotationAxis", ofVec3f(0,0,0),
        ofVec3f(-1,-1,-1),
        ofVec3f(1,1,1)};
    ofParameter<float> clusterRotationSpeed {"rotationSpeed", 0, -1, 1};
    
    ofParameter<ofFloatColor> clusterColor {"color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameter<float> clusterScale {"scale", 0, 0, 10};
    ofParameter<float> clusterScaleCells {"scaleCells", 0,0,0.999};
    
    ofParameterGroup clusterParams {"cluster",
        clusterNumCells,
        clusterOrigin,
        clusterScale,
        clusterScaleCells,
        clusterRotationAxis,
        clusterRotationSpeed,
        clusterColor
    };
    
    ofParameter<ofVec3f> soundWaveOriginNormalised {"originNormalised", ofVec3f(0,0,0),
        ofVec3f(-2,-2, -2),
        ofVec3f(2,2,2)};
    ofParameter<ofVec3f> soundWaveDirection {"direction", ofVec3f(0,0,0),
        ofVec3f(-1,-1,-1),
        ofVec3f(1,1,1)};
    ofParameter<float> soundWaveVelocity {"velocity", 10, -100, 100};
    ofParameter<int> soundWaveType {"type", 0, 0, 1 };
    ofParameter<float> soundWaveForce {"force", 1, -1, 1 };
    ofParameter<bool> soundWaveCreate {"create", false };
    ofParameter<bool> soundWaveClear {"clear", false };

    ofParameterGroup soundWaveParams {"soundWaves",
        soundWaveOriginNormalised,
        soundWaveType,
        soundWaveDirection,
        soundWaveVelocity,
        soundWaveForce,
        soundWaveCreate,
        soundWaveClear
    };
    
    ofParameterGroup params {"SoundField",
        enabled,
        qlab,
        soundWaveParams
    };
    
    SoundField() {
        clusterParams.add(matCluster.params);
        params.add(clusterParams);
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    
    void reconstructCluster();
    
    template<typename ParameterType>
    void reconstructCluster( ParameterType & p){
        reconstructCluster();
    }
    
    VoroNode cluster;
    ofVec3f clusterRotation;
    OrganicMaterial matCluster;
    
    struct soundWave {
        int type;
        ofVec3f origin;
        ofVec3f velocity;
        ofVec3f position;
        float force;
    };
    
    vector<soundWave> soundWaves;
};



