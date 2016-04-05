//
//  OrganicMaterial.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/11/16.
//
//

#include "OrganicMaterial.hpp"


#include "ofConstants.h"
#include "ofGLUtils.h"
#include "ofLight.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"

ofShader OrganicMaterial::shaderNoTexture;
ofShader OrganicMaterial::shaderTexture2D;
ofShader OrganicMaterial::shaderTextureRect;
bool OrganicMaterial::shadersInitialized = false;
size_t OrganicMaterial::shaderLights = 0;

static string vertexSource(string defaultHeader, int maxLights, bool hasTexture);
static string fragmentSource(string defaultHeader, int maxLights, bool hasTexture);

OrganicMaterial::Data::Data()
:diffuse(0.8f, 0.8f, 0.8f, 1.0f)
,ambient(0.2f, 0.2f, 0.2f, 1.0f)
,specular(0.0f, 0.0f, 0.0f, 1.0f)
,emissive(0.0f, 0.0f, 0.0f, 1.0f)
,shininess(0.2f){
    
}

OrganicMaterial::OrganicMaterial() {
}

void OrganicMaterial::setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive) {
    setDiffuseColor(oDiffuse);
    setAmbientColor(oAmbient);
    setSpecularColor(oSpecular);
    setEmissiveColor(oEmissive);
}

void OrganicMaterial::setDiffuseColor(ofFloatColor oDiffuse) {
    data.diffuse = oDiffuse;
}

void OrganicMaterial::setAmbientColor(ofFloatColor oAmbient) {
    data.ambient = oAmbient;
}

void OrganicMaterial::setSpecularColor(ofFloatColor oSpecular) {
    data.specular = oSpecular;
}

void OrganicMaterial::setEmissiveColor(ofFloatColor oEmissive) {
    data.emissive = oEmissive;
}

void OrganicMaterial::setShininess(float nShininess) {
    data.shininess = nShininess;
}

void OrganicMaterial::setWorldMatrix(ofMatrix4x4 m){
    worldMatrix = m;
    // a hack as we want to set the matrix dynamically
    shaderNoTexture.setUniformMatrix4f("worldMatrix", worldMatrix);
    shaderTexture2D.setUniformMatrix4f("worldMatrix", worldMatrix);
    shaderTextureRect.setUniformMatrix4f("worldMatrix", worldMatrix);
}

void OrganicMaterial::setData(const OrganicMaterial::Data &data){
    this->data = data;
}

float OrganicMaterial::getShininess()const{
    return data.shininess;
}

ofFloatColor OrganicMaterial::getDiffuseColor()const {
    return data.diffuse;
}

ofFloatColor OrganicMaterial::getAmbientColor()const {
    return data.ambient;
}

ofFloatColor OrganicMaterial::getSpecularColor()const {
    return data.specular;
}

ofFloatColor OrganicMaterial::getEmissiveColor()const {
    return data.emissive;
}

ofMatrix4x4 OrganicMaterial::getWorldMatrix()const {
    return worldMatrix;
}

OrganicMaterial::Data OrganicMaterial::getData()const{
    return data;
}

void OrganicMaterial::begin() const{
    if(ofGetGLRenderer()){
        ofGetGLRenderer()->bind(*this);
    }
}

void OrganicMaterial::end() const{
    if(ofGetGLRenderer()){
        ofGetGLRenderer()->unbind(*this);
    }
}

void OrganicMaterial::initShaders(ofGLProgrammableRenderer & renderer) const{
    if(!shadersInitialized || ofLightsData().size()!=shaderLights){
#ifndef TARGET_OPENGLES
        string vertexRectHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_RECTANGLE);
        string fragmentRectHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_RECTANGLE);
#endif
        string vertex2DHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_2D);
        string fragment2DHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_2D);
        shaderLights = ofLightsData().size();
        shaderNoTexture.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,shaderLights,false));
        shaderNoTexture.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,shaderLights,false));
        shaderNoTexture.bindDefaults();
        shaderNoTexture.linkProgram();
        
        shaderTexture2D.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,shaderLights,true));
        shaderTexture2D.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,shaderLights,true));
        shaderTexture2D.bindDefaults();
        shaderTexture2D.linkProgram();
        
