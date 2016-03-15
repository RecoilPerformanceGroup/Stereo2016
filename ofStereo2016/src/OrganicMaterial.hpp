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
    
    // TODO overwirte the shaders with vertex displace methods and more set uniforms to use them
    
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
        ofVec3f distortion;
    };
    Data getData() const;
    void setData(const OrganicMaterial::Data& data);
    
    // apply the material
    void begin() const;
    void end() const;
    
    
    
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