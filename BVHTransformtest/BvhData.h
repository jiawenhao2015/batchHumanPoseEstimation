#ifndef BVH_DATA_H
#define BVH_DATA_H

#include <vector>
#include "BvhHierarchyNode.h"

namespace BVHViewer_DataObjects
{
    using std::vector;

    class BvhData
    {
    // ===========================================================
    // Constants
    // ===========================================================

    // ===========================================================
    // Fields
    // ===========================================================
    private:
        vector<BvhHierarchyNode> mBvhNodes;
        float *mMotionData;
        int mMotionDataStride;
        int mFrameCount;
        float mFrameTime;

    // ===========================================================
    // Constructors/Destructors
    // ===========================================================
    public:
        BvhData();
        ~BvhData();

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================
    private:
        void release();

    // ===========================================================
    // Inner and Anonymous Classes
    // ===========================================================
    
    // ===========================================================
    // Getter & Setter
    // ===========================================================
    public:
        vector<BvhHierarchyNode> getRootNodes() const;
        vector<BvhHierarchyNode> getChildNodes(int parentIndex) const;
        BvhHierarchyNode getNode(int nodeIndex) const;
        int getMaxParentIndex() const;
        void  setBvhNodes(const vector<BvhHierarchyNode> &bvhNodes) { mBvhNodes = bvhNodes; }

        float *getMotionData() const { return mMotionData; }
        void setMotionData(float *motionData) { mMotionData = motionData; }

        int getMotionDataStride() const { return mMotionDataStride; }
        void setMotionDataStride(int motionDataStride) { mMotionDataStride = motionDataStride; }

        int getMotionDataSize() const { return mMotionDataStride * mFrameCount; }

        int getFrameCount() const { return mFrameCount; }
        void setFrameCount(int frameCount) { mFrameCount = frameCount; }

        float getFrameTime() const { return mFrameTime; }
        void setFrameTime(float frameTime) { mFrameTime = frameTime; }

		int getNodesCount() const { return mBvhNodes.size();}
    };
}

#endif