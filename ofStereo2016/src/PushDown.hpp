//
//  PushDown.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 4/12/16.
//
//


#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "OrganicMaterial.hpp"
#include "ofxEasing.h"

class PushDown : public ofxStereoscopy::Scene {
    
public:
    
    // push down position = dancer position
    
    // select nearest 3
    
    // float push down amount
    
    ofParameter<float> pushDown {"push", 2, 0, -800};

    ofParameter<float> pushRadius {"pushradius", 2, 0, 350};

    ofParameter<ofVec3f> pushPos {"pushpos", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    ofParameter<float> pullUp {"pull", 2, 0, 800};
    
    ofParameter<float> pullRadius {"pullradius", 2, 0, 350};
    
    ofParameter<ofVec3f> pullPos {"pullpos", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    
    // sequentially
    ofParameter<int> numCells {"cells", 2, 0, 200};
    ofParameter<int> seed {"seed", 2, 0, 200};
    
    ofParameter<ofVec3f> origin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    ofParameter<ofFloatColor> clusterColor {"color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameterGroup voroNodeParams {"cluster",
        numCells,
        seed,
        origin,
        clusterColor,
        pushDown,
        pushRadius,
        pushPos,
        pullUp,
        pullRadius,
        pullPos
    };
    
    ofParameterGroup params {"PushDown",
        enabled,
        qlab
    };
    
    PushDown() {
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
        doReconstruct = true;
    }
    void reconstruct();
    bool doReconstruct = false;
    
    ofBoxPrimitive box;
    
    VoroNode mainNode;
    
    //ofMesh intersect;
    //ofMesh outMesh;
    ofVec3f clusterRotation;
    
    OrganicMaterial mat;
    
    
};



