/* -*- Mode: C++ -*-
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * UniConf low-level tree storage abstraction.
 */
#ifndef __UNICONFTREE_H
#define __UNICONFTREE_H

#include "uniconfkey.h"
#include "unihashtree.h"
#include "wvtr1.h"

/**
 * A recursively composed dictionary for tree-structured
 * data indexed by UniConfKey.
 *
 * Someday this could be further abstracted into a generic WvTreeDict.
 *
 * "Sub" is the name of the concrete subclass of UniConfTree.
 */
template<class Sub>
class UniConfTree : public UniHashTreeBase
{
   
public:
    typedef wv::function<void(const Sub*, void*)> Visitor;
    typedef wv::function<bool(const Sub*, const Sub*)> Comparator;

    /** Creates a node and links it to a subtree, if parent is non-NULL */
    UniConfTree(Sub *parent, const UniConfKey &key) :
        UniHashTreeBase(parent, key)
        { }

    /** Destroy this node's contents and children. */
    ~UniConfTree()
        { zap(); }

    /** Returns a pointer to the parent node, or NULL if there is none. */
    Sub *parent() const
        { return static_cast<Sub*>(this->xparent); }

    /** Reparents this node. */
    void setparent(Sub *parent)
        { UniHashTreeBase::_setparent(parent); }
    
    /** Returns a pointer to the root node of the tree. */
    Sub *root() const
        { return static_cast<Sub*>(UniHashTreeBase::_root()); }
    
    /**
     * Returns full path of this node relative to an ancestor.
     * If ancestor is NULL, returns the root.
     */
    UniConfKey fullkey(const Sub *ancestor = NULL) const
        { return UniHashTreeBase::_fullkey(ancestor); }

    /**
     * Finds the sub-node with the specified key.
     * If key.isempty(), returns this node.
     */
    Sub *find(const UniConfKey &key) const
        { return static_cast<Sub*>(UniHashTreeBase::_find(key)); }
    
    /**
     * Finds the direct child node with the specified key.
     *
     * If key.numsegments() == 1, then performs the same task
     * as find(key), but a little faster.  Otherwise returns NULL.
     */
    Sub *findchild(const UniConfKey &key) const
        { return static_cast<Sub*>(UniHashTreeBase::_findchild(key)); }

    /**
     * Removes the node for the specified key from the tree
     * and deletes it along with any of its children.
     *
     * If the key is UniConfKey::EMPTY, deletes this object.
     */
    void remove(const UniConfKey &key)
        { delete find(key); }
    
    /** Removes and deletes all children of this node. */
    void zap()
    {
        if (!(this->xchildren))
            return;
        // set xchildren to NULL first so that the zap() will happen faster
        // otherwise, each child will attempt to unlink itself uselessly

        typename UniHashTreeBase::Container *oldchildren = this->xchildren;
        this->xchildren = NULL;

        // delete all children
        typename UniHashTreeBase::Container::Iter i(*oldchildren);
        for (i.rewind(); i.next();)
            delete static_cast<Sub*>(i.ptr());

        delete oldchildren;
    }

    /**
     * Performs a traversal on this tree using the specified
     * visitor function and traversal type(s).
     * "visitor" is the tree visitor function
     * "userdata" is userdata for the tree visitor function
     */
    void visit(const Visitor &visitor, void *userdata,
        bool preorder = true, bool postorder = false) const
    {
        _recursive_unsorted_visit(this, reinterpret_cast<
            const typename UniHashTreeBase::BaseVisitor&>(visitor), userdata,
            preorder, postorder);
    }

    /**
     * Compares this tree with another using the specified comparator
     * function.
     * Comparison of a subtree ends when the comparator returns false.
     * "comparator" is the value compare function
     * "userdata" is userdata for the compare function
     * Returns: true if the comparison function returned true each time
     */
    bool compare(const Sub *other, const Comparator &comparator)
    {
        return _recursivecompare(this, other, reinterpret_cast<
            const typename UniHashTreeBase::BaseComparator&>(comparator));
    }

    /**
     * An iterator that walks over all elements on one level of a
     * UniConfTree.
     */
    class Iter : public UniHashTreeBase::Iter
    {
    public:
        typedef typename UniHashTreeBase::Iter MyBase;

        /** Creates an iterator over the specified tree. */
        Iter(Sub &tree) : UniHashTreeBase::Iter(tree)
	    { }

        /** Returns a pointer to the current node. */
        Sub *ptr() const
            { return static_cast<Sub*>(MyBase::ptr()); }
        WvIterStuff(Sub);
    };
};


/** A plain UniConfTree that holds keys and values. */
class UniConfValueTree : public UniConfTree<UniConfValueTree>
{
    WvString xvalue;  /*!< the value of this entry */
    
public:
    UniConfValueTree(UniConfValueTree *parent,
		     const UniConfKey &key, WvStringParm value)
	: UniConfTree<UniConfValueTree>(parent, key), xvalue(value)
	{ }
    
    /** Returns the value field. */
    const WvString &value() const
        { return xvalue; }

    /** Sets the value field. */
    void setvalue(WvStringParm value)
        { xvalue = value; }
};


#endif // __UNICONFTREE_H
