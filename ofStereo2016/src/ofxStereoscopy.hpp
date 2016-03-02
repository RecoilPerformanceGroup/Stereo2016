//
//  ofxStereoscopy.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 25/02/2016.
//
//

#ifndef ofxStereoscopy_hpp
#define ofxStereoscopy_hpp

#include "ofMain.h"

#define WORLD_DIMENSION_MAX 10000.0

namespace ofxStereoscopy {
    
    namespace Homography {
        
        void gaussian_elimination(float *input, int n){
            // ported to c from pseudocode in
            // http://en.wikipedia.org/wiki/Gaussian_elimination
            
            float * A = input;
            int i = 0;
            int j = 0;
            int m = n-1;
            while (i < m && j < n){
                // Find pivot in column j, starting in row i:
                int maxi = i;
                for(int k = i+1; k<m; k++){
                    if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
                        maxi = k;
                    }
                }
                if (A[maxi*n+j] != 0){
                    //swap rows i and maxi, but do not change the value of i
                    if(i!=maxi)
                        for(int k=0;k<n;k++){
                            float aux = A[i*n+k];
                            A[i*n+k]=A[maxi*n+k];
                            A[maxi*n+k]=aux;
                        }
                    //Now A[i,j] will contain the old value of A[maxi,j].
                    //divide each entry in row i by A[i,j]
                    float A_ij=A[i*n+j];
                    for(int k=0;k<n;k++){
                        A[i*n+k]/=A_ij;
                    }
                    //Now A[i,j] will have the value 1.
                    for(int u = i+1; u< m; u++){
                        //subtract A[u,j] * row i from row u
                        float A_uj = A[u*n+j];
                        for(int k=0;k<n;k++){
                            A[u*n+k]-=A_uj*A[i*n+k];
                        }
                        //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
                    }
                    
                    i++;
                }
                j++;
            }
            
            //back substitution
            for(int i=m-2;i>=0;i--){
                for(int j=i+1;j<n-1;j++){
                    A[i*n+m]-=A[i*n+j]*A[j*n+m];
                    //A[i*n+j]=0;
                }
            }
        }
        