#ifndef TARGET_OPENGLES
        shaderTextureRect.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertexRectHeader,shaderLights,true));
        shaderTextureRect.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragmentRectHeader,shaderLights,true));
        shaderTextureRect.bindDefaults();
        shaderTextureRect.linkProgram();
#endif
        shadersInitialized = true;
    }
}

const ofShader & OrganicMaterial::getShader(int textureTarget, ofGLProgrammableRenderer & renderer) const{
    initShaders(renderer);
    switch(textureTarget){
        case OF_NO_TEXTURE:
            return shaderNoTexture;
            break;
        case GL_TEXTURE_2D:
            return shaderTexture2D;
            break;
        default:
            return shaderTextureRect;
            break;
    }
}

void OrganicMaterial::updateParameters() {
    noiseDisplacementTime += noiseDisplacementVelocity.get() * ofGetLastFrameTime();
    noiseLightTime += noiseLightVelocity.get() * ofGetLastFrameTime();
    noiseTextureTime += noiseTextureVelocity.get() * ofGetLastFrameTime();
}

void OrganicMaterial::updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
    
    shader.setUniform4fv("mat_ambient", &data.ambient.r);
    shader.setUniform4fv("mat_diffuse", &data.diffuse.r);
    shader.setUniform4fv("mat_specular", &data.specular.r);
    shader.setUniform4fv("mat_emissive", &data.emissive.r);
    shader.setUniform4fv("global_ambient", &ofGetGlobalAmbientColor().r);
    shader.setUniform1f("mat_shininess",data.shininess);
    
    // generic actual time
    shader.setUniform1f("time", ofGetElapsedTimef());
    
    // world matrix
    shader.setUniformMatrix4f("worldMatrix", worldMatrix);

    // noise displacement uniforms
    shader.setUniform3f("noiseDisplacementTime", noiseDisplacementTime);
    shader.setUniform3f("noiseDisplacementScale", noiseDisplacementScale);
    shader.setUniform3f("noiseDisplacementAmount", noiseDisplacementAmount);

    // noise light uniforms
    shader.setUniform3f("noiseLightTime", noiseLightTime);
    shader.setUniform1f("noiseLightScale", noiseLightScale);
    shader.setUniform1f("noiseLightAmount", noiseLightAmount);
    shader.setUniform3f("noiseLightAngle", noiseLightAngle);
    
    // noise texture uniforms
    shader.setUniform3f("noiseTextureTime", noiseTextureTime);
    shader.setUniform3f("noiseTextureScale", noiseTextureScale);
    shader.setUniform1f("noiseTextureAmount", noiseTextureAmount);
    
}

