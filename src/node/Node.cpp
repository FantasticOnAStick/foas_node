#include <foas/node/Node.h>


namespace foas {
  namespace node {
    Node::Node() : mShutdownRequested(false), mBus(std::make_shared<message::Bus>("node")) {
    }
    
    Node::~Node() {
    }
    
    bool Node::Initialize() {
      // Load all the plugin templates.
      std::string pluginsDirectory = common::FileSystem::GetExecutableDirectory() + "../plugins";
      std::list<common::FileSystem::FileSystemEntry> files = common::FileSystem::GetDirectoryContents(pluginsDirectory);
      
      for(common::FileSystem::FileSystemEntry fil : files) {
	if(fil.type == common::FileSystem::FileSystemEntryType::File) {
	  if(common::Regex::Matches("libfoas_plugins_(.+).so", fil.name)) {
	    std::shared_ptr<common::Task> loaderTask = mPluginManager.LoadTemplate(pluginsDirectory + "/" + fil.name);
	    loaderTask->Start();
	    loaderTask->Wait();
	  }
	}
      }
      
      // TODO: Read config file, instantiate plugins accordingly.
      std::shared_ptr<plugin::PluginInstance> pi_node_info = mPluginManager.InstantiateTemplate("node_info", mBus->CreateSubBus("node_info"));
      
      return true;
    }
    
    std::shared_ptr<common::Task> Node::Run() {
      std::shared_ptr<common::Task> task = std::make_shared<common::Task>([=] {
	  std::unique_lock<std::mutex> lock(mCycleMutex);
	  
	  while(!this->WasShutdownRequested()) {
	    mCycleStepper.wait(lock);
	    
	    this->ProcessEvents();
	  }
	});
      
      return task;
    }
    
    bool Node::WasShutdownRequested() {
      std::lock_guard<std::mutex> lock(mShutdownMutex);
      
      return mShutdownRequested;
    }
    
    void Node::Stop() {
      std::lock_guard<std::mutex> lock(mShutdownMutex);
      
      mShutdownRequested = true;
    }
    
    void Node::Step() {
      mCycleStepper.notify_one();
    }

    void Node::ProcessEvents() {
      // ...
    }
  }
}
