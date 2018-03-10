#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <foas/node/Node.h>


foas::node::Node node;


void catchHandler(int signal) {
  switch(signal) {
  case SIGTERM:
  case SIGINT: {
    std::cout << "\r";
    
    node.Stop();
  } break;
    
  default:
    break;
  }
}


int main(int argc, char** argv) {
  int returnValue;
  
  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = catchHandler;
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGINT, &action, NULL);
  
  if(node.Initialize()) {
    std::shared_ptr<foas::common::Task> runTask = node.Run();
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
