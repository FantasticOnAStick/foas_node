#ifndef __FOAS_NODE_NODE_H__
#define __FOAS_NODE_NODE_H__


#include <memory>

#include <foas/common/Task.h>
#include <foas/message/Bus.h>


namespace foas {
  namespace node {
    class Node {
    private:
      message::Bus mBus;
      
    public:
      Node();
      ~Node();

      bool Initialize();
      std::shared_ptr<common::Task> Run();
    };
  }
}


#endif /* __FOAS_NODE_NODE_H__ */
