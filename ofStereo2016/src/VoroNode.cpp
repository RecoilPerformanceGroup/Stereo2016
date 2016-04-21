//
//  VoroNode.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 09/03/2016.
//
//

#include "VoroNode.hpp"

int VoroNode::counter = 0;
VoroNode * VoroNode::theNode = nullptr;

VoroNode::VoroNode() {
    counter++;
    isSplit = false;
    bDraw = false;
    level = 0;
    clearParent();
};

/*VoroNode::VoroNode(const VoroNode& o) : ofNode(o) {
    counter = o.counter;
    isSplit = o.isSplit;
    minBounds = o.minBounds;
    maxBounds = o.maxBounds;
    level = o.level;
    bDraw = o.bDraw;
    mesh = o.mesh;
    nCells = o.nCells;
};*/

VoroNode::VoroNode(ofVboMesh _mesh) {
    counter++;
    isSplit = false;
    mesh = _mesh;
    level = 0;
    clearParent();
    calculateBoundingBox();
};

VoroNode::VoroNode(ofVboMesh _mesh, VoroNode& vnParent) {
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


void VoroNode::getCellsFromContainerParallel(voro::container &_con, float _wallsThikness, vector<ofVboMesh>& cells, bool bWireframe){
    
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

void VoroNode::getCellMesh(voro::voronoicell &_c, ofPoint _pos, ofVboMesh& _mesh, bool bWireframe){
    if( _c.p ) {
        
        if( !bWireframe ) {
            //  Extract Verteces
            //
            ofVboMesh mesh;
            mesh.setMode(OF_PRIMITIVE_TRIANGLES );
            mesh.addVertices(getCellVerteces(_c, _pos));
            
            //  Add triangles using Indices
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

void VoroNode::calculateBoundingBox() {
    
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
}

void VoroNode::setParent(ofNode& p, bool bMaintainGlobalTransform){
    if(this->parent != nullptr){
        clearParent(bMaintainGlobalTransform);
    }
    if (VoroNode* newVoroParent = dynamic_cast<VoroNode *>(&p)){
        // our new parent is a VoroNode
        newVoroParent->addChild(*this);
    }
    ofNode::setParent(p, bMaintainGlobalTransform);
};

void VoroNode::clearParent(bool bMaintainGlobalTransform){
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

void VoroNode::removeChild(VoroNode & vnChild){
    voroChildren.erase(&vnChild);
}

void VoroNode::addChild(VoroNode & vnChild){
    voroChildren.insert(&vnChild);
}

void VoroNode::clearChildren(){
    for_each(voroChildren.begin(), voroChildren.end(), del_fun<VoroNode>());
    voroChildren.clear();
}

vector<VoroNode *> VoroNode::getChildren() {
    return vector<VoroNode *> (voroChildren.begin(), voroChildren.end());
};


VoroNode & VoroNode::detachNodes(set<VoroNode *> nodes) {
    
    VoroNode * newNode = new VoroNode();
    newNode->setParent(*this);
    
    for(auto n : nodes) {
        n->setParent(*newNode);
    }
    
    return *newNode;
}

ofVboMesh & VoroNode::getBakedMesh() {
    
    bakedMesh = mesh;
    // todo cache if node didn't change
    int iv = 0;
    for(auto v : mesh.getVertices()) {
        bakedMesh.setVertex(iv++, v*getGlobalTransformMatrix());
    }
    
    return bakedMesh;
}


vector<VoroNode *> VoroNode::getNearestChildren(ofPoint point, int maxNum, bool recursive) {
    
    vector<VoroNode *> sortme;
    
    for(auto n : voroChildren) {
        sortme.push_back(n);
        
        // Todo: make recursive
        /*if(recursive) {
            for(auto nn : n->getChildren()) {
                sortme.push_back(nn);
            }
            sortme.insert(n->getChildren().begin(), n->getChildren().end());
        }*/
    }
    
    std::sort(sortme.begin(), sortme.end(), [point](VoroNode * x, VoroNode * y){ return x->getGlobalPosition().distance(point) < y->getGlobalPosition().distance(point); });
    
    vector<VoroNode *> select(sortme.begin(), sortme.begin()+maxNum);
    
    return select;
}

// global flag ?
set<VoroNode *> VoroNode::getChildrenInSphere(ofPoint point, float radius, bool recursive) {
    
    set<VoroNode *> select;
    
    for(auto n : voroChildren) {
        if(n->getGlobalPosition().distance(point) < radius) {
            select.insert(n);
        }
        
        if(recursive) {
            set<VoroNode *> rSelect = n->getChildrenInSphere(point, radius);
            select.insert(rSelect.begin(), rSelect.end());
        }
    }
    
    return select;
}


void VoroNode::split(int _nCells, bool overFlowX, bool overFlowY, bool overFlowZ) {
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

void VoroNode::update() {
    //if(nCells != cells.size()) {
    //generate();
    //}
    
    for(auto c : getChildren()) {
        c->update();
    }
};

void VoroNode::setupFromBoundingBox(float _w, float _h, float _d, int _c, bool overFlowX, bool overFlowY, bool overFlowZ) {
    nCells = _c;
    
    boundingBox.set(_w, _h, _d);
    minBounds = ofVec3f(-_w/2.0, -_h/2.0, -_d/2.0);
    maxBounds = ofVec3f(_w/2.0, _h/2.0, _d/2.0);
    
    clearChildren();

    split(nCells, overFlowX, overFlowY, overFlowZ);
};


void VoroNode::customDraw() {
    if(bDraw) {
        
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(renderPosOffset);
        
        
        if(bRenderScaleSet) ofScale(renderScale);
        if(bTintSet) ofSetColor(tint);

        mesh.drawFaces();
        
        ofPopMatrix();
        ofPopStyle();
    }
}

void VoroNode::draw(OrganicMaterial * m) {
    if(m != nullptr){
        m->setWorldMatrix(getGlobalTransformMatrix());
        
        //m->setAmbientColor(tint);
        
        if(bTintSet) m->setDiffuseColor(tint);
        
        /*for( auto & c : mesh.getColors()) {
            c = tint;
        }*/
        
        
        m->begin();
    }
    
    ofNode::draw();
    
    if(m != nullptr){
        m->end();
    }
    
    for(auto c : getChildren()) {
        c->draw(m);
    }
};

VoroNode::~VoroNode() {
    
    if(this->parent){
        ofNode::clearParent();
    }
    
    clearChildren();
    counter--;
    
};






