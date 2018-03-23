#ifndef __FOAS_NODE_NODE_H__
#define __FOAS_NODE_NODE_H__


// C++
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>

// foas_common
#include <foas/common/Task.h>
#include <foas/common/Protected.hpp>
#include <foas/common/FileSystem.h>
#include <foas/common/Regex.h>
#include <foas/common/parsers/JsonParser.h>

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
      std::list<std::shared_ptr<plugin::PluginInstance>> mPluginInstances;
      
      std::mutex mCycleMutex;
      std::condition_variable mCycleStepper;

      common::Protected<bool> mShutdown;
      
    public:
      Node();
      ~Node();
      
      bool Initialize();
      bool LoadConfiguration(std::string configFilePath);
      
      std::shared_ptr<common::Task> Run();
      
      void Stop();
      void Step();

      void ProcessEvents();
    };
  }
}


#endif /* __FOAS_NODE_NODE_H__ */
