//
//  VoroScenes.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/7/16.
//
//


#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "OrganicMaterial.hpp"

class VoroScenes : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<int> voroNumCells {"cells", 2, 0, 200};
    ofParameter<int> voroSeed {"seed", 2, 0, 200};
    ofParameter<ofFloatColor> voroColor {"color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameterGroup voroParams {"nodes",
        voroNumCells,
        voroSeed,
        voroColor,
    };

    ofParameter<float> animationsWallDown {"wallDown", 0, 0, 1};
    
    ofParameterGroup animations { "animations",
        animationsWallDown
    };
    
    ofParameterGroup params {"VoroScenes",
        enabled,
        qlab,
        animations
    };
    
    VoroScenes() {
        voroParams.add(voroMat.params);
        params.add(voroParams);
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    
    template<typename type>
    void reconstruct(type & t) {
        doReconstruct = true;
    }
    void reconstruct();
    bool doReconstruct = false;
    
    ofBoxPrimitive floorBox;
    ofBoxPrimitive wallBox;
    
    VoroNode floorNode;
    VoroNode wallNode;

    vector<ofNode> wallNodeOriginal;
    
    //ofMesh intersect;
    //ofMesh outMesh;
    ofVec3f clusterRotation;
    
    OrganicMaterial voroMat;
    
    
};



