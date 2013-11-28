#ifndef MARSYAS_SCRIPT_SYNTAX_TREE_INCLUDED
#define MARSYAS_SCRIPT_SYNTAX_TREE_INCLUDED

#include <vector>

namespace Marsyas {

enum node_tag
{
  GENERIC_NODE,
  ACTOR_NODE,
  PROTOTYPE_NODE,
  CONTROL_NODE,

  BOOL_NODE,
  INT_NODE,
  REAL_NODE,
  STRING_NODE
};

// This class always treats copying as moving, due to operational semantics
// of parsing: whenever a semantic value is copied, the older value becomes
// obsolete.

struct node
{
  node(): tag(GENERIC_NODE) {}

  node( const node & other):
    tag(other.tag),
    v(other.v),
    s(std::move(other.s)),
    components(std::move(other.components))
  {}

  node( const node && other ):
    tag(other.tag),
    v(other.v),
    s(std::move(other.s)),
    components(std::move(other.components))
  {}

  void operator=( const node & other )
  {
    *this = std::move(other);
  }

  void operator=( const node && other )
  {
    tag = other.tag;
    v = other.v;
    s = std::move(other.s);
    components = std::move(other.components);
  }

  void operator=( bool b )
  {
    tag = BOOL_NODE;
    v.b = b;
    s.clear();
    components.clear();
  }

  void operator=( long i )
  {
    tag = INT_NODE;
    v.i = i;
    s.clear();
    components.clear();
  }

  void operator=( double r )
  {
    tag = REAL_NODE;
    v.r = r;
    s.clear();
    components.clear();
  }

  void operator=( const std::string & other_s )
  {
    tag = STRING_NODE;
    s = other_s;
    components.clear();
  }

  node_tag tag;

  union {
    bool b;
    double r;
    long i;
  } v;

  std::string s;

  std::vector<node> components;
};

} // namespace Marsyas

#endif // MARSYAS_SCRIPT_SYNTAX_TREE_INCLUDED