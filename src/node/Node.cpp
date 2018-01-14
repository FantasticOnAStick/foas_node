#include <foas/node/Node.h>


namespace foas {
  namespace node {
    Node::Node() {
    }

    Node::~Node() {
    }
    
    bool Node::Initialize() {
      return true;
    }
    
    common::Task Node::Run() {
      common::Task task;
      
      // ...
      
      return task;
    }
  }
}