void OrganicMaterial::updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
    for(size_t i=0;i<ofLightsData().size();i++){
        string idx = ofToString(i);
        shared_ptr<ofLight::Data> light = ofLightsData()[i].lock();
        if(!light || !light->isEnabled){
            shader.setUniform1f("lights["+idx+"].enabled",0);
            continue;
        }
        ofVec4f lightEyePosition = light->position * renderer.getCurrentViewMatrix();
        shader.setUniform1f("lights["+idx+"].enabled",1);
        shader.setUniform1f("lights["+idx+"].type", light->lightType);
        shader.setUniform4f("lights["+idx+"].position", lightEyePosition);
        shader.setUniform4f("lights["+idx+"].ambient", light->ambientColor);
        shader.setUniform4f("lights["+idx+"].specular", light->specularColor);
        shader.setUniform4f("lights["+idx+"].diffuse", light->diffuseColor);
        
        if(light->lightType!=OF_LIGHT_DIRECTIONAL){
            shader.setUniform1f("lights["+idx+"].constantAttenuation", light->attenuation_constant);
            shader.setUniform1f("lights["+idx+"].linearAttenuation", light->attenuation_linear);
            shader.setUniform1f("lights["+idx+"].quadraticAttenuation", light->attenuation_quadratic);
        }
        
        if(light->lightType==OF_LIGHT_SPOT){
            ofVec3f direction = light->position + light->direction;
            direction = direction * renderer.getCurrentViewMatrix();
            direction = direction - lightEyePosition;
            shader.setUniform3f("lights["+idx+"].spotDirection", direction.normalize());
            shader.setUniform1f("lights["+idx+"].spotExponent", light->exponent);
            shader.setUniform1f("lights["+idx+"].spotCutoff", light->spotCutOff);
            shader.setUniform1f("lights["+idx+"].spotCosCutoff", cos(ofDegToRad(light->spotCutOff)));
        } else if(light->lightType==OF_LIGHT_DIRECTIONAL){
            ofVec3f halfVector = (ofVec3f(0,0,1) + lightEyePosition).getNormalized();
            shader.setUniform3f("lights["+idx+"].halfVector", halfVector);
        } else if(light->lightType==OF_LIGHT_AREA){
            shader.setUniform1f("lights["+idx+"].width", light->width);
            shader.setUniform1f("lights["+idx+"].height", light->height);
            ofVec3f direction = light->position + light->direction;
            direction = direction * renderer.getCurrentViewMatrix();
            direction = direction - lightEyePosition;
            shader.setUniform3f("lights["+idx+"].spotDirection", direction.normalize());
            ofVec3f right = light->position + light->right;
            right = right * renderer.getCurrentViewMatrix();
            right = right - lightEyePosition;
            ofVec3f up = right.getCrossed(direction);
            shader.setUniform3f("lights["+idx+"].right", right.normalize());
            shader.setUniform3f("lights["+idx+"].up", up.normalize());
        }
    }
}

static const string vertexShader = R"(

const float PI = 3.14159265359;

OUT vec2 outtexcoord; // pass the texCoord if needed
OUT vec3 transformedNormal;
OUT vec3 eyePosition3;
OUT vec4 worldPosition;
OUT vec4 modelViewPosition;
OUT vec4 localPosition;
OUT vec4 worldPositionBeforeNoise;
OUT vec4 modelViewPositionBeforeNoise;
OUT vec4 localPositionBeforeNoise;

IN vec4 position;
IN vec4 color;
IN vec4 normal;
IN vec2 texcoord;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

// actual time
uniform float time;

// world matrix
uniform mat4 worldMatrix;

// noise displacement
uniform vec3 noiseDisplacementTime;
uniform vec3 noiseDisplacementScale;
uniform vec3 noiseDisplacementAmount;

// Noise 2D
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise

vec3 mod289(vec3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
    return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,  // -1.0 + 2.0 * C.x
                        0.024390243902439); // 1.0 / 41.0
    // First corner
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);
    
    // Other corners
    vec2 i1;
    //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
    //i1.y = 1.0 - i1.x;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    // x0 = x0 - 0.0 + 0.0 * C.xx ;
    // x1 = x0 - i1 + 1.0 * C.xx ;
    // x2 = x0 - 1.0 + 2.0 * C.xx ;
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    
    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
                     + i.x + vec3(0.0, i1.x, 1.0 ));
    
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;
    
    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
    
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    
    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
    
    // Compute final noise value at P
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}


vec4 noiseDisplace (vec4 pos, vec3 t, vec3 scale, vec3 amount) {
    return vec4(snoise(vec2(pos.x / scale.x, t.x)) * amount.x,
                snoise(vec2(pos.y / scale.y, t.y)) * amount.y,
                snoise(vec2(pos.z / scale.z, t.z)) * amount.z, 0.0);
}

void main (void){
    
    localPositionBeforeNoise = position;
    modelViewPositionBeforeNoise = modelViewMatrix * position;
    worldPositionBeforeNoise = worldMatrix * position;
    
    vec4 displacementVec = noiseDisplace(worldPositionBeforeNoise, noiseDisplacementTime, noiseDisplacementScale, noiseDisplacementAmount);
    
    localPosition = position + displacementVec;
    worldPosition = worldMatrix * localPosition;
    modelViewPosition = modelViewMatrix * localPosition;
    
    vec4 modifiedPosition = modelViewProjectionMatrix * localPosition;
    
    vec4 eyePosition = modelViewMatrix * modifiedPosition;
    vec3 tempNormal = (normalMatrix * normal).xyz;
    transformedNormal = normalize(tempNormal);
    eyePosition3 = (eyePosition.xyz) / eyePosition.w;
    
    outtexcoord = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
    gl_Position = modifiedPosition;
}
)";

