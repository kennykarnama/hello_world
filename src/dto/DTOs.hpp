#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class MyDto : public oatpp::DTO {
  
  DTO_INIT(MyDto, DTO)
  
  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
  
};

class GrantDtoResponse : public oatpp::DTO {

    DTO_INIT(GrantDtoResponse, DTO)

    DTO_FIELD(String, token);
    DTO_FIELD(Int64, ttl);
    DTO_FIELD(Int64, epochExpiration);
};

class GrantRequestDto : public oatpp::DTO {

    DTO_INIT(GrantRequestDto, DTO)

    DTO_FIELD(String, username);

    DTO_FIELD(String, password);
};

class ErrorDto : public oatpp::DTO {
    DTO_INIT(ErrorDto, DTO)

    DTO_FIELD(String, errorCode);

    DTO_FIELD(String, errorMessage);
};

class ValidateTokenReqDto : public oatpp::DTO {
    DTO_INIT(ValidateTokenReqDto, DTO)

    DTO_FIELD(String, token);
};

class ValidateTokenRespDto : public oatpp::DTO {
    DTO_INIT(ValidateTokenRespDto, DTO);
};

class MemoDto : public oatpp::DTO {
    DTO_INIT(MemoDto, DTO)

    DTO_FIELD(Vector<String>, ids);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
