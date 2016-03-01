//
//  ofxStereoscopy.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 26/02/2016.
//
//

#include <ofxStereoscopy.hpp>

namespace ofxStereoscopy {
    
    const float World::dimensionMax=10000.0;
    
    void World::addPlane( shared_ptr<Plane> p){
        planes.insert(std::pair<string, shared_ptr<Plane>>(p->params.getName(),p));
    }
    
    shared_ptr<Plane> World::getPlane(std::string name){
        return planes[name];
    }
    
    void World::fboDrawProjectorCalibrations(){
        for(std::pair<string, shared_ptr<Plane>> p : planes){
            p.second->fboDrawChessboards();
        }
    }
    
    void World::drawModel() {
        
        for(std::pair<string, shared_ptr<Plane>> p : planes){
            p.second->drawPlaneModel();
        }

        ofPushMatrix();
        ofPushStyle();
        ofTranslate(physical_camera_pos_cm);
        ofSetColor(255, 0, 0);
        ofDrawSphere(1);
        ofPopStyle();
        ofPopMatrix();
        
        for(std::pair<string, shared_ptr<Plane>> p : planes){
            p.second->drawCameraModel();
        }
        ofDrawAxis(100);
        ofDrawBitmapString("+X", 100, 0, 0);
        ofDrawBitmapString("+Y", 0, 100, 0);
        ofDrawBitmapString("+Z", 0, 0, 100);
    }
    
}