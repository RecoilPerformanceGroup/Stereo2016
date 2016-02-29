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

namespace ofxStereoscopy {
    
    class Plane;
    
    class World {
        
    public:
        
        const static float dimensionMax;
        
        ofParameter<float> physical_eye_seperation_cm {"eye separation", 6.5, 0, 10};
        ofParameter<ofVec3f> physical_camera_pos_cm {"camera position", ofVec3f(0,250,1200), ofVec3f(-dimensionMax,-dimensionMax,-dimensionMax), ofVec3f(dimensionMax,dimensionMax,dimensionMax)};
        ofParameter<float> physical_focus_distance_cm {"focus distance", 200, 0, dimensionMax};
        ofParameter<float> physical_camera_near_clip {"camera near clip", 20, 0, dimensionMax};
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

        void addPlane(shared_ptr<Plane> p);
        shared_ptr<Plane> getPlane(std::string name);
        
        //TODO: Make drawWorldModel
        //TODO: Make drawPlane
        
        void drawModel();
        void drawPlaneFBO(shared_ptr<Plane> p);
        
        void fboDrawProjectorCalibrations();
        
        
        
    };
    
    class Plane : public ofPlanePrimitive{
        
    public:
        
        Plane(const string & name, float width, float height, const ofVec3f& pos_cm, const ofQuaternion& orientation_q){
            
            font.load("ofxbraitsch/fonts/Verdana.ttf", 32, true, true, true);
            
            ofPlanePrimitive::setWidth(width);
            ofPlanePrimitive::setHeight(height);
            ofPlanePrimitive::setGlobalPosition(pos_cm);
            ofPlanePrimitive::setOrientation(orientation_q);
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
            
            fboSettings.numSamples = 0;
            fboSettings.useDepth = true;
            fboSettings.width = floor(pixels_cm * width);
            fboSettings.height = floor(pixels_cm * height);
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
            
            //camLeft.setScale(width, height, width);
            camLeft.setScale(pixels_cm,pixels_cm,pixels_cm);
            camLeft.setNearClip(nearClip);
            
            //camRight.setScale(width, height, width);
            camRight.setScale(pixels_cm,pixels_cm,pixels_cm);
            camRight.setNearClip(nearClip);
            
            camsLookAt(ofVec3f(0,0,0));

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

        void drawCameraModel(){
            ofPushStyle();
            ofSetColor(0, 255, 255);
            ofPushMatrix();
            ofPlanePrimitive::transformGL();
                ofTranslate(camLeft.getGlobalPosition());
                ofDrawSphere(10);
            ofPlanePrimitive::restoreTransformGL();
            ofPopMatrix();
            ofSetColor(255, 255, 0);
            ofPushMatrix();
            ofPlanePrimitive::transformGL();
            ofTranslate(camRight.getGlobalPosition());
            ofDrawSphere(10);
            ofPlanePrimitive::restoreTransformGL();
            ofPopMatrix();
            ofPopStyle();
        }
        
        void drawChessboard() {

            ofPushStyle();
            ofFill();
            ofDisableDepthTest();
            
            float chessSize = 100;
            
            ofPushMatrix(); {

                ofTranslate(-width/2, -height/2);
                for(int x = 0; x < width/chessSize; x++){
                    for(int y = 0; y < height/chessSize; y++){
                        if(((y+x)%2)==1)
                            ofSetColor(255,255,255,127);
                        else
                            ofSetColor(0,0,0,127);
                        ofDrawRectangle(x*chessSize, y*chessSize, chessSize, chessSize);
                    }
                }

                ofSetColor(0, 100, 255, 127);
                ofDrawSphere(width/2, height/2, height/4);

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
            fboLeft.begin();
            //ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
            //ofClear(c);
            camLeft.setPosition(physical_pos_cm*getGlobalTransformMatrix());
            camLeft.setupOffAxisViewPortal(ofVec2f(0,0), ofVec2f(0,fboLeft.getHeight()), ofVec2f(fboLeft.getWidth(),fboLeft.getHeight()));
            camLeft.begin();
            ofTranslate(fboLeft.getWidth()/2,fboLeft.getHeight()/2);
            ofScale(pixels_cm,pixels_cm,pixels_cm);
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
            fboRight.begin();
//            ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
//            ofClear(c);
            float eye = physical_eye_seperation_cm / physical_focus_distance_cm;
            camLeft.setPosition((physical_pos_cm->operator+(ofVec3f(eye,0,0)))*getGlobalTransformMatrix());
            camRight.setupOffAxisViewPortal(ofVec2f(0,0), ofVec2f(0,fboRight.getHeight()), ofVec2f(fboRight.getWidth(),fboRight.getHeight()));
            camRight.begin();
            ofTranslate(fboRight.getWidth()/2,fboRight.getHeight()/2);
            ofScale(pixels_cm,pixels_cm,pixels_cm);

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
        ofParameter<ofVec3f> physical_pos_cm {"camera position", ofVec3f(0,250,1200), ofVec3f(-World::dimensionMax,-World::dimensionMax,-World::dimensionMax), ofVec3f(World::dimensionMax,World::dimensionMax,World::dimensionMax)};
        ofParameter<ofVec2f> physical_size_cm {"size", ofVec2f(100,100), ofVec2f(0,0), ofVec2f(World::dimensionMax*2,World::dimensionMax*2)};
        ofParameter<float> pixels_cm {"pixels pr. cm", 2, 0, 100};
        ofParameter<float> physical_eye_seperation_cm {"eye separation", 6.5, 0, 10};
        ofParameter<float> physical_focus_distance_cm {"focus distance", 200, 0, World::dimensionMax};

        ofParameterGroup params{
            "plane",
            enabled,
            physical_pos_cm,
            physical_size_cm,
            physical_eye_seperation_cm,
            physical_focus_distance_cm,
            pixels_cm,
        };
        
        ofCamera camLeft, camRight;
        ofFbo fboLeft, fboRight;
        ofTexture textureLeft, textureRight;
        ofTrueTypeFont font;
        
        float nearClip;

    };
    
    class Eyes {
    public:
        
        ofCamera left;
        ofCamera right;
        
        Eyes(): physical_eye_seperation_cm(6.5), physical_focus_distance_cm(200), focus_distance(200) {};
        ~Eyes(){};
        
        void setup(int w, int h)
        {
            height = 2.;
            width = height *(w/h);
            
            ofFbo::Settings settings;
            
            settings.height = h;
            settings.width = w;
            settings.numSamples = 8;
            settings.useDepth = true;
            //settings.colorFormats = GL_RGBA;
            
            leftFbo.allocate(settings);
            rightFbo.allocate(settings);
            
            left.setScale(1, 1, 1);
            left.setNearClip(0.1);
            
            right.setScale(1, 1, 1);
            right.setNearClip(0.1);
        }
        
        void setPosition(float x, float y, float z) {
            setPosition(ofVec3f(x,y,z));
        }
        
        void setPosition(ofVec3f pos) {
            left.setPosition(pos);
            right.setPosition(pos);
        }
        
        void setupPerspective(){
            left.setupPerspective();
            right.setupPerspective();
        }
        
        void lookAt(ofVec3f v){
            left.lookAt(v);
            right.lookAt(v);
        }
        
        void update(ofRectangle viewport = ofGetCurrentViewport())
        {
            
            eye = physical_eye_seperation_cm / physical_focus_distance_cm;
            
            //FIXME: WHAT IS THIS?
            
            /*aspect = viewport.width / viewport.height;
             fovy_tan = tanf(PI * left.getFov() / 360.);
             fovx_tan = fovy_tan * aspect;
             */
            //zNear = left.getNearClip();
            //zFar  = left.getFarClip();
            
            right.setPosition(left.getPosition().x + eye, left.getPosition().y, left.getPosition().z);
            
            right.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
            left.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
            
        }
        
        void setPhysicalEyeSeparation(float cm) { physical_eye_seperation_cm = cm; }
        void setPhysicalFocusDistance(float cm) { physical_focus_distance_cm = cm; }
        void setFocusDistance(float v) { focus_distance = v; }
        
        void beginLeft()
        {
            leftFbo.begin();
            ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
            ofClear(c);
            left.begin();
        }
        
        void endLeft()
        {
            left.end();
            leftFbo.end();
        }
        
        void beginRight()
        {
            rightFbo.begin();
            ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
            ofClear(c);
            right.begin();
        }
        
        void endRight()
        {
            right.end();
            rightFbo.end();
        }
        
        ofFbo leftFbo, rightFbo;
        
    protected:
        
        int width, height;
        
        float eye, focus_distance;
        
        float physical_eye_seperation_cm;
        float physical_focus_distance_cm;
        
        //FIXME: WHAT IS THIS?
        // parallel
        float zNear, zFar;
        float fovx_tan, fovy_tan;
        float aspect;
        
    private:
        
    };
    
    /*
    class oldPlane {
        
    public:
        // TODO frustrum - dynamicly calculate
        // TODO viewer world location - inherit from world
        
        //ofxStereoCamera<ofCamera> cam;
        
        oldPlane(string n) {
            name = n;
        };
        
        ~oldPlane() {
        };

        ofxStereoscopy::Eyes cam;
        
        ofRectangle viewport;
        
        int width;
        int height;
        
        int camWidth; // resolution
        int camHeight;
        
        float aspect = 1.0;
        
        ofVec3f camPos;
        
        string name;
        
        ofVec2f pos;
        
        
        //    ofxXmlSettings * settings;
        
        void setup(int w, int h) {
            
            viewport = ofRectangle(-1, -1, 2, 2);
            
            camHeight = 2.;
            camWidth = height *(w/h);
            
            width = w;
            height = h;
            
            cam.setup(w, h);
            
            cam.setPhysicalFocusDistance(120);
            cam.setFocusDistance(50);
            
            
        };
        
        void setViewPort(ofRectangle _viewport) {
            viewport = _viewport;
        };
        
        void drawGrid() {
            
            
            ofFill();
            
            ofDrawGrid(1., 4, true);
            ofDrawGrid(1., 4, true,false,true,false);
            
            ofLight l;
            
            l.setDirectional();
            l.setPosition(ofVec3f(-1,1,-1));
            l.lookAt(ofVec3f(0,0,0));
            
            l.enable();
            
            ofPushMatrix();
            ofTranslate(0,0,1);
            ofEnableLighting();
            ofDrawSphere(0.1);
            ofDisableLighting();
            ofPopMatrix();
            
            ofEnableLighting();
            ofDrawSphere(0.1);
            ofDisableLighting();
            
            ofPushMatrix();
            ofTranslate(0,0,-1);
            ofEnableLighting();
            ofDrawSphere(0.1);
            ofDisableLighting();
            ofPopMatrix();
            
            l.disable();
            
            ofPushMatrix();
            ofTranslate(0,1,0);
            ofDrawGrid(1., 4, true,false,true,false);
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(0,-1,0);
            ofDrawGrid(1., 4, true,false,true,false);
            ofPopMatrix();
            
        }
        
        void drawChessboard() {
            
            
            
            ofPushStyle();
            ofFill();
            ofDisableDepthTest();
            
            float chessSize = 0.05;
            
            ofPushMatrix(); {
                ofScale(1./(aspect*height*1.0/width),1, 1);
                
                bool white = true;
                bool red = true;
                int row = 0;
                
                ofColor col;
                
                ofTranslate(-1, -1);
                for(float x = 0; x < 2; x+=chessSize){
                    
                    row += 1;
                    
                    for(float y = 0; y < 2; y+=chessSize){
                        
                        col.set(200,200,200);
                        
                        if(!white) col.a = 0;
                        
                        ofSetColor(col.r, col.g, col.b, col.a);
                        white = !white;
                        
                        ofDrawRectangle(x, y, chessSize, chessSize);
                    }
                }
            } ofPopMatrix();
            
            
            ofPushMatrix();
            
            ofSetColor(255,255,255);
            ofDrawCircle(0, 0, 0.5, 0.5);
            
            
            ofSetColor(255,255,0);
            
            ofDrawCircle(-0.9, -0.9, 0.04);
            ofDrawCircle(0.9, 0.9, 0.04);
            ofDrawCircle(-0.9, 0.9, 0.04);
            ofDrawCircle(0.9, -0.9, 0.04);
            
            ofDrawCircle(0, 0.9, 0.04);
            ofDrawCircle(0, -0.9, 0.04);
            
            
            //ofDrawAxis(1);
            ofPopMatrix();
            ofPopStyle();
            
        }
        
        
        void drawMappingGrid() {
         
         
         ofPushStyle();
         ofFill();
         ofDisableDepthTest();
         
         float chessSize = 0.05;
         
         ofPushMatrix(); {
         ofScale(1./(aspect*height*1.0/width),1, 1);
         
         
         ofRect(-1,-1, 0.2, 2);
         
         
         
         
         } ofPopMatrix();
         
         
         ofPushMatrix();
         
         ofSetColor(255,0,0);
         ofEllipse(0, 0, 1, 1);
         //ofDrawAxis(1);
         ofPopMatrix();
         ofPopStyle();
         
         }
         
         void drawMappingGrids() {
         
         beginLeft();
         drawChessboard();
         endLeft();
         
         beginRight();
         drawChessboard();
         endRight();
         }
        
        
        void drawChessboards() {
            
            beginLeft();
            drawChessboard();
            endLeft();
            
            beginRight();
            drawChessboard();
            endRight();
        }
        
        void drawGrids() {
            cam.beginLeft();
            drawGrid();
            cam.endLeft();
            
            cam.beginRight();
            drawGrid();
            cam.endRight();
        }
        
        
        int controlSide = 0; // 0: left, 1: right
        
        void beginLeft(){
            ofPushView();
            glPushMatrix();
            cam.beginLeft();
            ofScale(aspect*height*1.0/width, 1.0, 1.0);
        }
        
        void endLeft(){
            cam.endLeft();
            glPopMatrix();
            ofPopView();
        }
        
        void beginRight(){
            ofPushView();
            glPushMatrix();
            cam.beginRight();
            ofScale(aspect*height*1.0/width, 1.0, 1.0);
        }
        
        void endRight(){
            cam.endRight();
            glPopMatrix();
            ofPopView();
        }
        
        void drawLeft() {
            ofPushMatrix();
            ofTranslate(pos.x, pos.y);
            cam.leftFbo.draw(0,0,width,height);
            ofPopMatrix();
        }
        
        void drawRight() {
            ofPushMatrix();
            ofTranslate(pos.x + width, pos.y);
            cam.rightFbo.draw(0,0,width,height);
            ofPopMatrix();
        }
        
        void draw(){
            ofSetColor(255);
            if(controlSide == 1) {
                drawRight();
                drawLeft();
            } else {
                drawLeft();
                drawRight();
            }
        }
        
        void update() {
            // observerPosition.set(sin(ofGetElapsedTimef()), cos(ofGetElapsedTimef()), -1);
            cam.setPosition(camPos);
            // cam.setFocusDistance( cam.left.getGlobalPosition().length() );
            cam.update(viewport);
        }
        
        void drawInfo() {
            ofDrawBitmapString(name, 10, 10);
        }
        
    private:
        
    };
     */
    
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