static const string fragmentShader = R"(
IN vec2 outtexcoord; // pass the texCoord if needed
IN vec3 transformedNormal;
// Eye-coordinate position of vertex
IN vec3 eyePosition3;
IN vec4 worldPosition;
IN vec4 modelViewPosition;
IN vec4 localPosition;

IN vec4 worldPositionBeforeNoise;
IN vec4 modelViewPositionBeforeNoise;
IN vec4 localPositionBeforeNoise;

struct lightData
{
    float enabled;
    vec4 ambient;
    float type; // 0 = pointlight 1 = directionlight
    vec4 position; // where are we
    vec4 diffuse; // how diffuse
    vec4 specular; // what kinda specular stuff we got going on?
    // attenuation
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    // only for spot
    float spotCutoff;
    float spotCosCutoff;
    float spotExponent;
    // spot and area
    vec3 spotDirection;
    // only for directional
    vec3 halfVector;
    // only for area
    float width;
    float height;
    vec3 right;
    vec3 up;
};

uniform SAMPLER tex0;

uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform vec4 mat_emissive;
uniform float mat_shininess;
uniform vec4 global_ambient;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform lightData lights[MAX_LIGHTS];

// actual time
uniform float time;

// world matrix
uniform mat4 worldMatrix;

// noise light
uniform vec3 noiseLightTime;
uniform float noiseLightScale;
uniform float noiseLightAmount;
uniform vec3 noiseLightAngle;

// noise texture
uniform vec3 noiseTextureTime;
uniform vec3 noiseTextureScale;
uniform float noiseTextureAmount;

void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
    float nDotVP;       // normal . light direction
    float nDotHV;       // normal . light half vector
    float pf;           // power factor
    float attenuation;  // computed attenuation factor
    float d;            // distance from surface to light source
    vec3  VP;           // direction from surface to light position
    vec3  halfVector;   // direction of maximum highlights
    vec3 eye = vec3 (0.0, 0.0, 1.0);
    
    // Compute vector from surface to light position
    VP = vec3 (light.position.xyz) - ecPosition3;
    
    // Compute distance between surface and light position
    d = length(VP);
    
    // Compute attenuation
    attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
    
    // Normalize the vector from surface to light position
    VP = normalize(VP);
    halfVector = normalize(VP + eye);
    nDotHV = max(0.0, dot(normal, halfVector));
    nDotVP = max(0.0, dot(normal, VP));
    
    ambient += light.ambient.rgb * attenuation;
    diffuse += light.diffuse.rgb * nDotVP * attenuation;
#ifndef TARGET_OPENGLES
#define SPECULAR_REFLECTION
#endif
#ifndef SPECULAR_REFLECTION
    // ha! no branching :)
    pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));
    specular += light.specular.rgb * pf * nDotVP * attenuation;
#else
    // fresnel factor
    // http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes
    float w = pow(1.0 - max(0.0, dot(halfVector, VP)), 5.0);
    vec3 specularReflection = attenuation * vec3(light.specular.rgb)
		  * mix(vec3(mat_specular.rgb), vec3(1.0), w)
		  * pow(nDotHV, mat_shininess);
    specular += mix(vec3(0.0), specularReflection, step(0.0000001, nDotVP));
#endif
}

void directionalLight(in lightData light, in vec3 normal, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
    float nDotVP;         // normal . light direction
    float nDotHV;         // normal . light half vector
    float pf;             // power factor
    
    nDotVP = max(0.0, dot(normal, normalize(vec3(light.position))));
    nDotHV = max(0.0, dot(normal, light.halfVector));
    
    pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));
    
    ambient += light.ambient.rgb;
    diffuse += light.diffuse.rgb * nDotVP;
    specular += light.specular.rgb * pf * nDotVP;
}

