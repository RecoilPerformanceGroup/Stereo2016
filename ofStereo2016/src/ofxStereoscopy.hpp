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
    
    class Plane;
    
    class World {
        
    public:
        
        ofParameter<float> physical_eye_seperation_cm {"eye separation", 6.5, 0, 10};
        ofParameter<ofVec3f> physical_camera_pos_cm {"camera position", ofVec3f(0,250,1200), ofVec3f(-WORLD_DIMENSION_MAX/5,0,0), ofVec3f(WORLD_DIMENSION_MAX/5,WORLD_DIMENSION_MAX/5,WORLD_DIMENSION_MAX/5)};
        ofParameter<float> physical_focus_distance_cm {"focus distance", 200, 0, WORLD_DIMENSION_MAX};
        ofParameter<float> physical_camera_near_clip {"camera near clip", 20, 0, WORLD_DIMENSION_MAX};
        ofParameter<float> pixels_cm {"pixels pr. cm", 2, 0, 100};
        
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
        
        void fboDrawProjectorCalibrations();
        
        
        
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
            ofPlanePrimitive::transformGL();
            ofSetColor(255,255);
            textureLeft.draw(-width/2, -height/2, width, height);
            ofSetColor(127, 255, 255);
            ofNoFill();
            ofDrawRectangle(-width/2, -height/2, width, height);
            ofFill();
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
            ofMaterial materialSphere;
            
            areaLight.setParent(*this);
            areaLight.setup();
            areaLight.enable();
            areaLight.setAreaLight(400,400);
            areaLight.setAmbientColor(ofFloatColor(0.1,0.1,0.1));
            areaLight.setAttenuation(0.5,0.000001,0.000001);
            areaLight.setDiffuseColor(ofFloatColor(1,1,1));
            areaLight.setSpecularColor(ofFloatColor(1,1,1));
            areaLight.setGlobalPosition(-400,500,500);
            areaLight.setGlobalOrientation(ofQuaternion(-90, ofVec3f(1,0,0)));
            
            materialSphere.setAmbientColor(ofFloatColor(1.0,0.0,1.0,1.0));
            materialSphere.setDiffuseColor(ofFloatColor(0.8,0.8,0.4,1.0));
            materialSphere.setSpecularColor(ofFloatColor(0.8,0.8,0.8,1.0));
            materialSphere.setShininess(10);
            
            float chessSize = 100;
            
            ofPushMatrix(); {
                ofMultMatrix(getLocalTransformMatrix());
                
                ofPushMatrix(); {
                    
                    ofTranslate(-width/2, -height/2);
                    for(int x = 0; x < width/chessSize; x++){
                        for(int y = 0; y < height/chessSize; y++){
                            if(((y+x)%2)==1)
                                ofSetColor(245,255,200,255);
                            else
                                ofSetColor(245,255,200,64);
                            ofDrawRectangle(x*chessSize, y*chessSize, chessSize, chessSize);
                        }
                    }
                    
                } ofPopMatrix();
                
                
                ofSetColor(ofFloatColor(0.7,0.8,0.8,1.0));
                
                ofDisableDepthTest();
                font.drawString(params.getName(), (-width/2)+15, (height/2)-font.getLineHeight());
                ofEnableDepthTest();
                
                ofSetColor(255, 255);
                
                ofEnableLighting();
                materialSphere.begin();
                ofDrawSphere(0.0, 0.0, sin(ofGetElapsedTimef())*100, width/6.0);
                materialSphere.end();
                
                
            } ofPopMatrix();
            
            ofPopStyle();
            
        }
        
        void fboDrawChessboards() {
            
            beginLeft();
            drawChessboard();
            endLeft();
            
            beginRight();
            drawChessboard();
            endRight();
        }
        
        void drawTest() {
            
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
            camLeft.setGlobalPosition(world->physical_camera_pos_cm);
            
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
            
            camLeft.setGlobalPosition(world->physical_camera_pos_cm);
            
            ofPushView();
            ofPushMatrix();
            float eye = world->physical_eye_seperation_cm;
            camRight.setGlobalPosition(ofVec3f(world->physical_camera_pos_cm->x+eye,world->physical_camera_pos_cm->y,world->physical_camera_pos_cm->z));
            
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
