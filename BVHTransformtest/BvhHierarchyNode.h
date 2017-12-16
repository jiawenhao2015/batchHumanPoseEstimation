#pragma once
#ifndef BVH_HIERARCHY_H
#define BVH_HIERARCHY_H

#include <string>
#include "BvhToken.h"
#define NODE_NAME_MAX 80

namespace BVHViewer_DataObjects
{
    using std::string;

    struct  BvhHierarchyNode
    {
        int SkeletonIndex;
        int NodeIndex;
        char NodeName[NODE_NAME_MAX + 1];
        int ParentIndex;
        int FirstChannelIndex;
        int ChannelCount;
        float Offset[3];

		BvhToken channels[9];

        BvhHierarchyNode(
            int skeletonIndex,
            int parentIndex)
        {
            SkeletonIndex = skeletonIndex;
            NodeIndex = -1;
            strncpy_s(NodeName, "<None>", NODE_NAME_MAX);
            ParentIndex = parentIndex;
            FirstChannelIndex = -1;
            ChannelCount = 0;

            Offset[0] = 0;
            Offset[1] = 0;
            Offset[2] = 0;

			for (int i=0;i<9;i++)
			{
				channels[i] = E_BVH_TOKEN_TKW_CHANNELS;
			}
			
        }

        // hierarchy root node has scene root (with index 0) as parent
        bool isRootNode() const
        {
            return ParentIndex == -1;
        }
    };
}

#endif