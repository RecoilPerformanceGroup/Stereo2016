#pragma once

#include "ofMain.h"

#include "ofMain.h"
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

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    vector<ofPoint> cellCentroids;
    vector<float>   cellRadius;
    vector<ofMesh>  cellMeshes;
    
    ofLight pointlight;
    ofLight spotlight;
    
    VoroCube * cube;
    
    ofShader shader;
    ofPlanePrimitive plane;

    ofEasyCam cam;
    
    ofParameterGroup params;
    
    ofParameter<int> numCells;
    ofParameter<ofVec3f> autoRotation;
    ofParameter<float> autoRotationSpeed;
    
    ofParameter<ofColor_<float>> crystalColor;
    ofParameter<ofVec3f> spotlightPosition;
    
    ofParameter<float> scale;
    
    ofParameter<float> scaleCells;
    
    ofVec3f rotation;
    
    ofParameter<bool> debugDraw;
    
    ofxPanel panel;
    
    ofMaterial mat;
    ofTexture tex;
    
    ofVideoGrabber grab;
    ofTexture * grabTex;
    
    ofFbo randFbo;
    
};
