#include "health_service_component.h"

void HealthServiceComponent::Check(
    grpc::health::v1::HealthBase::CheckCall& call,
    grpc::health::v1::HealthCheckRequest&& request) {
  grpc::health::v1::HealthCheckResponse response{};
  response.set_status(grpc::health::v1::HealthCheckResponse_ServingStatus::
                          HealthCheckResponse_ServingStatus_SERVING);

  call.Finish(response);
}
