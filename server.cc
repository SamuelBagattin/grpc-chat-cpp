#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "chat.grpc.pb.h"
#include "chat.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter;
using grpc::Status;
using grpc_chat::Message;
using grpc_chat::MessagesService;

class ChatServiceImpl final : public MessagesService::Service
{
  Status Chat(ServerContext *context, ServerReaderWriter<Message, Message> *stream) override
  {
    std::vector<Message> received_messages;
    Message message;
    while (stream->Read(&message))
    {
      for (const Message &msg : received_messages)
      {
        std::cout << msg.name() << msg.content() << std::endl;
      }
      received_messages.push_back(message);
    }
    return Status::OK;
  }
};

void RunServer()
{
  std::string server_address("0.0.0.0:50051");
  ChatServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char **argv)
{
  RunServer();
  return 0;
}