void spotLight(in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
    float nDotVP; // = max(dot(normal,normalize(vec3(light.position))),0.0);
    float nDotHV;       // normal . light half vector
    float pf;
    float d;            // distance from surface to light source
    vec3  VP;           // direction from surface to light position
    vec3 eye = vec3 (0.0, 0.0, 1.0);
    float spotEffect;
    float attenuation=1.0;
    vec3  halfVector;   // direction of maximum highlights
    // Compute vector from surface to light position
    VP = light.position.xyz - ecPosition3;
    spotEffect = dot(light.spotDirection, -normalize(VP));
    
    if (spotEffect > light.spotCosCutoff) {
        // Compute distance between surface and light position
        d = length(VP);
        spotEffect = pow(spotEffect, light.spotExponent);
        attenuation = spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
        
        VP = normalize(VP);
        halfVector = normalize(VP + eye);
        nDotHV = max(0.0, dot(normal, halfVector));
        nDotVP = max(0.0, dot(normal, VP));
        
        pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));
        
        diffuse += light.diffuse.rgb * nDotVP * attenuation;
        specular += light.specular.rgb * pf * nDotVP * attenuation;
        
    }
    
    ambient += light.ambient.rgb * attenuation;
    
}


vec3 projectOnPlane(in vec3 point, in vec3 planeCenter, in vec3 planeNormal){
    return point - dot( point - planeCenter, planeNormal ) * planeNormal;
}

vec3 linePlaneIntersect(in vec3 lp, in vec3 lv, in vec3 pc, in vec3 pn){
	   return lp+lv*(dot(pn,pc-lp)/dot(pn,lv));
}

void areaLight(in lightData light, in vec3 N, in vec3 V, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
    vec3 right = light.right;
    vec3 pnormal = light.spotDirection;
    vec3 up = light.up;
    
    //width and height of the area light:
    float width = light.width*0.5;
    float height = light.height*0.5;
    float attenuation;
    
    //project onto plane and calculate direction from center to the projection.
    vec3 projection = projectOnPlane(V,light.position.xyz,pnormal);// projection in plane
    vec3 dir = projection-light.position.xyz;
    
    //calculate distance from area:
    vec2 diagonal = vec2(dot(dir,right),dot(dir,up));
    vec2 nearest2D = vec2(clamp( diagonal.x,-width,width  ),clamp(  diagonal.y,-height,height));
    vec3 nearestPointInside = vec3(light.position.xyz)+(right*nearest2D.x+up*nearest2D.y);
    float dist = distance(V,nearestPointInside);//real distance to area rectangle
    
    vec3 lightDir = normalize(nearestPointInside - V);
    attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * dist + light.quadraticAttenuation * dist * dist);
    
    float NdotL = max( dot( pnormal, -lightDir ), 0.0 );
    float NdotL2 = max( dot( N, lightDir ), 0.0 );
    if ( NdotL * NdotL2 > 0.0 ) {
        float diffuseFactor = sqrt( NdotL * NdotL2 );
        vec3 R = reflect( normalize( -V ), N );
        vec3 E = linePlaneIntersect( V, R, light.position.xyz, pnormal );
        float specAngle = dot( R, pnormal );
        if (specAngle > 0.0){
            vec3 dirSpec = E - light.position.xyz;
            vec2 dirSpec2D = vec2(dot(dirSpec,right),dot(dirSpec,up));
            vec2 nearestSpec2D = vec2(clamp( dirSpec2D.x,-width,width  ),clamp(  dirSpec2D.y,-height,height));
            float specFactor = 1.0-clamp(length(nearestSpec2D-dirSpec2D) * 0.05 * mat_shininess,0.0,1.0);
            specular += light.specular.rgb * specFactor * specAngle * diffuseFactor * attenuation;
        }
        diffuse  += light.diffuse.rgb  * diffuseFactor * attenuation;
    }
    ambient += light.ambient.rgb * attenuation;
}

