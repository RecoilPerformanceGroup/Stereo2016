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

class VoroEnd : public ofxStereoscopy::Scene {
    
public:
    
    // perlin displacement along depth axis amount
    ofParameter<float> oceanNoiseDisplaceSpeed {"noise speed", 0, 0, 1};
    ofParameter<float> oceanNoiseDisplaceAmount {"noise amount", 0, 0, 1000};
    
    // used when next scene takes over
    ofParameter<bool> applyNoise {"applyNoise", 1, 0, 2};
    
    float oceanNoiseTime = 0;
    float wallNoiseTime = 0;
    
    ofParameter<int> oceanNumCells {"cells", 2, 0, 200};
    ofParameter<int> oceanSeed {"seed", 2, 0, 200};
    
    ofParameter<float> minDarkFade {"minDarkFade", 200, 0, 1000};
    
    ofParameter<float> fadeOutEdge {"fadeOutEdge", 0, 0, 1};
    
    ofParameter<float> oceanFall {"fall", 0, 0, -1000};
    
    ofParameter<ofVec3f> oceanOrigin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    ofParameter<ofVec3f> wallSize {"size", ofVec3f(1,1,1),
        ofVec3f(0,0,0),
        ofVec3f(4,4,4)};
    
    
    ofParameter<float> wallNoiseDisplaceSpeed {"noise speed", 0, 0, 1};
    ofParameter<float> wallNoiseDisplaceAmount {"noise amount", 0, 0, 1000};
    
    ofParameter<ofVec3f> wallOrigin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    ofParameter<int> wallNumCells {"cells", 2, 0, 200};
    ofParameter<int> wallSeed {"seed", 2, 0, 200};

    ofParameter<float> openWall {"open", 0, 0, 90};
    ofParameter<float> fallWall {"fall", 0, 0, 1};
    
    ofParameter<ofVec3f> horizonOrigin {"horizonOrigin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    ofParameter<float> dpPersist {"dpPersist", 0, 0, 1};
    
    ofParameter<float> dpPersistRadius {"dpPersistRadius", 0, 0, 1000};
    
    ofParameter<float> stillAtEdge {"stillAtEdge", 0, 0, 1};
    ofParameter<float> stillAtEdgeDist {"stillAtEdgeDist", 0, 0, 2000};

    //ofParameter<ofVec3f> size {"size", ofVec3f(1000,1000,1000), ofVec3f(0,0,0), ofVec3f(10000,10000,10000) };
    
    ofParameter<ofFloatColor> matcolor {"color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    
    ofParameter<float> fadeOutFloor {"floorFadeOut", 0, 0, 1};
    
    
    ofParameter<ofVec3f> scaleCells {"scaleCells", ofVec3f(1,1,1),
        ofVec3f(0,0,0),
        ofVec3f(2,2,2)};
    
    
    ofParameterGroup wallParams {"wall",
        wallNumCells,
        wallSeed,
        wallOrigin,
        wallNoiseDisplaceSpeed,
        wallNoiseDisplaceAmount,
        openWall,
        fallWall,
        wallSize
    };
    
    ofParameterGroup oceanParams {"ocean",
        oceanNumCells,
        oceanSeed,
        oceanOrigin,
        oceanNoiseDisplaceSpeed,
        oceanNoiseDisplaceAmount,
        fadeOutFloor,
        fadeOutEdge,
        oceanFall,
    };
    
    ofParameterGroup params {"VoroEnd",
        enabled,
        enabledDraw,
        enabledDrawModel,
        qlab,
        stillAtEdge,
        stillAtEdgeDist,
        //horizonOrigin,
        dpPersist,
        dpPersistRadius,
        minDarkFade,
        //sceneDisplaceMod,
        scaleCells,
        matcolor
    };
    
    VoroEnd() {
        
        params.add(wallParams);
        params.add(oceanParams);
        params.add(mat.params);
        
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    
    void enable() {
        
        
        /*for( auto c : wall.getChildren()) {

            c->bOffsetPositionOveride = false;
        }
        
        for( auto c : oceanHorizon.getChildren()) {
            
            c->bOffsetPositionOveride = false;
        }*/

    }
    
    void reconstruct() {
        
        ofVec3f _s = getWorldSize();
        
        wallCenter.setParent(world->origin);
        floorCenter.setParent(world->origin);
        
        wallCenter.setPosition(0, _s.y, 0);
        floorCenter.setPosition(0, 0, _s.z);
        
        reconstructWall();
        reconstructOcean();
    }
    
    template<typename type>
    void reconstructWall(type & t) {
        
        /*if (t != wall.nCells) */
        bWallReconstruct = true;
    }
    
    template<typename type>
    void reconstructOcean(type & t) {
        /*if (t != ocean.nCells) */
        bOceanReconstruct = true;
    }
    
    void reconstructWall();
    void reconstructOcean();
    
    ofBoxPrimitive box;
    
    //VoroNode oceanHorizon;
    VoroNode ocean;
    VoroNode wall;
    
    ofVec3f clusterRotation;
    
    OrganicMaterial mat;
    
    ofNode wallCenter;
    ofNode floorCenter;
    
    bool bOceanReconstruct = false;
    bool bWallReconstruct = false;
    
    bool flagResetTransitionReferences = false;
    
};

