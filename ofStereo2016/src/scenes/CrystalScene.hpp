//
//  CrystalScene.hpp
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

class CrystalScene : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<int> clusterNumCells {"cells", 2, 0, 200};
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

    ofParameter<int> crystalSeed {"seed", 2, 0, 255};
    ofParameter<float> crystalSize {"size", 250, 0, 1000};
    ofParameter<ofVec3f> crystalOrigin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    ofParameter<ofFloatColor> crystalColor {"color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    ofParameterGroup crystalParams {"crystal",
        crystalSeed,
        crystalSize,
        crystalOrigin,
        crystalColor
    };
    
    ofParameterGroup params {"Crystal",
        enabled,
        qlab
    };
    
    CrystalScene() {
        clusterParams.add(matCluster.params);
        crystalParams.add(matCrystal.params);
        params.add(clusterParams);
        params.add(crystalParams);
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawGui();
    void update();
    void setup();
    
    void reconstructCrystal();
    
    template<typename ParameterType>
    void reconstructCrystal( ParameterType & p){
        reconstructCrystal();
    }

    void reconstructCluster();

    template<typename ParameterType>
    void reconstructCluster( ParameterType & p){
        reconstructCluster();
    }
    
    VoroNode cluster;
    VoroNode crystal;
    VoroNode * crystalBoulder;
    
    ofVec3f rotation;
    
    OrganicMaterial matCrystal;
    OrganicMaterial matCluster;
};