vec3 mod289(vec3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
    return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{
    const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
    
    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;
    
    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );
    
    //   x0 = x0 - 0.0 + 0.0 * C.xxx;
    //   x1 = x0 - i1  + 1.0 * C.xxx;
    //   x2 = x0 - i2  + 2.0 * C.xxx;
    //   x3 = x0 - 1.0 + 3.0 * C.xxx;
    vec3 x1 = x0 - i1 + C.xxx;
    vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
    vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y
    
    // Permutations
    i = mod289(i);
    vec4 p = permute( permute( permute(
                                       i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
                              + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
                     + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
    
    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float n_ = 0.142857142857; // 1.0/7.0
    vec3  ns = n_ * D.wyz - D.xzx;
    
    vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)
    
    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)
    
    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);
    
    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );
    
    //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
    //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));
    
    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
    
    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);
    
    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;
    
    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                 dot(p2,x2), dot(p3,x3) ) );
}



//////////////////////////////////////////////////////
// here's the main method
//////////////////////////////////////////////////////


void main (void){
    
    vec3 ambient = global_ambient.rgb;
    vec3 diffuse = vec3(0.0,0.0,0.0);
    vec3 specular = vec3(0.0,0.0,0.0);
    
    for( int i = 0; i < MAX_LIGHTS; i++ ){
        if(lights[i].enabled<0.5) continue;
        if(lights[i].type<0.5){
            pointLight(lights[i], transformedNormal, eyePosition3, ambient, diffuse, specular);
        }else if(lights[i].type<1.5){
            directionalLight(lights[i], transformedNormal, ambient, diffuse, specular);
        }else if(lights[i].type<2.5){
            spotLight(lights[i], transformedNormal, eyePosition3, ambient, diffuse, specular);
        }else{
            areaLight(lights[i], transformedNormal, eyePosition3, ambient, diffuse, specular);
        }
    }
    
    
    ////////////////////////////////////////////////////////////
    // now add the material info
#ifdef HAS_TEXTURE
    vec4 tex = TEXTURE(tex0, outtexcoord);
    vec4 localColor = vec4(ambient,1.0) * mat_ambient + vec4(diffuse,1.0) * tex + vec4(specular,1.0) * mat_specular + mat_emissive;
#else
    vec4 localColor = vec4(ambient,1.0) * mat_ambient + vec4(diffuse,1.0) * mat_diffuse + vec4(specular,1.0) * mat_specular + mat_emissive;
#endif
    
    
    ////////////////////////////////////////////////////////////
    // fragment noise
    
/*    float noiseLightComponent = (0.5+snoise(
                                 vec3(
                                      worldPositionBeforeNoise.x,
                                      worldPositionBeforeNoise.y,
                                      worldPositionBeforeNoise.z,
                                      
                                            )
                                            )*0.5)*0.1
    
*/
    float textureNoise = (0.5+snoise(((worldPositionBeforeNoise.xyz-noiseTextureTime)/noiseTextureScale))*0.5)*noiseTextureAmount;
    
    float lightNoise = 1.0+(noiseLightAmount*snoise(((worldPosition.xyz * noiseLightAngle)-noiseLightTime)/noiseLightScale));
    
    localColor.rgb += textureNoise;
    localColor.rgb *= lightNoise;
 //   localColor.rgb *= 1.0+(0.25*snoise(vec3(worldPosition.xzz/10000.0)+vec3(noiseDisplacementTime.x,noiseDisplacementTime.y,noiseDisplacementTime.z)));
    
    //localColor.rgb = (mod((worldPosition.z)+(snoise(vec3(time, 0, 0)))*300, 200) > 180.0)?vec3(0,0,0):transformedNormal;
    
     FRAG_COLOR = clamp( localColor, 0.0, 1.0 );
}
)";


static string shaderHeader(string header, int maxLights, bool hasTexture){
    header += "#define MAX_LIGHTS " + ofToString(max(1,maxLights)) + "\n";
    if(hasTexture){
        header += "#define HAS_TEXTURE\n";
    }
    return header;
}

static string vertexSource(string defaultHeader, int maxLights, bool hasTexture){
    return shaderHeader(defaultHeader,maxLights,hasTexture) + vertexShader;
}

static string fragmentSource(string defaultHeader, int maxLights, bool hasTexture){
    return shaderHeader(defaultHeader,maxLights,hasTexture) + fragmentShader;
}
