//
//  VoroNode.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 09/03/2016.
//
//

#pragma once

#include "ofMain.h"
#include "ofxVoro.h"
#include "OrganicMaterial.hpp"
#include "dispatch/dispatch.h"

class VoroNode : public ofNode {
    
    
public:
    
    static int counter;
    
    VoroNode();
    VoroNode(ofVboMesh _mesh);
    VoroNode(ofVboMesh _mesh, VoroNode& vnParent);
    ~VoroNode();
    
    template<class T>
    struct del_fun_t
    {
        del_fun_t& operator()(T* p) {
            delete p;
            p = NULL;
            return *this;
        }
    };
    
    template<class T>
    del_fun_t<T> del_fun()
    { 
        return del_fun_t<T>(); 
    }
    
    //float width;
    //float height;
    //float depth;
    ofVec3f minBounds;
    ofVec3f maxBounds;
    ofBoxPrimitive boundingBox;
    bool isSplit;
    bool bDraw;
    int level;
    ofVboMesh mesh;
    int nCells;
    
    std::set<VoroNode*> voroChildren;
    
    void getCellsFromContainerParallel(voro::container &_con, float _wallsThikness, vector<ofVboMesh>& cells, bool bWireframe);
    
    void getCellMesh(voro::voronoicell &_c, ofPoint _pos, ofVboMesh& _mesh, bool bWireframe);
    
    void calculateBoundingBox();
    
    void setParent(ofNode& p, bool bMaintainGlobalTransform = false );
    
    /// \brief Remove parent node linking
    void clearParent(bool bMaintainGlobalTransform = false);
    
    void removeChild(VoroNode & vnChild);
    
    void addChild(VoroNode & vnChild);
    
    void clearChildren();
    
    set<VoroNode *> getChildren();
    
    set<VoroNode *> getChildrenInSphere(ofPoint point, float radius);
    //set<VoroNode *> getChildrenInBoundingBox( corners or plane + dir + dist);
    
    void split(int _nCells=3, bool overFlowX = false, bool overFlowY = false, bool overFlowZ = false);;
    
    // start from a box
    void setupFromBoundingBox(float _w = 100, float _h = 100, float _d = 100, int _c = 5, bool overFlowX = false, bool overFlowY = false, bool overFlowZ = false);
    
    void update();
    
    void customDraw();
    
    void draw(OrganicMaterial * m = nullptr);
    
   
};