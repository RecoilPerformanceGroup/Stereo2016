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


// provide specific seeds
//

/*struct Cell {
    ofMesh mesh;
    ofVec3f offset;
    ofVec3f rotation;
    ofColor color;
    float r = 0;
};*/

class VoroUnit : public ofNode {
public:
    
    // bounding box
    float width;
    float height;
    float depth;
    
    bool isSplit;
    bool bDraw;
    
    int level;
    
    ofVboMesh mesh;
    
    int nCells;
    
    list<VoroUnit *> children;

    VoroUnit() {
        isSplit = false;
        bDraw = false;
        level = 0;
        
    };
    
    VoroUnit(ofVboMesh _mesh) {
        isSplit = false;
        mesh = _mesh;
    };
    
    VoroUnit(ofVboMesh _mesh, VoroUnit & parent) {
        isSplit = false;
        bDraw = true;
        mesh = _mesh;
        setParent(parent);
        level += 1;
        
        // calculate bounding box from parent
        
    };
    
    ~VoroUnit() {
        
        for(auto s : children) {
            delete s;
        }
        
        children.clear();
    };
    
    
    void setParent(ofNode& parent, bool bMaintainGlobalTransform = false ){
        if(this->parent != nullptr){
            // we have a parent allready
            if (VoroUnit* oldVoroParent = dynamic_cast<VoroUnit*>(this->parent)) {
                // our old parent is also VoroUnit
                // remove this from parents' children.
                oldVoroParent->children.remove(this);
            }
        }
        if (VoroUnit* newVoroParent = dynamic_cast<VoroUnit*>(&parent)){
            // our new parent is a VoroUnit
            newVoroParent->children.push_back(this);
        }
        ofNode::setParent(parent, bMaintainGlobalTransform);
    };
    
    /// \brief Remove parent node linking
    void clearParent(bool bMaintainGlobalTransform = false){
        if(this->parent != nullptr){
            // we have a parent allready
            if (VoroUnit* oldVoroParent = dynamic_cast<VoroUnit*>(this->parent)) {
                // our old parent is also VoroUnit
                // remove this from parents' children.
                oldVoroParent->children.remove(this);
            }
        }
        ofNode::clearParent();
    }
    
    list<VoroUnit *> getChildren() {
        return children;
    };
    
    void split() {
        isSplit = true;
        
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
        
        for(ofMeshFace f : mesh.getUniqueFaces()){
            
            /** Constructs a plane wall object.
             * \param[in] (xc_,yc_,zc_) a normal vector to the plane.
             * \param[in] ac_ a displacement along the normal vector.
             * \param[in] w_id_ an ID number to associate with the wall for
             *		      neighbor tracking. */
            
            /*
            // dist_Point_to_Plane(): get distance (and perp base) from a point to a plane
            //    Input:  P  = a 3D point
            //            PL = a  plane with point V0 and normal n
            //    Output: *B = base point on PL of perpendicular from P
            //    Return: the distance from P to the plane PL
            float
            dist_Point_to_Plane( Point P, Plane PL, Point* B)
            {
                float    sb, sn, sd;
                
                sn = -dot( PL.n, (P - PL.V0));
                sd = dot(PL.n, PL.n);
                sb = sn / sd;
                
                *B = P + sb * PL.n;
                return d(P, *B);
            }
            */
            
            // UNTESTED!
            
            ofVec3f fNormal = f.getFaceNormal();
            ofVec3f fOrigin = ofVec3f(0,0,0);
            
            float sb, sn, sd;
            
            sn = -fNormal.dot(fOrigin - f.getVertex(0));
            sd = fNormal.dot(fNormal);
            sb = sn / sd;
            
            ofVec3f basePointOnPlane(fOrigin + sb * fNormal);
            
            float distance = fOrigin.distance(basePointOnPlane);
            
            voro::wall_plane planeWall(fNormal.x, fNormal.y, fNormal.z, distance);
            con.add_wall(planeWall);
        }
        
        for(int i = 0; i < nCells;i++){
            ofPoint newCell = ofPoint(ofRandom(-width,width),
                                      ofRandom(-height,height),
                                      ofRandom(-depth,depth));
            
            addCellSeed(con, newCell, i, true);
        }
        
        vector<ofVboMesh> cellMeshes = getCellsFromContainer(con, 0);
        
        children.clear();
        
        for(auto && m : cellMeshes) {
            
            VoroUnit * sub = new VoroUnit(m, *this);
            
        }
        
    };
    
    
    // start from a box
    void setup(float _w = 100, float _h = 100, float _d = 100, int _c = 10) {
        width  = _w;
        height = _h;
        depth  = _d;
        nCells = _c;
        
        generate();
    };
    
    void update() {
        //if(nCells != cells.size()) {
            //generate();
        //}
        
        for(auto c : getChildren()) {
            c->update();
        }
        
    };
    
    
    void customDraw() {
        
        
        if(bDraw) mesh.drawWireframe();
        
    };
    
    void draw() {
        
        ofNode::draw();
        
        for(auto c : getChildren()) {
            c->draw();
        }
    };
    
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
        
        vector<ofVboMesh> cellMeshes = getCellsFromContainer(con, 0);
        
        children.clear();
        
        for(auto && m : cellMeshes) {
            
            VoroUnit * sub = new VoroUnit(m, *this);
            
        }
        
        //cells.clear(); // todo clear children
        /*
        for (int i=0; i < cellMeshes.size(); i++) {
            Cell cell;
            cell.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
            cell.offset = ofVec3f(0,0,0);
            cell.r = ofRandom(-1.0,1.0);
            cell.mesh = cellMeshes[i];
            
            cells.push_back(cell);
        }*/
        
    }

};


class CrystalScene : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<int> numCells {"Cells", 40, 0, 1000};
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
    
    ofParameterGroup params {"",
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
        
        name = "Crystal Test";
        oscAddress = "crystaltest";
        
        ofxStereoscopy::Scene::params = params;
        params.setName(oscAddress);
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
    
    VoroUnit * cube;
    
    ofShader shader;
    ofPlanePrimitive plane;
        
    ofVec3f rotation;
    
    ofxPanel panel;
    
    ofMaterial mat;
    ofTexture tex;
};