        void findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]){
            
            // create the equation system to be solved
            //
            // from: Multiple View Geometry in Computer Vision 2ed
            //       Hartley R. and Zisserman A.
            //
            // x' = xH
            // where H is the homography: a 3 by 3 matrix
            // that transformed to inhomogeneous coordinates for each point
            // gives the following equations for each point:
            //
            // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
            // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
            //
            // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
            // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
            // after ordering the terms it gives the following matrix
            // that can be solved with gaussian elimination:
            
            float P[8][9]={
                {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
                {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
                
                {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
                {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
                
                {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
                {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
                
                {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
                {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
            };
            
            gaussian_elimination(&P[0][0],9);
            
            // gaussian elimination gives the results of the equation system
            // in the last column of the original matrix.
            // opengl needs the transposed 4x4 matrix:
            float aux_H[]={ P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31
                P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32
                0      ,      0,0,0,       // 0    0   0 0
                P[2][8],P[5][8],0,1};      // h13  h23 0 h33
            
            for(int i=0;i<16;i++) homography[i] = aux_H[i];
        }
        
        ofMatrix4x4 findHomography(ofPoint src[4], ofPoint dst[4]){
            ofMatrix4x4 matrix;
            
            // create the equation system to be solved
            //
            // from: Multiple View Geometry in Computer Vision 2ed
            //       Hartley R. and Zisserman A.
            //
            // x' = xH
            // where H is the homography: a 3 by 3 matrix
            // that transformed to inhomogeneous coordinates for each point
            // gives the following equations for each point:
            //
            // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
            // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
            //
            // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
            // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
            // after ordering the terms it gives the following matrix
            // that can be solved with gaussian elimination:
            
            float P[8][9]={
                {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
                {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
                
                {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
                {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
                
                {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
                {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
                
                {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
                {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
            };
            
            gaussian_elimination(&P[0][0],9);
            
            matrix(0,0)=P[0][8];
            matrix(0,1)=P[1][8];
            matrix(0,2)=0;
            matrix(0,3)=P[2][8];
            
            matrix(1,0)=P[3][8];
            matrix(1,1)=P[4][8];
            matrix(1,2)=0;
            matrix(1,3)=P[5][8];
            
            matrix(2,0)=0;
            matrix(2,1)=0;
            matrix(2,2)=0;
            matrix(2,3)=0;
            
            matrix(3,0)=P[6][8];
            matrix(3,1)=P[7][8];
            matrix(3,2)=0;
            matrix(3,3)=1;
            
            return matrix;
        }
        
        class Quad
        {
        public:
            /** Default constructor */
            Quad();
            Quad(ofPoint outputPoint1, ofPoint outputPoint2, ofPoint outputPoint3, ofPoint outputPoint4);
            void beginDraw();
            void endDraw();
            void update();
            void draw();
            void drawInputConfig();
            void drawOutputConfig();
            
            void setLineColor(ofColor value) { lineColor.set(value); }
            void setHexLineColor(int value) { lineColor.setHex(value); }
            
            
            ofParameter<ofVec3f> outputPointTopLeft { "Top Left",
                ofVec3f(0.0,0.0,0.0), ofVec3f(0,0,0), ofVec3f(1,1,1)
            };
            
            ofParameter<ofVec3f> outputPointTopRight { "Top Right",
                ofVec3f(1,0,0), ofVec3f(0,0,0), ofVec3f(1,1,1)
            };
            
            ofParameter<ofVec3f> outputPointBottomLeft { "Bottom Left",
                ofVec3f(0,1,0), ofVec3f(0,0,0), ofVec3f(1,1,1)
            };
            
            ofParameter<ofVec3f> outputPointBottomRight { "Bottom Right",
                ofVec3f(1,1,0), ofVec3f(0,0,0), ofVec3f(1,1,1)
            };
            
            ofParameterGroup outputPoints {
                "Output Points",
                outputPointTopLeft,
                outputPointTopRight,
                outputPointBottomLeft,
                outputPointBottomRight
            };
            
            ofParameterGroup params {
                "Quad Warper",
                outputPoints
            };
            
        protected:
        private:
            void drawConfig(ofPoint* points);
            
            GLfloat transformMatrix[16];
            
            ofColor lineColor;
        };
        
    }
    
    class Plane;
    
    class World {
        
    public:
        
        ofParameter<float> physical_eye_seperation_cm {"eye separation", 6.5, 0, 10};
        ofParameter<ofVec3f> physical_camera_pos_cm {"camera position", ofVec3f(0,250,1200), ofVec3f(-WORLD_DIMENSION_MAX/5,0,0), ofVec3f(WORLD_DIMENSION_MAX/5,WORLD_DIMENSION_MAX/5,WORLD_DIMENSION_MAX/5)};
        ofParameter<float> physical_focus_distance_cm {"focus distance", 200, 0, WORLD_DIMENSION_MAX};
        ofParameter<float> physical_camera_near_clip {"camera near clip", 20, 0, WORLD_DIMENSION_MAX};
        ofParameter<float> pixels_cm {"pixels pr. cm", 2, 0, 3};
        
        ofParameterGroup paramsCamera{
            "camera",
            physical_camera_pos_cm,
            physical_eye_seperation_cm,
            physical_focus_distance_cm,
            physical_camera_near_clip,
        };
        
        ofParameterGroup paramsPlanes;
        
        ofParameterGroup params{
            "world",
            paramsCamera,
            pixels_cm,
        };
        
        //TODO: Make parameter changes update all planes...
        
        std::map<std::string, shared_ptr<Plane>> planes;
        
        ofNode origin;
        
        void addPlane(shared_ptr<Plane> p);
        shared_ptr<Plane> getPlane(std::string name);
        
        //TODO: Make drawWorldModel
        //TODO: Make drawPlane
        
        void drawModel(bool showCameraFrustrums = true);
        void drawPlaneFBO(shared_ptr<Plane> p);
        
        void renderProjectorCalibrations();
        
        
        
    };
    
    class Plane : public ofPlanePrimitive{
        
    public:
        
        Plane(const string & name, float width, float height, const ofVec3f& pos_cm, const ofQuaternion& orientation_q, World * w){
            
            font.load("ofxbraitsch/fonts/Verdana.ttf", 32, true, true, true);
            
            ofPlanePrimitive::setWidth(width);
            ofPlanePrimitive::setHeight(height);
            ofPlanePrimitive::setGlobalPosition(pos_cm);
            ofPlanePrimitive::setGlobalOrientation(orientation_q);
            world = w;
            ofPlanePrimitive::setParent(world->origin);
            
            params.add(quadLeft.params);
            params.add(quadRight.params);
            
            world->params.add(params);
            
            setName(name);
            
            // TODO: bind size parameter to ofPlanePrimitive::width and ofPlanePrimitive::height
            // TODO: bind position parameter to ofPlanePrimitive::position
            // TODO: make a bound orientation parameter to ofPlanePrimitive::orientation quarternion
            
            dimensionsChanged();
            
        }
        
        ~Plane() {}
        
        void setName(std::string name){
            params.setName(name);
        }
        
        void setWidth(float w) {
            ofPlanePrimitive::setWidth(w);
            dimensionsChanged();
        }
        
        void setHeight(float h) {
            ofPlanePrimitive::setHeight(h);
            dimensionsChanged();
        }
        
        void setOrientation(const ofQuaternion& q) {
            ofPlanePrimitive::setOrientation(q);
        }
        
        void dimensionsChanged(ofRectangle viewport = ofGetCurrentViewport()){
            
            physical_size_cm.set(ofVec2f(width, height));
            
            ofFbo::Settings fboSettings;
            
            //TODO: Fix MSAA for FBOs
            
            //fboSettings.numSamples = 0;
            fboSettings.useDepth = true;
            fboSettings.width = floor(world->pixels_cm * width);
            fboSettings.height = floor(world->pixels_cm * height);
            //fboSettings.internalformat = GL_RGBA32F_ARB;
            fboSettings.internalformat = GL_RGBA;
            
            fboLeft.allocate(fboSettings);
            fboRight.allocate(fboSettings);
            
            textureLeft = fboLeft.getTexture();
            textureRight = fboRight.getTexture();
            
            textureLeft.setUseExternalTextureID(fboLeft.getTexture().texData.textureID);
            textureRight.setUseExternalTextureID(fboRight.getTexture().texData.textureID);
            
            fboLeft.begin();
            ofClear(255,255,0,255);
            fboLeft.end();
            
            fboRight.begin();
            ofClear(0,255,255,255);
            fboRight.end();
            
            camLeft.setParent(world->origin);
            camRight.setParent(world->origin);
            
            //camLeft.setScale(width, height, width);
            camLeft.setScale(1,1,1);
            camLeft.setNearClip(nearClip);
            camLeft.setFarClip(farClip);
            
            //camRight.setScale(width, height, width);
            camRight.setScale(1,1,1);
            camRight.setNearClip(nearClip);
            camRight.setFarClip(farClip);
            
            camsLookAt(getGlobalPosition()*getGlobalTransformMatrix());
            
        }
        
        void camsSetupPerspective(){
            camLeft.setupPerspective();
            camRight.setupPerspective();
        }
        
        void camsLookAt(ofVec3f v){
            camLeft.lookAt(v);
            camRight.lookAt(v);
        }
        
        void drawPlaneModel(){
            ofPushStyle();
            ofEnableDepthTest();
            ofPlanePrimitive::transformGL();
            ofSetColor(255,255);
            textureLeft.draw(-width/2, -height/2, width, height);
            ofSetColor(127, 255, 255);
            ofNoFill();
            ofDrawRectangle(-width/2, -height/2, width, height);
            ofFill();
            ofDisableDepthTest();
            font.drawStringAsShapes(params.getName(), (-width/2)+15, (height/2)-font.getLineHeight());
            ofPlanePrimitive::restoreTransformGL();
            ofPopStyle();
        }
        
        void drawCamerasModel(){
            drawCameraModel(camLeft, ofColor::cyan);
            drawCameraModel(camRight, ofColor::red);
        }
        
        void drawCameraModel(ofCamera cam, ofColor c){
            ofPushStyle();
            ofEnableDepthTest();
            
            ofPushMatrix();
            
            cam.transformGL();
            
            ofSetColor(c);
            
            ofFill();
            ofPushMatrix();
            ofRotateX(-90);
            ofDrawCone(0,-1,0, 1, 2);
            ofPopMatrix();
            
            ofPushMatrix();
            ofMultMatrix(cam.getProjectionMatrix().getInverse());
            ofNoFill();
            ofDrawBox(2.0, 2.0, 2.0);
            ofPopMatrix();
            
            cam.restoreTransformGL();
            
            ofPopMatrix();
            ofPopStyle();
        }
        
        void drawChessboard() {
            
            ofPushStyle();
            ofFill();
            ofEnableDepthTest();
            
            ofLight areaLight;
            areaLight.setParent(*this);
            areaLight.setup();
            areaLight.enable();
            areaLight.setAreaLight(400,400);
            areaLight.setAmbientColor(ofFloatColor(0.1,0.1,0.1));
            areaLight.setAttenuation(0.5,0.000001,0.000001);
            areaLight.setDiffuseColor(ofFloatColor(0.5,0.5,0.5));
            areaLight.setSpecularColor(ofFloatColor(1,1,1));
            areaLight.setGlobalPosition(-400,500,500);
            areaLight.setGlobalOrientation(ofQuaternion(-90, ofVec3f(1,0,0)));
            
            ofLight areaLight2;
            areaLight2.setParent(*this);
            areaLight2.setup();
            areaLight2.enable();
            areaLight2.setAreaLight(800,800);
            areaLight2.setAmbientColor(ofFloatColor(0.1,0.1,0.1));
            areaLight2.setAttenuation(0.5,0.000001,0.000001);
            areaLight2.setDiffuseColor(ofFloatColor(0.5,0.5,0.5));
            areaLight2.setSpecularColor(ofFloatColor(1,1,1));
            areaLight2.setGlobalPosition(400,600,100);
            areaLight2.setGlobalOrientation(ofQuaternion(-90, ofVec3f(1,0,0)));
            
            ofMaterial materialSphere;
            materialSphere.setAmbientColor(ofFloatColor(0.0,0.8,1.0,1.0));
            materialSphere.setDiffuseColor(ofFloatColor(0.8,0.8,0.4,1.0));
            materialSphere.setSpecularColor(ofFloatColor(0.8,0.8,0.8,1.0));
            materialSphere.setShininess(10);
            
            ofMaterial materialFloorWhite;
            materialFloorWhite.setAmbientColor(ofFloatColor(1.0,1.0,1.0,1.0));
            materialFloorWhite.setDiffuseColor(ofFloatColor(0.8,0.8,0.4,1.0));
            materialFloorWhite.setSpecularColor(ofFloatColor(0.8,0.8,0.8,1.0));
            materialFloorWhite.setShininess(10);
            
            ofMaterial materialFloorBlack;
            materialFloorBlack.setAmbientColor(ofFloatColor(0.1,0.1,0.1,1.0));
            materialFloorBlack.setDiffuseColor(ofFloatColor(0.8,0.8,0.4,1.0));
            materialFloorBlack.setSpecularColor(ofFloatColor(0.8,0.8,0.8,1.0));
            materialFloorBlack.setShininess(10);
            
            float chessSize = 100;
            
            ofPushMatrix(); {
                ofMultMatrix(getLocalTransformMatrix());
                
                ofEnableLighting();
                
                for(int x = 0; x < width/chessSize; x++){
                    for(int y = 0; y < height/chessSize; y++){
                        bool isWhite = (((y+x)%2)==1);
                        if(isWhite) {
                            materialFloorWhite.begin();
                        }else{
                            materialFloorBlack.begin();
                        }
                        ofPushMatrix(); {
                            
                            ofTranslate(-width/2, -height/2);
                            ofDrawRectangle(x*chessSize, y*chessSize, chessSize, chessSize);
                            
                        } ofPopMatrix();
                        
                        if(isWhite) {
                            materialFloorWhite.end();
                        }else{
                            materialFloorBlack.end();
                        }
                        
                    }
                }
                ofDisableLighting();
                
                
                
                ofSetColor(ofFloatColor(0.7,0.8,0.8,1.0));
                
                ofDisableDepthTest();
                font.drawString(params.getName(), (-width/2)+15, (height/2)-font.getLineHeight());
                ofEnableDepthTest();
                
                ofSetColor(255, 255);
                
                ofEnableLighting();
                materialSphere.begin();
                ofDrawSphere(0.0, 0.0, 150, width/6.0);
                materialSphere.end();
                
                
            } ofPopMatrix();
            
            ofPopStyle();
            
        }
        
        void renderChessboards() {
            
            beginLeft();
            drawChessboard();
            endLeft();
            
            beginRight();
            drawChessboard();
            endRight();
        }
        
        void beginLeft()
        {
            ofPushView();
            ofPushMatrix();
            float eye = world->physical_eye_seperation_cm;
            camLeft.setGlobalPosition(ofVec3f(world->physical_camera_pos_cm->x-(eye/2.0),world->physical_camera_pos_cm->y,world->physical_camera_pos_cm->z));
            
            ofVec3f windowTopLeft(-width / 2.0f,
                                  +height / 2.0f,
                                  0.0f);
            windowTopLeft = windowTopLeft * getLocalTransformMatrix();
            ofVec3f windowBottomLeft(-width / 2.0f,
                                     -height / 2.0f,
                                     0.0f);
            windowBottomLeft = windowBottomLeft * getLocalTransformMatrix();
            ofVec3f  windowBottomRight(+width / 2.0f,
                                       -height / 2.0f,
                                       0.0f);
            windowBottomRight = windowBottomRight * getLocalTransformMatrix();
            // To setup off axis view portal we need to be in the plane's matrix. All window coordinates of the camera are relative to the plane.
            ofPlanePrimitive::transformGL();
            camLeft.setupOffAxisViewPortal(windowTopLeft , windowBottomLeft, windowBottomRight );
            ofPlanePrimitive::restoreTransformGL();
            fboLeft.begin();
            ofClear(255,0);
            camLeft.begin();
        }
        
        void endLeft()
        {
            camLeft.end();
            fboLeft.end();
            ofPopMatrix();
            ofPopView();
        }
        
        void beginRight()
        {
            ofPushView();
            ofPushMatrix();
            float eye = world->physical_eye_seperation_cm;
            camRight.setGlobalPosition(ofVec3f(world->physical_camera_pos_cm->x+(eye/2.0),world->physical_camera_pos_cm->y,world->physical_camera_pos_cm->z));
            
            ofVec3f windowTopLeft(-width / 2.0f,
                                  +height / 2.0f,
                                  0.0f);
            windowTopLeft = windowTopLeft * getLocalTransformMatrix();
            ofVec3f windowBottomLeft(-width / 2.0f,
                                     -height / 2.0f,
                                     0.0f);
            windowBottomLeft = windowBottomLeft * getLocalTransformMatrix();
            ofVec3f  windowBottomRight(+width / 2.0f,
                                       -height / 2.0f,
                                       0.0f);
            windowBottomRight = windowBottomRight * getLocalTransformMatrix();
            // To setup off axis view portal we need to be in the plane's matrix. All local transforms of the camera are relative to it's parent node, the plane.
            ofPlanePrimitive::transformGL();
            camRight.setupOffAxisViewPortal(windowTopLeft , windowBottomLeft, windowBottomRight );
            ofPlanePrimitive::restoreTransformGL();
            fboRight.begin();
            ofClear(255,0);
            camRight.begin();
        }
        
        void endRight()
        {
            camRight.end();
            fboRight.end();
            ofPopMatrix();
            ofPopView();
        }
        
        void drawLeft(){
            quadLeft.beginDraw();
            ofScale(ofGetWidth(),ofGetHeight(),ofGetWidth());
            fboLeft.draw(0,0,1,1);
            quadLeft.endDraw();
        }
        
        void drawRight(){
            quadRight.beginDraw();
            ofScale(ofGetWidth(),ofGetHeight(),ofGetWidth());
            fboRight.draw(0,0,1,1);
            quadRight.endDraw();
        }
        
        //TODO: Add begin and end calls to enter and exit cameras and FBOs
        
        ofParameter<bool> enabled {true};
        ofParameter<ofVec3f> physical_pos_cm {"position", ofVec3f(0,0,0), ofVec3f(-WORLD_DIMENSION_MAX,-WORLD_DIMENSION_MAX,-WORLD_DIMENSION_MAX), ofVec3f(WORLD_DIMENSION_MAX,WORLD_DIMENSION_MAX,WORLD_DIMENSION_MAX)};
        ofParameter<ofVec2f> physical_size_cm {"size", ofVec2f(100,100), ofVec2f(0,0), ofVec2f(WORLD_DIMENSION_MAX*2,WORLD_DIMENSION_MAX*2)};
        
        ofParameterGroup params{
            "plane",
            enabled,
            physical_pos_cm,
            physical_size_cm,
        };
        
        ofCamera camLeft, camRight;
        ofFbo fboLeft, fboRight;
        ofTexture textureLeft, textureRight;
        ofTrueTypeFont font;
        World * world;
        Homography::Quad quadLeft, quadRight;
        
        float nearClip = 10;
        float farClip = 10000.0;
        
    };
    
    class Scene {
        
    public:
        
        int index;
        string indexStr;
        string name = "";
        string oscAddress = "/default";
        
        bool enabled;
        float time;
        
        //    ofxTimeline * mainTimeline;
        //    ofxTLCurves * tlenabled;
        
        Scene() {
        };
        
        virtual ~Scene(){}
        
        virtual void debugDraw(int _surfaceId=0) {};
        virtual void exit(){};
        //    virtual void receiveOsc(ofxOscMessage * m, string rest) {};
        
        /*    virtual void setGui(ofxUICanvas * gui, float width){
         gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
         gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
         gui->addSpacer(width, 1);
         //gui->addToggle(indexStr+"Enabled", &enabled);
         }
         
         virtual void guiEvent(ofxUIEventArgs &e) {};
         */
        void setName(string _name) {
            setName(_name, "/"+_name);
        }
        
        void setName(string _name, string _osc) {
            name = _name;
            oscAddress = _osc;
        }
        
        void setupScene(int _index) {
            if(name == "") setName("scene-" + ofToString(_index));
            
            index = _index;
            indexStr = "["+ ofToString(_index) + "] ";
            
            setup();
            
            //        mainTimeline->setCurrentPage("Page One");
            //        tlenabled = mainTimeline->addCurves("Enable " + name);
            
        };
        
        void updateScene() {
            
            //        enabled = (tlenabled->getValue() == 1);
            
            if(enabled) {
                update();
            }
        };
        
        void drawScene(int _surfaceId=0) {
            if(enabled) {
                glPushMatrix();ofPushMatrix();ofPushStyle();
                draw(_surfaceId);
                ofPopStyle();ofPopMatrix();glPopMatrix();
                
                glPushMatrix();ofPushMatrix();ofPushStyle();
                debugDraw(_surfaceId);
                ofPopStyle();ofPopMatrix();glPopMatrix();
            }
        };
        void beginSceneWorld(int _surfaceId=0){
            if(enabled) {
                beginWorld(_surfaceId);
            }
        };
        void endSceneWorld(int _surfaceId=0){
            if(enabled) {
                endWorld(_surfaceId);
            }
        };
        
        void setSurface(int _surfaceId) {
            primarySurface = _surfaceId;
        }
        
        int primarySurface = 0;
        
    private:
        virtual void setup(){};
        virtual void update(){};
        virtual void beginWorld(int _surfaceId=0){};
        virtual void endWorld(int _surfaceId=0){};
        virtual void draw(int _surfaceId=0){};
        
    };
    
}

#endif /* ofxStereoscopy_hpp */
