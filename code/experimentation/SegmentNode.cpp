/**
 * \file SegmentNode.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include "SegmentNode.h"


/** Truncates Data from left 
 * \param cutSize amount to cut off the left of current segment 
 **/
void SegmentNode::TruncateLeft(size_t cutSize)
{
    Start = Start+cutSize;
    Size -= cutSize;
    Weight -= (int)cutSize;
}

/** Truncates Data from right 
 * \param cutSize amount to cut off the right of current segment 
 **/
void SegmentNode::TruncateRight(size_t cutSize)
{
    Size -= cutSize;
    Weight -= (int)cutSize;
}


/** Subdivide segment at index
 * \param index Index to cut the segment
 * \return pair of two new SegmentNodes to the cut segments 
 **/
SegmentNode* SegmentNode::Cut(size_t index)
{
    SegmentNode* cutSegment = new SegmentNode(Data, Start+index, Size-index);
    TruncateRight(Size-index);

    return cutSegment;
}
