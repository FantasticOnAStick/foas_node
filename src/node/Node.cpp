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
    
    std::shared_ptr<common::Task> Node::Run() {
      std::shared_ptr<common::Task> task = std::make_shared<common::Task>([] {
	  // ...
	});
      
      return task;
    }
  }
}
