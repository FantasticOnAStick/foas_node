#ifndef __FOAS_NODE_NODE_H__
#define __FOAS_NODE_NODE_H__


// C++
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>

// foas_common
#include <foas/common/Task.h>
#include <foas/common/FileSystem.h>
#include <foas/common/Regex.h>

// foas_message
#include <foas/message/Bus.h>

// foas_plugin
#include <foas/plugin/PluginManager.h>


namespace foas {
  namespace node {
    class Node {
    private:
      std::shared_ptr<message::Bus> mBus;
      plugin::PluginManager mPluginManager;
      
      std::mutex mCycleMutex;
      std::condition_variable mCycleStepper;

      std::mutex mShutdownMutex;
      bool mShutdownRequested;

      bool WasShutdownRequested();
      
    public:
      Node();
      ~Node();
      
      bool Initialize();
      std::shared_ptr<common::Task> Run();
      
      void Stop();
      void Step();

      void ProcessEvents();
    };
  }
}


#endif /* __FOAS_NODE_NODE_H__ */
