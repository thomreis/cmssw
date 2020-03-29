#ifndef L1TkTrigger_L1TkPrimaryVertex_h
#define L1TkTrigger_L1TkPrimaryVertex_h

// Nov 12, 2013
// First version of a class for L1-zvertex

namespace l1t {

  class L1TkPrimaryVertex {
  public:
    L1TkPrimaryVertex() : zvertex_(-999), sum_(-999) {}

    ~L1TkPrimaryVertex() {}

    L1TkPrimaryVertex(float z, float s) : zvertex_(z), sum_(s) {}

    float zvertex() const { return zvertex_; }
    float sum() const { return sum_; }

  private:
    float zvertex_;
    float sum_;
  };

#include <vector>

  typedef std::vector<L1TkPrimaryVertex> L1TkPrimaryVertexCollection;

}  // namespace l1t
#endif
