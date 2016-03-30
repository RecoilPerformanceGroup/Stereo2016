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
#include "dispatch/dispatch.h"

// Vertex displacement using grand central dispatch
class VertexDisplacer {
public:
    
    dispatch_queue_t vertexQueue;
    
    void setup() {
        vertexQueue = dispatch_queue_create("stereo.voroNode.vertexQueue", NULL);
    }
    
    void twistModel(ofVboMesh & m, const ofNode * node) {
    
        // give it to dispatch
        
        float t = ofGetElapsedTimef();
        
        dispatch_async(vertexQueue, ^{
            
            for(int i=0; i<m.getNumVertices(); i++) {
                
                ofVec3f o = m.getVertex(i);
                
                ofVec3f oG = m.getVertex(i) + node->getPosition();
                
                ofVec3f v = m.getVertex(i);
                
                v.x += ofNoise(oG.z * 0.05, oG.y * 0.05, t * 0.5) * 10;
                v.y += ofNoise(oG.x * 0.05, oG.z * 0.05, t * 0.5) * 10;
                v.z += ofNoise(oG.y * 0.05, oG.x * 0.05, t * 0.5) * 10;
                
                m.setVertex(i, v);
            }
            
        });
    }
};


class CrystalScene : public ofxStereoscopy::Scene {
    
public:
    
    VertexDisplacer vertexDisplacer;
    
    ofParameter<int> numCells {"Cells", 2, 0, 200};
    ofParameter<ofVec3f> autoRotation {"Automatic rotation", ofVec3f(0,0,0),
        ofVec3f(-1,-1,-1),
        ofVec3f(1,1,1)};
    
    ofParameter<ofVec3f> origin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};
    
    ofParameter<float> autoRotationSpeed {"rot speed", 0, -1, 1};
    ofParameter<ofColor_<float>> crystalColor {"Crystal color", ofColor_<float>(1,1,1,1), ofColor_<float>(0,0,0,0), ofColor_<float>(1,1,1,1)};
    ofParameter<ofVec3f> spotlightPosition {"spotlightPosition", ofVec3f(0,0,0), ofVec3f(-2000,-2000,-2000), ofVec3f(2000,2000,2000)};
    ofParameter<float> scale {"scale", 0, 0, 10};
    ofParameter<float> scaleCells {"scaleCells", 0,0,0.999};
    ofParameter<bool> debugDraw {"Debug draw", false};
    ofParameter<bool> rebuild {"rebuild", false};
    
    ofParameterGroup params {"Crystal",
        enabled,
        rebuild,
        origin,
        numCells,
        autoRotation,
        autoRotationSpeed,
        crystalColor,
        spotlightPosition,
        scale,
        scaleCells,
        debugDraw};
    
    
    CrystalScene() {
        params.add(mat.noiseDisplacementParams);

        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawGui();
    void update();
    void setup();
    
    ofLight light;
    
    vector<ofPoint> cellCentroids;
    vector<float>   cellRadius;
    vector<ofMesh>  cellMeshes;
    
    ofLight pointlight;
    ofLight spotlight;
    
    VoroNode * cube;
    
    ofShader shader;
    ofPlanePrimitive plane;
    
    ofVec3f rotation;
    
    ofxPanel panel;
    
    OrganicMaterial mat;
    ofTexture tex;
};



