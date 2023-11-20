#include "greeter_http_handler_component.h"

namespace samples {
std::string GreeterHttpHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext&) const {
  return grpc_greeter_client_.SayHello(request.RequestBody());
}
}  // namespace samples
