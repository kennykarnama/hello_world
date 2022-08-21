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

using namespace std::chrono;

/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
private:
    folly::ConcurrentHashMap<String,uint64_t> memo;
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {
  }
public:
  
  ENDPOINT("GET", "/", root) {
    auto dto = MyDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }
  ADD_CORS(login)
  ENDPOINT("POST", "/grant", login, BODY_DTO(Object<GrantRequestDto>, grantRequestDto)) {
        auto dto = GrantDtoResponse::createShared();
        auto username = getenv("APP_USERNAME");
        auto password = getenv("APP_PASSWORD");
        auto ttl_str = getenv("APP_LOGIN_TTL");
        auto ttl = strtoull(ttl_str, NULL, 0);

        auto errorDto = ErrorDto::createShared();
        if (strcmp(username, grantRequestDto->username->c_str()) != 0
        || strcmp(password, grantRequestDto->password->c_str())) {
            errorDto->errorCode = "-";
            errorDto->errorMessage = "invalid credentials";
            return createDtoResponse(Status::CODE_401, errorDto);
        }
        boost::uuids::random_generator gen;
        boost::uuids::uuid id = gen();

        system_clock::time_point tp = system_clock::now();
        system_clock::duration dtn = tp.time_since_epoch();
        auto ms = duration_cast<milliseconds>(dtn).count();
        auto epochExpiration = ms + ttl * 1000;

        this->memo.insert(to_string(id), epochExpiration);
        dto->token = to_string(id);
        dto->ttl = ttl;
        dto->epochExpiration = epochExpiration;

        return createDtoResponse(Status::CODE_200, dto);
  }

    ENDPOINT("GET", "/memos", memos) {
        auto memoDto = MemoDto::createShared();
        memoDto->ids = {};
        OATPP_LOGD("list", "'%d'", 2);

        auto it = this->memo.begin();

        while (it != this->memo.end()) {
            OATPP_LOGD("memos", "'%s'", it->first->c_str());
            memoDto->ids->push_back(it->first);
            ++it;
        }
        return createDtoResponse(Status::CODE_200, memoDto);
    }
    
    ADD_CORS(validateToken)
    ENDPOINT("POST", "/grant/valid", validateToken, BODY_DTO(Object<ValidateTokenReqDto>, req)) {

        OATPP_LOGD("/grant/valid", "")
        auto validateResp = ValidateTokenRespDto::createShared();

        system_clock::time_point tp = system_clock::now();
        system_clock::duration dtn = tp.time_since_epoch();
        auto ms = duration_cast<milliseconds>(dtn).count();

        auto it = this->memo.find(req->token);

        auto errorDto = ErrorDto::createShared();

        if (it == this->memo.end()) {
            errorDto->errorCode = "1000";
            errorDto->errorMessage = "not authorized";
            return createDtoResponse(Status::CODE_401, errorDto);
        }

        if (ms > it->second) {
            errorDto->errorCode = "1001";
            errorDto->errorMessage = "not authorized";
            return createDtoResponse(Status::CODE_401, errorDto);
        }

        return createDtoResponse(Status::CODE_200, validateResp);
    }

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */
