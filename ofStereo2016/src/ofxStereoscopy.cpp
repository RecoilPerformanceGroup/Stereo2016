//
//  ofxStereoscopy.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 26/02/2016.
//
//

#include <ofxStereoscopy.hpp>

namespace ofxStereoscopy {
    
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
    
    void World::drawModel(bool showCameraFrustrums) {
        
        for(std::pair<string, shared_ptr<Plane>> p : planes){
            p.second->drawPlaneModel();
        }
        
        if(showCameraFrustrums){
            for(std::pair<string, shared_ptr<Plane>> p : planes){
                
                p.second->drawCamerasModel();
            }
        }
        ofDrawAxis(100);
        ofDrawBitmapString("+X", 100, 0, 0);
        ofDrawBitmapString("+Y", 0, 100, 0);
        ofDrawBitmapString("+Z", 0, 0, 100);
    }
    
}