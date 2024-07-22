#pragma once

template <class Type>
struct node_t
{
	Type type;
};

using pnode_t = node_t*;