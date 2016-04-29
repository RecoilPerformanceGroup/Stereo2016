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
#include "VoroEnd.hpp"

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
    ofParameter<float> clusterBackToHomeForce {"clusterBackToHomeForce", 0, 0, 1.0};
    ofParameterGroup clusterParams {"cluster",
        clusterNumCells,
        clusterOrigin,
        clusterScale,
        clusterScaleCells,
        clusterRotationAxis,
        clusterRotationSpeed,
        clusterBackToHomeForce,
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
    ofParameter<float> soundWaveAffectDistance {"affectDistance", 500, 0, 10000 };
    ofParameter<float> soundWaveGlobalForce {"globalForce", 1, -100, 100 };

    
    ofParameter<float> fadeIn {"fadeIn", 0, 0, 1};

    ofParameter<bool> transitionOn {"transitionOn", false};
    
    
    ofParameterGroup soundWaveParams {"soundWaves",
        soundWaveOriginNormalised,
        soundWaveType,
        soundWaveDirection,
        soundWaveVelocity,
        soundWaveForce,
        soundWaveCreate,
        soundWaveClear,
        soundWaveAffectDistance,
        soundWaveGlobalForce
    };
    
    ofParameterGroup params {"SoundField",
        enabled,
        enabledDraw,
        enabledDrawModel,
        qlab,
        transitionOn,
        fadeIn,
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
    
    vector<VoroNode *> fadeInFromNodes;

    
    void enable() {
        
        // on scene enable
        
        /*for(auto c : voroEnd->wall.getChildren()) {
            // + c->renderPosOffset) * c->getGlobalTransformMatrix()
            c->move(c->renderPosOffset);
            c->bOffsetPositionOveride = true;
            c->positionOverideOrigin = c->getGlobalPosition();
        }*/
        
        /*for(auto c : voroEnd->oceanHorizon.getChildren()) {
            // + c->renderPosOffset) * c->getGlobalTransformMatrix()
            c->move(c->renderPosOffset);
            c->bOffsetPositionOveride = true;
            c->positionOverideOrigin = c->getGlobalPosition();
        }*/
        
        
    }
    
    void applyWaves(VoroNode & vn, bool recursive = false);
    
    shared_ptr<VoroEnd> voroEnd;
    
    void resetTransitionReferences () {
        
        for( auto c : cluster.getChildren() ) {
            c->transitionRef = nullptr;
        }
        
    }
    
    bool doReconstruct = false;
    
    vector<ofVec3f> originalPositions;
};



