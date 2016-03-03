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
#include "ofxVoro.h"
#include "ofxGui.h"


struct Cell {
    ofMesh mesh;
    ofVec3f offset;
    ofVec3f rotation;
    ofColor color;
    float r = 0;
};

class VoroCube {
public:
    
    float width;
    float height;
    float depth;
    
    vector<ofVboMesh>  cellMeshes;
    int nCells;
    vector<Cell> cells;
    
    void setup(float _w = 0.2, float _h = 0.2, float _d = 0.2, int _c = 40) {
        width  = _w;
        height = _h;
        depth  = _d;
        nCells = _c;
        
        generate();
    }
    
    void update() {
        if(nCells != cells.size()) {
            //generate();
        }
    }
    
    void generate() {
        
        
        voro::container con(-width,width,
                            -height,height,
                            -depth,depth,
                            1,1,1,
                            false,false,false, // set true to flow beyond box
                            8);
        
        //voro::wall_sphere sph(0, 0, 0, min(width, height) );
        //con.add_wall(sph);
        
        /*voro::wall_sphere sph2(0.5, 0.5, 0, min(width, height) );
         con.add_wall(sph2);
         */
        
        for(int i = 0; i < nCells;i++){
            ofPoint newCell = ofPoint(ofRandom(-width,width),
                                      ofRandom(-height,height),
                                      ofRandom(-depth,depth));
            
            addCellSeed(con, newCell, i, true);
        }
        
        
        cellMeshes = getCellsFromContainer(con, 0);
        
        cells.clear();
        for (int i=0; i < cellMeshes.size(); i++) {
            
            Cell cell;
            cell.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
            
            cell.offset = ofVec3f(0,0,0);
            cell.r = ofRandom(-1.0,1.0);
            cell.mesh = cellMeshes[i];
            
            cells.push_back(cell);
        }
    }
};


class CrystalScene : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<int> numCells {"Cells", 40, 0, 1000};
    ofParameter<ofVec3f> autoRotation {"Automatic rotation", ofVec3f(0,0,0),
        ofVec3f(-1,-1,-1),
        ofVec3f(1,1,1)};
    
    ofParameter<float> autoRotationSpeed {"rot speed", 0, -1, 1};
    ofParameter<ofColor_<float>> crystalColor {"Crystal color", ofColor_<float>(1,1,1,1), ofColor_<float>(0,0,0,0), ofColor_<float>(1,1,1,1)};
    ofParameter<ofVec3f> spotlightPosition {"spotlightPosition", ofVec3f(0,0,0), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)};
    ofParameter<float> scale {"scale", 0, 0, 10};
    ofParameter<float> scaleCells {"scaleCells", 0,0,0.999};
    ofParameter<bool> debugDraw {"Debug draw", false};
    
    ofParameterGroup params {"CrystalParams",
        numCells,
        autoRotation,
        autoRotationSpeed,
        crystalColor,
        spotlightPosition,
        scale,
        scaleCells,
        debugDraw};
    
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
    
    VoroCube * cube;
    
    ofShader shader;
    ofPlanePrimitive plane;
        
    ofVec3f rotation;
    
    ofxPanel panel;
    
    ofMaterial mat;
    ofTexture tex;
};



