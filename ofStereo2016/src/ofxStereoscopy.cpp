//
//  ofxStereoscopy.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 26/02/2016.
//
//

#include <ofxStereoscopy.hpp>

namespace ofxStereoscopy {
    
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
    
    void Calibrator::setPlane(shared_ptr<Plane> p, bool rightEye){
        plane = p;
        this->rightEye = rightEye;
        if(rightEye){
            quad = &p->quadRight;
        }else{
            quad = &p->quadLeft;
        }
    }
    
    void Calibrator::draw(){
        
        ofScale(1.0, 1.0*outputRectangle.getAspectRatio());
        
        ofFill();
        
        ofSetColor(255, 255, 255, 127);
        ofDrawRectangle(0,0,1.0,1.0);
        
        ofDisableDepthTest();
        
        if(!plane.expired()){
            
            shared_ptr<ofxStereoscopy::Plane> p = plane.lock();
            
            if(rightEye){
                p->drawRight();
                ofSetColor(p->rightColor);
            }else{
                p->drawLeft();
                ofSetColor(p->leftColor);
            }
            quad->drawOutputConfig();
            ofSetColor(ofColor::orangeRed);
            ofFill();
            ofDrawEllipse(mouseVec,0.03, 0.03*outputAspect);
            
            
            for (shared_ptr<ofAbstractParameter> cornerPoint : quad->outputPoints) {
                shared_ptr<ofParameter<ofVec3f>> cp = std::dynamic_pointer_cast<ofParameter<ofVec3f>>(cornerPoint);
                ofVec3f cpVec = cp->get();
                ofSetColor(ofColor::greenYellow);
                ofDrawEllipse(cpVec,0.03, 0.03*outputAspect);
                
            }
            if (!point.expired()) {
                ofSetColor(ofColor::yellow);
                ofDrawEllipse(point.lock()->get(), 0.05, 0.05*outputAspect);
            }
        }
    }
    
    void Calibrator::mouseDragged(ofVec3f v, int button){
        if(button== 0)mouseVec.set(v);
        if(!point.expired()){
            point.lock()->set(mouseVec);
        }
    }
    
    void Calibrator::mousePressed(ofVec3f v, int button){
        mouseVec.set(v);
        if(button == 0){
            
            if(!plane.expired()){
                
                shared_ptr<Plane> p = plane.lock();
                
                for (shared_ptr<ofAbstractParameter> cornerPoint : quad->outputPoints) {
                    shared_ptr<ofParameter<ofVec3f>> cp = std::dynamic_pointer_cast<ofParameter<ofVec3f>>(cornerPoint);
                    ofVec3f cpVec = cp->get();
                    if (cpVec.distance(mouseVec) < 0.06) {
                        point = cp;
                        point.lock()->set(mouseVec);
                        break;
                    }
                }
            }
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