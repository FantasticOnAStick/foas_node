#include <iostream>
#include <unistd.h>

#include <foas/node/Node.h>


int main(int argc, char** argv) {
  int returnValue;
  foas::node::Node main;
  
  if(main.Initialize()) {
    std::shared_ptr<foas::common::Task> runTask = main.Run();
    runTask->Start();
    foas::common::TaskResult result = runTask->Wait();
    
    switch(result.Status) {
    case foas::common::Success: {
      returnValue = EXIT_SUCCESS;
    } break;
      
    default: {
      returnValue = EXIT_FAILURE;
    } break;
    }
  }
  
  return returnValue;
}
