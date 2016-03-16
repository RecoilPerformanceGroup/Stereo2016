//
//  VoroNode.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 09/03/2016.
//
//

#ifndef VoroNode_hpp
#define VoroNode_hpp
#include "ofMain.h"
#include "ofxVoro.h"
#include "OrganicMaterial.hpp"
#include "dispatch/dispatch.h"

class VoroNode : public ofNode {
    
    
public:
    
    static int counter;
    
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
    
    // bounding box
    float width;
    float height;
    float depth;
    
    ofVec3f minBounds;
    ofVec3f maxBounds;
    
    ofBoxPrimitive boundingBox;
    
    bool isSplit;
    bool bDraw;
    
    int level;
    
    ofVboMesh mesh;
    
    int nCells;
    std::set<VoroNode*> voroChildren;
    
    VoroNode() {
        counter++;
        isSplit = false;
        bDraw = false;
        level = 0;
        clearParent();
    };
    
    VoroNode(ofVboMesh _mesh) {
        counter++;
        isSplit = false;
        mesh = _mesh;
        level = 0;
        clearParent();
        calculateBoundingBox();
    };
    
    VoroNode(ofVboMesh _mesh, VoroNode& vnParent) {
        counter++;
        isSplit = false;
        bDraw = true;
        
        level = vnParent.level+1;
        
        // make the mesh vertices local for the node
        // move th relative position to the nodes position
        // transformations on the unit are around the centroid of the mesh not the centroid of the parent
        mesh = _mesh;
        setPosition(mesh.getCentroid());
        
        for(int i=0; i<_mesh.getNumVertices(); i++) {
            mesh.setVertex(i,  _mesh.getVertex(i)-_mesh.getCentroid());
        }
        
        mesh.setupIndicesAuto();
        
        calculateBoundingBox();
        
        setParent(vnParent);
        
    };
    
    void getCellsFromContainerParallel(voro::container &_con, float _wallsThikness, vector<ofVboMesh>& cells, bool bWireframe){
        
        cells.clear();
        
        voro::c_loop_all vl( _con );
        if( vl.start() ){
            
            do {
//                dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
                    voro::voronoicell c;
                if( !_con.compute_cell(c, vl) ) {
                    return;
                } else {
                        double *pp = _con.p[vl.ijk] + _con.ps * vl.q;
                        ofVboMesh cellMesh;
                        getCellMesh(c, ofPoint(pp[0],pp[1],pp[2])*(float)(1.0+_wallsThikness), cellMesh, bWireframe);
                        cells.push_back( cellMesh );
                }
 
//                });
                
            } while( vl.inc() );
        }
    }
    
    void getCellMesh(voro::voronoicell &_c, ofPoint _pos, ofVboMesh& _mesh, bool bWireframe){
        if( _c.p ) {
            
            if( !bWireframe ) {
                //  Extract Verteces
                //
                ofVboMesh mesh;
                mesh.setMode(OF_PRIMITIVE_TRIANGLES );
                mesh.addVertices(getCellVerteces(_c, _pos));
                
                //  Add triangles using Indeces
                //
                int k,l,m,n;
                for(int i = 1; i < _c.p; i++){
                    for(int j = 0; j < _c.nu[i]; j++) {
                        
                        k = _c.ed[i][j];
                        
                        if( k >= 0 ) {
                            _c.ed[i][j]=-1-k;
                            l = _c.cycle_up( _c.ed[i][ _c.nu[i]+j], k);
                            m = _c.ed[k][l];
                            _c.ed[k][l]=-1-m;
                            
                            while(m!=i) {
                                n = _c.cycle_up( _c.ed[k][ _c.nu[k]+l],m);
                                mesh.addTriangle(i, k, m);
                                
                                k=m;
                                l=n;
                                m=_c.ed[k][l];
                                _c.ed[k][l]=-1-m;
                            }
                        }
                    }
                }
                
                //  Calculate Normals
                //
                _mesh.setMode(OF_PRIMITIVE_TRIANGLES);
                vector<ofMeshFace> faces = mesh.getUniqueFaces();
                for (int i = 0; i < faces.size(); i++) {
                    ofMeshFace face = faces[i];
                    ofPoint a = face.getVertex(0);
                    ofPoint b = face.getVertex(1);
                    ofPoint c = face.getVertex(2);
                    
                    ofPoint normal = ((b - a).cross(c - a)).normalize() * -1.;
                    
                    
                    _mesh.addVertex(a);
                    _mesh.addNormal(normal);
                    
                    _mesh.addVertex(b);
                    _mesh.addNormal(normal);
                    _mesh.addVertex(c);
                    _mesh.addNormal(normal);
                    
                    // add texture coordinates
                    if( i % 2 == 0) {
                        _mesh.addTexCoord(ofVec2f(0.0, 0.0));
                        _mesh.addTexCoord(ofVec2f(0.0, 1.0));
                        _mesh.addTexCoord(ofVec2f(1.0, 0.0));
                    } else {
                        _mesh.addTexCoord(ofVec2f(1.0, 0.0));
                        _mesh.addTexCoord(ofVec2f(0.0, 1.0));
                        _mesh.addTexCoord(ofVec2f(1.0, 1.0));
                    }
                    
                    
                }
            } else { // wireframe
                _mesh.setMode(OF_PRIMITIVE_LINES);
                _mesh.addVertices(getCellVerteces(_c, _pos));
                for(int i = 1; i < _c.p; i++){
                    for(int j = 0; j < _c.nu[i]; j++) {
                        
                        int k = _c.ed[i][j];
                        
                        if( k >= 0 ) {
                            _mesh.addIndex(i);
                            _mesh.addIndex(k);
                        }
                    }
                }
            }
        }
    }

    
    void calculateBoundingBox() {
        
        minBounds = mesh.getVertex(0);
        maxBounds = mesh.getVertex(0);
        
        for(int i=0; i<mesh.getNumVertices(); i++) {
            
            minBounds.x = min(mesh.getVertex(i).x, minBounds.x);
            minBounds.y = min(mesh.getVertex(i).y, minBounds.y);
            minBounds.z = min(mesh.getVertex(i).z, minBounds.z);
            
            maxBounds.x = max(mesh.getVertex(i).x, maxBounds.x);
            maxBounds.y = max(mesh.getVertex(i).y, maxBounds.y);
            maxBounds.z = max(mesh.getVertex(i).z, maxBounds.z);
        }
        
        
        float width = abs(maxBounds.x - minBounds.x);
        float height = abs(maxBounds.y - minBounds.y);
        float depth = abs(maxBounds.z - minBounds.z);
        
        boundingBox.set(width, height, depth);
        boundingBox.setPosition(maxBounds.getMiddle(minBounds));
    };
    
