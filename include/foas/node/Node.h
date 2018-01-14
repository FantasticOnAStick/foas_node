#ifndef __FOAS_NODE_NODE_H__
#define __FOAS_NODE_NODE_H__


#include <foas/common/Task.h>


namespace foas {
  namespace node {
    class Node {
    public:
      Node();
      ~Node();

      bool Initialize();
      common::Task Run();
    };
  }
}


#endif /* __FOAS_NODE_NODE_H__ */
