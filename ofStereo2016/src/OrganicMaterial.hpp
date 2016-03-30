//
//  OrganicMaterial.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/11/16.
//
//

#pragma once

#include "ofMain.h"


class OrganicMaterial : public ofBaseMaterial {
public:
    
    
    
    
    // Parameters for vertex displacements
    
    
    // twist
    // Work in progress
    /*ofVec3f twistDisplacementTime {0,0,0};
    
    ofParameter<ofVec3f> twistDisplacementVelocity {"velocity",
        ofVec3f(0,0,0),
        ofVec3f(-10,-10,-10),
        ofVec3f(10,10,10)};
    
    ofParameter<ofVec3f> twistDisplacementAmount {"amount",
        ofVec3f(0,0,0),
        ofVec3f(0,0,0),
        ofVec3f(10000,10000,10000)};
    
    ofParameter<ofVec3f> twistAround {"amount",
        ofVec3f(0,0,0),
        ofVec3f(0,0,0),
        ofVec3f(10000,10000,10000)};*/
    
    
    // Rewrite to GLSL
    /*ofVec3f rotateAround = cube->getPosition();
     for(auto c : cube->getChildren()) {
     
     
     ofMesh m = c->mesh;
     for(int i=0; i<  m.getNumVertices(); i++) {
     
     ofVec3f v = m.getVertex(i);
     ofVec3f gv = v * c->getGlobalTransformMatrix();
     //ofVec3f vn = v+c->getPosition();//gv;
     
     //ofDrawSphere(v * c->getGlobalTransformMatrix(), 5);
     
     //float rotateAmt = sin(ofGetElapsedTimef() + (gv.y / 100.0)) * 90;
     
     //vn.rotate(rotateAmt, rotateAround, ofVec3f(0,1,0));
     
     //m.setVertex(i, vn);
     m.setVertex(i, gv);
     }
     
     mat.begin();
     m.draw();
     mat.end();
     }*/

    
    
    // noise
    ofVec3f noiseDisplacementTime {0,0,0};

    ofParameter<ofVec3f> noiseDisplacementVelocity {"velocity",
        ofVec3f(0,0,0),
        ofVec3f(-10,-10,-10),
        ofVec3f(10,10,10)};

    ofParameter<ofVec3f> noiseDisplacementScale {"scale",
        ofVec3f(1,1,1),
        ofVec3f(0,0,0),
        ofVec3f(100000,100000,100000)};

    ofParameter<ofVec3f> noiseDisplacementAmount {"amount",
        ofVec3f(0,0,0),
        ofVec3f(0,0,0),
        ofVec3f(10000,10000,10000)};
    
    ofParameterGroup noiseDisplacementParams {
        "displacement",
        noiseDisplacementVelocity,
        noiseDisplacementScale,
        noiseDisplacementAmount
        };
    
    // Parameters for light noise
    
    ofVec3f noiseLightTime {0,0,0};
    
    ofParameter<ofVec3f> noiseLightVelocity {"velocity",
        ofVec3f(0,0,0),
        ofVec3f(-10000,-10000,-10000),
        ofVec3f(10000,10000,10000)};
    
    ofParameter<float> noiseLightScale {"scale",
        10000,0,100000};
    
    ofParameter<float> noiseLightAmount {"amount", 0, 0, 1};

    ofParameter<ofVec3f> noiseLightAngle {"angle",
        ofVec3f(0,1,0),
        ofVec3f(-1,-1,-1),
        ofVec3f(1,1,1)};
    
    
    ofParameterGroup noiseLightParams {
        "light",
        noiseLightVelocity,
        noiseLightScale,
        noiseLightAmount,
        noiseLightAngle
    };
    
    // Parameters for texture noise
    
    ofVec3f noiseTextureTime {0,0,0};
    
    ofParameter<ofVec3f> noiseTextureVelocity {"velocity",
        ofVec3f(0,0,0),
        ofVec3f(-10000,-10000,-10000),
        ofVec3f(10000,10000,10000)};
    
    ofParameter<ofVec3f> noiseTextureScale {"scale",
        ofVec3f(1.6666,1.6666,1.6666),
        ofVec3f(0,0,0),
        ofVec3f(10000,10000,10000)};
    
    ofParameter<float> noiseTextureAmount {"amount", -0.05, -1, 1};
    
    ofParameterGroup noiseTextureParams {
        "texture",
        noiseTextureVelocity,
        noiseTextureScale,
        noiseTextureAmount
    };

    ofParameterGroup noiseParams {
        "noise",
        noiseDisplacementParams,
        noiseLightParams,
        noiseTextureParams
    };
    
    ofParameterGroup params {
        "material",
        noiseParams
    };
    
    OrganicMaterial();
    virtual ~OrganicMaterial(){};
    
    // set colors
    void setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor emissive);
    void setDiffuseColor(ofFloatColor oDiffuse);
    void setAmbientColor(ofFloatColor oAmbient);
    void setSpecularColor(ofFloatColor oSpecular);
    void setEmissiveColor(ofFloatColor oEmmisive);
    void setShininess(float nShininess);
    void setWorldMatrix(ofMatrix4x4 m);

    ofFloatColor getDiffuseColor() const;
    ofFloatColor getAmbientColor() const;
    ofFloatColor getSpecularColor() const;
    ofFloatColor getEmissiveColor() const;
    float getShininess() const;
    ofMatrix4x4 getWorldMatrix() const;
    
    struct Data{
        Data();
        ofFloatColor diffuse;
        ofFloatColor ambient;
        ofFloatColor specular;
        ofFloatColor emissive;
        float shininess;
    };
    Data getData() const;
    void setData(const OrganicMaterial::Data& data);
    
    // apply the material
    void begin() const;
    void end() const;
    
    
    void updateParameters();

    
private:
    void initShaders(ofGLProgrammableRenderer & renderer) const;
    const ofShader & getShader(int textureTarget, ofGLProgrammableRenderer & renderer) const;
    void updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
    void updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
    
    Data data;
    
    static ofShader shaderNoTexture;
    static ofShader shaderTexture2D;
    static ofShader shaderTextureRect;
    static bool shadersInitialized;
    static size_t shaderLights;
    
    static string vertexShader;
    static string fragmentShader;
    
    ofMatrix4x4 worldMatrix;
    
};