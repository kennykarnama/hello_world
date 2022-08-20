#ifndef MyController_hpp
#define MyController_hpp

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include<cstdlib>
#include <folly/concurrency/ConcurrentHashMap.h>

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  ENDPOINT("GET", "/", root) {
    auto dto = MyDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("POST", "/grant", login, BODY_DTO(Object<GrantRequestDto>, grantRequestDto)) {
        auto dto = GrantDtoResponse::createShared();
        auto username = getenv("APP_USERNAME");
        auto password = getenv("APP_PASSWORD");

        auto errorDto = ErrorDto::createShared();
        if (strcmp(username, grantRequestDto->username->c_str()) != 0
        || strcmp(password, grantRequestDto->password->c_str())) {
            errorDto->errorCode = "-";
            errorDto->errorMessage = "invalid credentials";
            return createDtoResponse(Status::CODE_401, errorDto);
        }
        boost::uuids::random_generator gen;
        boost::uuids::uuid id = gen();
        folly::ConcurrentHashMap<String, int> cache;
        cache.insert(to_string(id), 1);
        dto->token = to_string(id);
        dto->ttl = 30;
        auto it = cache.find("test");
        if (it == cache.end()) {
            OATPP_LOGD("hashmap not exist", "test=not found");
        }
        it = cache.find(to_string(id));
        if (it != cache.end()) {
            OATPP_LOGD("hashmap exist", "test=%d", it->second);
        }
        return createDtoResponse(Status::CODE_200, dto);
  }
  
  // TODO Insert Your endpoints here !!!

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */