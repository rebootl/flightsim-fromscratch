#ifndef MODEL_H
#define MODEL_H

#include <osg/PositionAttitudeTransform>
#include <osg/Node>
#include <osg/NodeCallback>
#include <osg/NodeVisitor>

#include "FDMInterface.h"

class modelData : public osg::Referenced {
    public:
        modelData(osg::PositionAttitudeTransform* n, FDMInterface* f);
        void updateAttitude();
        void updatePosition();
    protected:
        osg::PositionAttitudeTransform* model_pat;
        FDMInterface* fdmi;
        osg::Vec3d position;
        //osg::Quat attitude;
};

class modelCallback : public osg::NodeCallback {
    public:
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
            osg::ref_ptr<modelData> md =
                dynamic_cast<modelData*> (node->getUserData());
            if (md) {
                md->updateAttitude();
                md->updatePosition();
        }
        traverse(node, nv);
    }
};

#endif
