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

class VoroNode : public ofNode {
public:
    
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
    list<VoroNode *> children;
    
    VoroNode() {
        isSplit = false;
        bDraw = false;
        level = 0;
    };
    
    VoroNode(ofVboMesh _mesh) {
        isSplit = false;
        mesh = _mesh;
        level = 0;
        
        calculateBoundingBox();
    };
    
    VoroNode(ofVboMesh _mesh, VoroNode & parent) {
        isSplit = false;
        bDraw = true;
        
        level = parent.level+1;
        
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
        
        setParent(parent);
        
    };
    
    
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
        
        for(auto s : children) {
            delete s;
        }
        
        children.clear();
    };
    
    void setParent(ofNode& parent, bool bMaintainGlobalTransform = false ){
        if(this->parent != nullptr){
            // we have a parent allready
            if (VoroNode* oldVoroParent = dynamic_cast<VoroNode*>(this->parent)) {
                // our old parent is also VoroNode
                // remove this from parents' children.
                oldVoroParent->children.remove(this);
            }
        }
        if (VoroNode* newVoroParent = dynamic_cast<VoroNode*>(&parent)){
            // our new parent is a VoroNode
            newVoroParent->children.push_back(this);
        }
        ofNode::setParent(parent, bMaintainGlobalTransform);
    };
    
    /// \brief Remove parent node linking
    void clearParent(bool bMaintainGlobalTransform = false){
        if(this->parent != nullptr){
            // we have a parent allready
            if (VoroNode* oldVoroParent = dynamic_cast<VoroNode*>(this->parent)) {
                // our old parent is also VoroNode
                // remove this from parents' children.
                oldVoroParent->children.remove(this);
            }
        }
        ofNode::clearParent();
    }
    
    list<VoroNode *> getChildren() {
        return children;
    };
    
    void split(int _nCells=3) {
        nCells = _nCells;
        
        isSplit = true;
        bDraw = false;
        
        
        voro::container con(minBounds.x,maxBounds.x,
                            minBounds.y,maxBounds.y,
                            minBounds.z,maxBounds.z,
                            1,1,1,
                            false,false,false, // set true to flow beyond box
                            8);
        
        //voro::wall_sphere sph(0, 0, 0, min(width, height) );
        //con.add_wall(sph);
        
        /*voro::wall_sphere sph2(0.5, 0.5, 0, min(width, height) );
         con.add_wall(sph2);
         */
        
        vector<voro::wall_plane*> wallsToDeleteAfterGettingCells;
        
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
        
        vector<ofVboMesh> cellMeshes = getCellsFromContainer(con, 0);
        
        for (auto w : wallsToDeleteAfterGettingCells){
            delete(w);
        }
        
        children.clear();
        
        for(auto && m : cellMeshes) {
            
            VoroNode * sub = new VoroNode(m, *this);
            
        }
        
    };
    
    
    // start from a box
    
    void setup(float _w = 100, float _h = 100, float _d = 100, int _c = 5) {
        width  = _w;
        height = _h;
        depth  = _d;
        nCells = _c;
        
        boundingBox.set(width, height, depth);
        
        // TODO: use split instead of generate
        // rename this method something like setupFromBoundingBox ...
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
        
        if(bDraw) mesh.drawFaces();
        
    }
    
    void draw() {
        
        ofNode::draw();
        
        for(auto c : getChildren()) {
            c->draw();
        }
    };
    
    void generate() {
        
        voro::container con(-width/2,width/2,
                            -height/2,height/2,
                            -depth/2,depth/2,
                            1,1,1,
                            true,false,true, // set true to flow beyond box
                            8);
        
        for(int i = 0; i < nCells;i++){
            ofPoint newCell = ofPoint(ofRandom(-width/2,width/2),
                                      ofRandom(-height/2,height/2),
                                      ofRandom(-depth/2,depth/2));
            
            addCellSeed(con, newCell, i, true);
        }
        
        vector<ofVboMesh> cellMeshes = getCellsFromContainer(con, 0);
        
        children.clear();
        
        vector<ofPoint> centroids = getCellsCentroids(con);
        
        for(auto && m : cellMeshes) {
            
            VoroNode * sub = new VoroNode(m, *this);
            
        }
        
        //cells.clear(); // todo clear children

    }
    
};

#endif /* VoroNode_hpp */
