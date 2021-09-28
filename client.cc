#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "chat.grpc.pb.h"
#include "chat.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc_chat::MessagesService;
using grpc_chat::Message;

class GreeterClient
{
public:
    GreeterClient(std::shared_ptr<Channel> channel) : stub_(MessagesService::NewStub(channel)) {}

    std::string SayHello(const std::string& user) {
        Message request;
        request.set_name(user);

        Message reply;

        ClientContext context;

        Status status = stub_->SayHello(&context, request, &reply);

        if(status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** const argv)
{
    GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user("world");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;
    return 0;
}
