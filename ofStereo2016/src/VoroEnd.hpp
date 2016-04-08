//
//  VoroEnd.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/8/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"
#include "OrganicMaterial.hpp"
#include "ofxCSG.h"

class VoroEnd : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<int> numCells {"cells", 2, 0, 200};
    ofParameter<int> seed {"seed", 2, 0, 200};
    
    ofParameter<ofVec3f> origin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    //ofParameter<ofVec3f> size {"size", ofVec3f(1000,1000,1000), ofVec3f(0,0,0), ofVec3f(10000,10000,10000) };
    
    ofParameter<ofFloatColor> clusterColor {"color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameter<float> scale {"scale", 0, 0, 10};
    ofParameter<float> scaleCells {"scaleCells", 0,0,0.999};
    
    ofParameter<ofVec3f> subtractOffset {"subtractOffset", ofVec3f(0,0,0),
        ofVec3f(-100,-100,-100),
        ofVec3f(100,100,100)};
    
    
    
    ofParameterGroup voroNodeParams {"cluster",
        numCells,
        seed,
        origin,
        subtractOffset,
        scale,
        scaleCells,
        clusterColor,
    };
    
    ofParameterGroup params {"VoroEnd",
        enabled,
        qlab
    };
    
    VoroEnd() {
        voroNodeParams.add(mat.params);
        params.add(voroNodeParams);
        
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    
    template<typename type>
    void reconstruct(type & t) {
        reconstruct();
    }
    
    void reconstruct();
    
    ofBoxPrimitive box;
    
    
    VoroNode unClipped;
    VoroNode yClipped;
    VoroNode yClippedCopy;
    
    //ofMesh intersect;
    //ofMesh outMesh;
    ofVec3f clusterRotation;
    
    OrganicMaterial mat;
    
    
    ofMesh newBox;
    
    
};

