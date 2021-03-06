#include "kd_node.h"

KdNode KdNode::inner(float split_pos, AXIS split_axis, uint32_t lod_prim_offset)
{
    KdNode n;
    n.split_pos = split_pos;
    n.right_child = split_axis;
    n.lod_prims = lod_prim_offset;
    return n;
}

KdNode KdNode::leaf(uint32_t nprims, uint32_t prim_offset)
{
    KdNode n;
    n.prim_indices_offset = prim_offset;
    n.num_prims = 3 | (nprims << 2);
    n.lod_prims = -1;
    return n;
}

KdNode::KdNode(const KdNode &n)
{
    if (n.is_leaf()) {
        prim_indices_offset = n.prim_indices_offset;
        num_prims = n.num_prims;
    } else {
        split_pos = n.split_pos;
        right_child = n.right_child;
    }
    lod_prims = n.lod_prims;
}

KdNode::KdNode() : prim_indices_offset(0), num_prims(0), lod_prims(-1) {}

KdNode &KdNode::operator=(const KdNode &n)
{
    if (n.is_leaf()) {
        prim_indices_offset = n.prim_indices_offset;
        num_prims = n.num_prims;
    } else {
        split_pos = n.split_pos;
        right_child = n.right_child;
    }
    lod_prims = n.lod_prims;
    return *this;
}

void KdNode::set_right_child(uint32_t r)
{
    // Clear the previous right child bits before setting the value
    right_child = right_child & 0x3;
    right_child |= (r << 2);
}

uint32_t KdNode::get_num_prims() const
{
    return num_prims >> 2;
}

uint32_t KdNode::right_child_offset() const
{
    return right_child >> 2;
}

AXIS KdNode::split_axis() const
{
    return static_cast<AXIS>(num_prims & 3);
}

bool KdNode::is_leaf() const
{
    return (num_prims & 3) == 3;
}

bool KdNode::has_lod_prims() const
{
    return !is_leaf() && lod_prims != uint32_t(-1);
}

