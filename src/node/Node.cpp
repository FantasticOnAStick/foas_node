#include <foas/node/Node.h>


namespace foas {
  namespace node {
    Node::Node() : mShutdown(false), mCycleStepper(std::make_shared<std::condition_variable>()), mBus(std::make_shared<message::Bus>("node", nullptr, mCycleStepper)) {
    }
    
    Node::~Node() {
    }
    
    bool Node::Initialize() {
      mBus->SetClassManager(std::make_shared<message::ClassManager>());
      
      if(this->LoadConfiguration("../config/default.config")) {
	return true;
      }
      
      return false;
    }
    
    bool Node::LoadConfiguration(std::string configFilePath) {
      std::string absoluteConfigFilePath = common::FileSystem::GetAbsolutePath(common::FileSystem::GetExecutableDirectory(), configFilePath);
      
      std::shared_ptr<common::parsers::Parser> parser = std::make_shared<common::parsers::JsonParser>();
      std::shared_ptr<common::Property> configRoot = parser->ParseFile(absoluteConfigFilePath);
      
      if(configRoot) {
	std::shared_ptr<common::Property> configPlugins = configRoot->Get("plugins");
	
	if(configPlugins) {
	  std::vector<std::string> searchPaths;
	  std::shared_ptr<common::Property> configPluginsSearchPaths = configPlugins->Get("search-paths");
	  
	  if(configPluginsSearchPaths) {
	    for(int i = 0; i < configPluginsSearchPaths->Size(); ++i) {
	      searchPaths.push_back(configPluginsSearchPaths->Get(i)->Get<std::string>());
	    }
	  }
	  
	  std::vector<std::string> filenamePatterns;
	  std::shared_ptr<common::Property> configPluginsFilenamePatterns = configPlugins->Get("filename-patterns");
	  
	  if(configPluginsFilenamePatterns) {
	    for(int i = 0; i < configPluginsFilenamePatterns->Size(); ++i) {
	      filenamePatterns.push_back(configPluginsFilenamePatterns->Get(i)->Get<std::string>());
	    }
	  }

	  for(std::string searchPath : searchPaths) {
	    std::string absoluteSearchPath = common::FileSystem::GetAbsolutePath(common::FileSystem::GetExecutableDirectory(), searchPath);
	    
	    std::list<common::FileSystem::FileSystemEntry> files = common::FileSystem::GetDirectoryContents(absoluteSearchPath);
	    
	    for(common::FileSystem::FileSystemEntry fil : files) {
	      if(fil.type == common::FileSystem::FileSystemEntryType::File) {
		bool matches = false;

		for(std::string filenamePattern : filenamePatterns) {
		  if(common::Regex::Matches(filenamePattern, fil.name)) {
		    matches = true;
		    break;
		  }
		}

		if(matches) {
		  std::shared_ptr<common::Task> loaderTask = mPluginManager.LoadTemplate(common::FileSystem::GetAbsolutePath(absoluteSearchPath, fil.name));
		  
		  loaderTask->Start();
		  loaderTask->Wait();
		}
	      }
	    }
	  }
	  
	  std::shared_ptr<common::Property> configPluginsInstances = configPlugins->Get("instances");
	  
	  if(configPluginsInstances) {
	    for(int i = 0; i < configPluginsInstances->Size(); ++i) {
	      std::string name = configPluginsInstances->Get(i)->Get("name")->Get<std::string>();
	      std::string type = configPluginsInstances->Get(i)->Get("type")->Get<std::string>();

	      std::shared_ptr<message::Bus> instanceBus = mBus->CreateSubBus(name);
	      
	      std::shared_ptr<plugin::PluginInstance> instance = mPluginManager.InstantiateTemplate(type, instanceBus);
	      
	      mPluginInstances.push_back(instance);
	    }
	  }
	}
	
	return true;
      } else {
	std::cerr << "Unable to parse config file '" << absoluteConfigFilePath << "'" << std::endl;
      }
      
      return false;
    }
    
    std::shared_ptr<common::Task> Node::Run() {
      std::shared_ptr<common::Task> task = std::make_shared<common::Task>([=] {
	  std::unique_lock<std::mutex> lock(mCycleMutex);
	  
	  while(mShutdown == false) {
	    mCycleStepper->wait(lock);
	    
	    this->ProcessEvents();
	  }
	  
	  for(std::shared_ptr<plugin::PluginInstance> instance : mPluginInstances) {
	    instance->Deinitialize();
	  }
	});
      
      return task;
    }
    
    void Node::Stop() {
      mShutdown = true;
      
      this->Step();
    }
    
    void Node::Step() {
      mCycleStepper->notify_one();
    }
    
    void Node::ProcessEvents() {
      std::map<std::string, std::list<std::shared_ptr<message::Message>>> messageMap = mBus->CollectQueuedMessages();

      std::list<std::shared_ptr<message::Message>> messages;
      for(std::pair<std::string, std::list<std::shared_ptr<message::Message>>> mapPair : messageMap) {
	messages.splice(messages.end(), mapPair.second);
      }
      
      std::cout << "Process: " << messages.size() << " message(s)" << std::endl;
      for(std::shared_ptr<message::Message> message : messages) {
	std::cout << " * From: " << message->GetSender() << std::endl;
      }
    }
  }
}
