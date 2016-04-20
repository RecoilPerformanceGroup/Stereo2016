//
//  SoundField.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/3/16.
//
//

#include "SoundField.hpp"
#include <dispatch/dispatch.h>

float smootherstep(float x, float edge0, float edge1 )
{
    float sign = ofSign(x);
    // Scale, and clamp x to 0..1 range
    x = ofClamp((fabs(x) - edge0)/(edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return fabs(x*x*x*(x*(x*6 - 15) + 10))*sign;
}

void SoundField::setup() {
    clusterNumCells.addListener(this, &SoundField::reconstructCluster<int>);
    reconstructCluster();
    
    voroEnd = std::static_pointer_cast<VoroEnd>(getScene("VoroEnd"));

    

    
}

void SoundField::draw() {
    ofEnableAlphaBlending();
    ofSetColor(255);
    matCluster.begin();
    cluster.draw(&matCluster);
    matCluster.end();
}

void SoundField::update() {
    
    if(soundWaveCreate){
        soundWave sw;
        sw.type = soundWaveType;
        sw.position = sw.origin = soundWaveOriginNormalised.get() * getWorldSize();
        sw.velocity = soundWaveVelocity * soundWaveDirection.get().getNormalized();
        sw.force = soundWaveForce;
        soundWaves.push_back(sw);
        soundWaveCreate.set(false);
    }
    
    if(soundWaveClear){
        soundWaves.clear();
        soundWaveClear.set(false);
    }
    
    matCluster.updateParameters();
    
    clusterRotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
                                                             clusterRotationAxis.get().x*
                                                             clusterRotationSpeed.get(),
                                                             clusterRotationAxis.get().y*
                                                             clusterRotationSpeed.get(),
                                                             clusterRotationAxis.get().z
                                                             *clusterRotationSpeed.get());
    
    cluster.setOrientation(clusterRotation);
    cluster.setGlobalPosition(clusterOrigin);
    cluster.setScale(clusterScale.get());

    std::vector<soundWave>::iterator sw;
    for (sw = soundWaves.begin(); sw != soundWaves.end(); ) {
        sw->position += sw->velocity * ofGetLastFrameTime() * 60.0;
        if(sw->position.distance(world->origin.getPosition()) > 4000.0){
            sw = soundWaves.erase(sw);
        } else {
            ++sw;
        }
    }
    
    applyWaves(cluster);
    
    for(auto c : cluster.getChildren()) {
        c->setScale(clusterScaleCells);
        for(auto cc : c->getChildren()) {
            cc->setScale(clusterScaleCells);
        }
    }
    
    matCluster.setDiffuseColor(clusterColor.get());
    
    for(auto c : cluster.getChildren()) {
        c->setScale(clusterScaleCells);
        for(auto cc : c->getChildren()) {
            cc->setScale(clusterScaleCells);
        }
    }
    
    int nI = 0;
    int nII = 0;
    for( auto c : cluster.getChildren() ) {
        
        //c->setScale(1-fadeIn);
        
        if(nI < voroEnd->wall.getChildren().size()) {
            
            // + c->renderPosOffset) * c->getGlobalTransformMatrix()
            
            voroEnd->wall.getChildren()[nI]->setGlobalPosition(
                                        c->getGlobalPosition().getInterpolated(
                                                voroEnd->wall.getChildren()[nI]->origin, 1-fadeIn) );
            
            //voroEnd->wall.getChildren()[nI]->setScale(1-fadeIn);
            //voroEnd->wall.getChildren()[nI]->setTint(ofFloatColor(1-fadeIn, 0));
            
            nI++;
            
        } else {
            
            if(nII < voroEnd->oceanHorizon.getChildren().size()) {
                
                // + c->renderPosOffset) * c->getGlobalTransformMatrix()
                
                voroEnd->oceanHorizon.getChildren()[nII]->setGlobalPosition(
                                                                   c->getGlobalPosition().getInterpolated(
                                                                                                          voroEnd->oceanHorizon.getChildren()[nII]->origin, 1-fadeIn) );

                nII++;
        
            
            }
        }
        
        
        
        
        
        
    }
    
    /*
    for( auto c : voroEnd->oceanHorizon.getChildren() ) {
        
        //c->setScale(1-fadeIn);
        
        if(nI < cluster.getChildren().size()) {
            cluster.getChildren()[nI]->setGlobalPosition( (c->getPosition() + c->renderPosOffset) * c->getGlobalTransformMatrix()  );
        }
        nI++;
    
    }*/
    
    

}

void SoundField::applyWaves(VoroNode & vn, bool recursive){
    
    vector<VoroNode *> vnChildren = vn.getChildren();
    
    dispatch_apply( vnChildren.size(), dispatch_get_global_queue(QOS_CLASS_USER_INTERACTIVE, 0), ^(size_t childNumber){
        VoroNode * n = vnChildren[childNumber];
        float maxDist = soundWaveAffectDistance;
        std::vector<soundWave>::iterator sw;
        for (sw = soundWaves.begin(); sw != soundWaves.end(); ) {
            ofVec3f normal = sw->velocity.getNormalized();
            float sphereRadius = 0;
            if(sw->type == 1){
                sphereRadius = sw->origin.distance(sw->position);
            }
                if(sw->type == 0){
                    float distToPlane(normal.dot(n->getGlobalPosition() - sw->position));
                    distToPlane = ofClamp(distToPlane, -maxDist, maxDist);
                    distToPlane = (maxDist - fabs(distToPlane)) * ofSign(distToPlane);
                    distToPlane = smootherstep(ofMap(distToPlane, -maxDist, maxDist, -1, 1, true), 0.0, 1.0);
                    ofVec3f offsetVec(sw->velocity.getNormalized() * sw->force*soundWaveGlobalForce*distToPlane);
                    n->setGlobalPosition(n->getGlobalPosition()+offsetVec);
                } else if (sw->type == 1) {
                    float distToOrigin(sw->origin.distance(n->getGlobalPosition()));
                    float distToWave = ofClamp(distToOrigin-sphereRadius, -maxDist, maxDist);
                    distToWave = (maxDist - fabs(distToWave)) * ofSign(distToWave);
                    distToWave = smootherstep(ofMap(distToWave, -maxDist, maxDist, -1, 1, true), 0.0, 1.0);
                    ofVec3f offsetVec((n->getGlobalPosition()-sw->origin).getNormalized()*sw->force*soundWaveGlobalForce* distToWave);
                    n->setGlobalPosition(n->getGlobalPosition()+offsetVec);
                }
            sw++;
        }
        if(recursive){
            for(auto nc : n->getChildren()) {
                applyWaves(*nc, recursive);
            }
        }
    });
}

void SoundField::reconstructCluster(){
    ofSeedRandom();
    cluster.setupFromBoundingBox(200,200,200,clusterNumCells, true, true, true);
    cluster.setGlobalPosition(clusterOrigin);
}

void SoundField::drawModel() {
    ofSetColor(255,15);
    cluster.draw();
    for(auto sw : soundWaves){
        if(sw.type == 0){
            ofPlanePrimitive p;
            p.set(1000,1000, 3, 3);
            p.setParent(world->origin);
            p.setGlobalPosition(sw.position);
            p.lookAt(sw.position+sw.velocity, ofVec3f(0,0,1));
            p.draw();
        }
        if(sw.type == 1){
            ofDrawSphere(sw.origin, sw.origin.distance(sw.position));
        }
    }
}


