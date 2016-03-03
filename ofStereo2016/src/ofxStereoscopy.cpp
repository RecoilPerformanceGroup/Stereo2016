//
//  ofxStereoscopy.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 26/02/2016.
//
//

#include <ofxStereoscopy.hpp>

namespace ofxStereoscopy {
    
    namespace Homography {
    
        Quad::Quad() {
            Quad(ofPoint(0, 0), ofPoint(1, 0), ofPoint(1, 1), ofPoint(0, 1));
        }
        
        Quad::Quad(ofPoint outputPoint1, ofPoint outputPoint2, ofPoint outputPoint3, ofPoint outputPoint4) {
            outputPointTopLeft.set(outputPoint1);
            outputPointTopRight.set(outputPoint2);
            outputPointBottomLeft.set(outputPoint3);
            outputPointBottomRight.set(outputPoint4);
            
            lineColor.setHex(0xffffff);
        }
        
        void Quad::beginDraw() {
            // Start quad transform
            
            ofPoint inputPoints[4] = {ofPoint(0,0), ofPoint(1,0), ofPoint(0,1), ofPoint(1,1)};
            ofPoint outputPoints[4] = {outputPointTopLeft, outputPointTopRight, outputPointBottomLeft, outputPointBottomRight};
            
            findHomography(inputPoints, outputPoints, transformMatrix);
            ofPushMatrix();
            ofMultMatrix(transformMatrix); //previously glMultMatrixf(transformMatrix)
        }
        
        void Quad::endDraw() {
            // End quad transform
            ofPopMatrix();
        }
        
        void Quad::drawInputConfig() {
            ofPoint inputPoints[4] = {ofPoint(0,0), ofPoint(1,0), ofPoint(0,1), ofPoint(1,1)};
            drawConfig(inputPoints);
        }
        
        void Quad::drawOutputConfig() {
            ofPoint outputPoints[4] = {outputPointTopLeft, outputPointTopRight, outputPointBottomLeft, outputPointBottomRight};
            drawConfig(outputPoints);
        }
        
        void Quad::drawConfig(ofPoint* points) {
            ofPushStyle();
            ofNoFill();
            ofSetColor(lineColor);
            ofBeginShape();
            for (int i = 0; i < 5; i++) {
                ofVertex(points[i%4].x, points[i%4].y);
            }  
            ofEndShape();
            
            ofPopStyle();
        }
    
    }
    
    void World::addPlane( shared_ptr<Plane> p){
        planes.insert(std::pair<string, shared_ptr<Plane>>(p->params.getName(),p));
    }
    
    shared_ptr<Plane> World::getPlane(std::string name){
        return planes[name];
    }
    
    void World::renderProjectorCalibrations(){
        for(std::pair<string, shared_ptr<Plane>> p : planes){
            p.second->renderChessboards();
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
        ofDisableDepthTest();
        ofDrawAxis(100);
        ofDrawBitmapString("+X", 100, 0, 0);
        ofDrawBitmapString("+Y", 0, 100, 0);
        ofDrawBitmapString("+Z", 0, 0, 100);
    }
    
}