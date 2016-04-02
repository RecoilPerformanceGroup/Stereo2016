//
//  PlateauScene.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#ifndef PlateauScene_hpp
#define PlateauScene_hpp

#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"
#include "OrganicMaterial.hpp"
#include "dispatch/dispatch.h"

class PlateauScene : public ofxStereoscopy::Scene {
    
public:
    
    // Plateau
    
    VoroNode plateau;
    
    ofParameter<int> plateauRandomSeed {"randomSeed", 0,0,10000 };
    ofParameter<int> plateauCellCount {"cellCount", 2,1,1000 };
    ofParameter<ofVec3f> plateauSize {"size", ofVec3f(1000,1000,1000), ofVec3f(0,0,0), ofVec3f(10000,10000,10000) };
    ofParameter<ofVec3f> plateauPosition {"position", ofVec3f(0,-10000,-50000), ofVec3f(-10000,-10000,-10000), ofVec3f(10000,10000,10000) };
    ofParameter<ofVec3f> plateauRotation {"rotation", ofVec3f(0,0,0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360) };
    
    ofParameter<ofColor> plateauColor {"color", ofColor::white, ofColor::black, ofColor::white };

    ofParameter<ofVec2f> plateauZeroPosition {"zeroPosition", ofVec2f(0,0), ofVec2f(-1000,-1000), ofVec2f(1000,1000) };

    ofParameter<ofVec3f> selectPosition {"selectPosition", ofVec3f(0,-100000,-50000), ofVec3f(-10000,-10000,-100000), ofVec3f(10000,10000,100000) };
    ofParameter<float> selectRadius {"selectRadius", 2,0,2000};
    
    ofParameter<float> plateauSplit {"split", 2,0,2000};
    
    ofParameterGroup plateauParams {"Plateau",
        plateauRandomSeed,
        plateauCellCount,
        plateauSize,
        plateauPosition,
        plateauZeroPosition,
        plateauRotation,
        plateauColor,
        selectRadius,
        selectPosition,
        plateauSplit
    };
    
    // floor
    
    ofParameter<ofColor> floorColor {"color", ofColor::white, ofColor::black, ofColor::white };
    
    ofParameterGroup floorParams {"floor",
        floorColor
    };
    
    ofBoxPrimitive floor;
    
    // scene
    
    ofParameter<ofColor> anaglyphColorLeft {"anaglyphColorLeft", ofColor::cyan, ofColor::black, ofColor::white };
    ofParameter<ofColor> anaglyphColorRight {"anaglyphColorRight", ofColor::red, ofColor::black, ofColor::white };
    
    ofParameter<float> anaglyphAmount {"anaglyphAmount", 0.0, 0.0, 1.0 };
    
    ofParameterGroup params {"PlateauScene",
        enabled,
        qlab,
        anaglyphAmount,
        anaglyphColorLeft,
        anaglyphColorRight,
        floorParams
    };
    
    
    PlateauScene() {
        plateauParams.add(matPlateau.params);
        params.add(plateauParams);
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawModel();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    template<typename ParameterType>
    void reconstructPlateau( ParameterType & p){
        reconstructPlateau();
    }
    void reconstructPlateau();
    
    ofxPanel panel;
    
    OrganicMaterial matPlateau;
    OrganicMaterial matFloor;
    OrganicMaterial matAnaglyphRight;
    OrganicMaterial matAnaglyphLeft;
    
};

#endif /* PlateauScene_hpp */
