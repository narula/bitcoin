// Copyright (c) 2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UNORDERED_INDIRECTMAP_H
#define BITCOIN_UNORDERED_INDIRECTMAP_H

#include <unordered_map>

template <class T>
struct DereferencingEqual { bool operator()(const T a, const T b) const { return *a == *b; } };
template <class T>
struct DereferencingHash  { std::size_t operator()(const T a) const { return (*a).GetCheapHash(); } };

/* Unordered map whose keys are pointers, but are compared by
 * their dereferenced values.
 *
 * See indirectmap.h for how this differs from a plain
 * std::unordered_multimap<const K*, T, DereferencingEqual<K*>
 * DereferencingHash<K*> >.
 *
 * Objects pointed to by keys must not be modified in any way that
 * changes the result of DereferencingEqual or DereferencingHash.
 */
template <class K, class T>
class unordered_indirectmap {
private:
    typedef std::unordered_map<const K*, T, DereferencingHash<const K*>, DereferencingEqual<const K*> > base;
    base m;
public:
    typedef typename base::iterator iterator;
    typedef typename base::const_iterator const_iterator;
    typedef typename base::size_type size_type;
    typedef typename base::value_type value_type;

    // passthrough (pointer interface)
    std::pair<iterator,bool> insert(const value_type& value) { return m.insert(value); }

    // pass address (value interface)
    iterator find(const K& key)                                               { return m.find(&key); }
    const_iterator find(const K& key) const                                   { return m.find(&key); }
    size_type erase(const K& key)                                             { return m.erase(&key); }
    iterator erase(iterator itr)                                              { return m.erase(itr); }
    size_type count(const K& key) const                                       { return m.count(&key); }

    // passthrough
    bool empty() const              { return m.empty(); }
    size_type size() const          { return m.size(); }
    size_type max_size() const      { return m.max_size(); }
    void clear()                    { m.clear(); }
    iterator begin()                { return m.begin(); }
    iterator end()                  { return m.end(); }
    const_iterator begin() const    { return m.begin(); }
    const_iterator end() const      { return m.end(); }
    const_iterator cbegin() const   { return m.cbegin(); }
    const_iterator cend() const     { return m.cend(); }
};

#endif // BITCOIN_UNORDERED_INDIRECTMAP_H
