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
    
    static VoroNode * theNode;
    static int counter;
    
    VoroNode();
    VoroNode(ofVboMesh _mesh);
    VoroNode(ofVboMesh _mesh, VoroNode& vnParent);
    
    //VoroNode(const VoroNode& other);
    
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
    
    ofVec3f minBounds;
    ofVec3f maxBounds;
    ofBoxPrimitive boundingBox;
    bool isSplit;
    bool bDraw;
    int level;
    ofVboMesh mesh;
    ofVboMesh bakedMesh;
    int nCells;
    
    ofVec3f renderPosOffset;
    ofVec3f velocity;
    
    ofFloatColor tint {1,1,1,1};
    bool bTintSet = false;
    
    void setTint(ofFloatColor col) {
        bTintSet = true;
        tint = col;
    }
    
    //int seed;
    
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
    
    vector<VoroNode *> getChildren();
    
    set<VoroNode *> getChildrenInSphere(ofPoint point, float radius, bool recursive = false);

    vector<VoroNode *> getNearestChildren(ofPoint point, int maxNum=1, bool recursive = false);
    //set<VoroNode *> getChildrenInBoundingBox( corners or plane + dir + dist);
    
    void split(int _nCells=3, bool overFlowX = false, bool overFlowY = false, bool overFlowZ = false);;
    
    // start from a box
    void setupFromBoundingBox(float _w = 100, float _h = 100, float _d = 100, int _c = 5, bool overFlowX = false, bool overFlowY = false, bool overFlowZ = false);
    
    void update();
    
    void customDraw();
    
    void draw(OrganicMaterial * m = nullptr);
    
    VoroNode & detachNodes(set<VoroNode *> nodes);
    
    ofVboMesh & getBakedMesh();
    
    template<typename meshType>
    static meshType bakeMesh(meshType _m, const ofNode & _node) {
        
        // todo cache if node didn't change
        for(auto & v : _m.getVertices()) {
            v.set(v * _node.getGlobalTransformMatrix());
        }
        
        return _m;
    }
    
    
};