    ~VoroNode() {

        if(this->parent){
            ofNode::clearParent();
        }
        
        clearChildren();
        
        counter--;
        
    };
    
    void setParent(ofNode& p, bool bMaintainGlobalTransform = false ){
        if(this->parent != nullptr){
            clearParent(bMaintainGlobalTransform);
        }
        if (VoroNode* newVoroParent = dynamic_cast<VoroNode *>(&p)){
            // our new parent is a VoroNode
            newVoroParent->addChild(*this);
        }
        ofNode::setParent(p, bMaintainGlobalTransform);
    };
    
    /// \brief Remove parent node linking
    void clearParent(bool bMaintainGlobalTransform = false){
        if(parent){
            // we have a parent allready
            if (VoroNode* oldVoroParent = dynamic_cast<VoroNode*>(this->parent)) {
                // our old parent is also VoroNode
                // remove this from parents' children.
                oldVoroParent->removeChild(*this);
            }
            ofNode::clearParent(bMaintainGlobalTransform);
        }
    }
    
    void removeChild(VoroNode & vnChild){
        voroChildren.erase(&vnChild);
    }
    
    void addChild(VoroNode & vnChild){
        voroChildren.insert(&vnChild);
    }
    
    void clearChildren(){
        for_each(voroChildren.begin(), voroChildren.end(), del_fun<VoroNode>());
        voroChildren.clear();
    }
    
    set<VoroNode *> getChildren() {
        return voroChildren;
    };
    
    void split(int _nCells=3, bool overFlowX = false, bool overFlowY = false, bool overFlowZ = false) {
        nCells = _nCells;
        
        isSplit = true;
        bDraw = false;
        
        
        voro::container con(minBounds.x,maxBounds.x,
                            minBounds.y,maxBounds.y,
                            minBounds.z,maxBounds.z,
                            1,1,1,
                            overFlowX,overFlowY,overFlowZ, // set true to flow beyond box
                            8);
        
        vector<voro::wall_plane*> wallsToDeleteAfterGettingCells;
        
        for(ofMeshFace f : mesh.getUniqueFaces()){
            
            ofVec3f fNormal = f.getFaceNormal();
            ofVec3f fOrigin = ofVec3f(0,0,0);
            
            float sb, sn, sd;
            
            sn = -fNormal.dot(fOrigin - f.getVertex(1));
            sd = fNormal.dot(fNormal);
            sb = sn / sd;
            
            ofVec3f basePointOnPlane(fOrigin + sb * fNormal);
            
            float distance = fOrigin.distance(basePointOnPlane);
            
            voro::wall_plane * planeWall = new voro::wall_plane(-fNormal.x, -fNormal.y, -fNormal.z, distance);
            wallsToDeleteAfterGettingCells.push_back(planeWall);
            con.add_wall(planeWall);
            
        }
        
        
        for(int i = 0; i < nCells;i++){
            ofPoint newCell = ofPoint(ofRandom(minBounds.x,maxBounds.x),
                                      ofRandom(minBounds.y,maxBounds.y),
                                      ofRandom(minBounds.z,maxBounds.z));
            if(insideContainer(con, newCell))
                addCellSeed(con, newCell, i, true);
            else
                i--;
        }
        
        vector<ofVboMesh> cellMeshes;
        getCellsFromContainerParallel(con, 0, cellMeshes, false);
            
        for (auto w : wallsToDeleteAfterGettingCells){
            delete(w);
        }
        
        clearChildren();
        
        for(auto && m : cellMeshes) {
            
            VoroNode * sub = new VoroNode(m, *this);
            
        }
        
    };
    
    
    // start from a box
    
    void setup(float _w = 100, float _h = 100, float _d = 100, int _c = 5, bool overFlowX = false, bool overFlowY = false, bool overFlowZ = false) {
        width  = _w;
        height = _h;
        depth  = _d;
        nCells = _c;
        
        boundingBox.set(width, height, depth);
        minBounds = ofVec3f(-width/2.0, -height/2.0, -depth/2.0);
        maxBounds = ofVec3f(width/2.0, height/2.0, depth/2.0);
        
        clearChildren();
        
        // TODO: use split instead of generate
        // rename this method something like setupFromBoundingBox ...
        // generate();
        
        split(nCells, overFlowX, overFlowY, overFlowZ);
        
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
        
        if(bDraw) mesh.drawFaces();
        
    }
    
    void draw(OrganicMaterial * m = nullptr) {
        if(m != nullptr){
            m->setWorldMatrix(getGlobalTransformMatrix());
        }
        ofNode::draw();
        
        for(auto c : getChildren()) {
            c->draw(m);
        }
    };
    
    
};

#endif /* VoroNode_hpp */
