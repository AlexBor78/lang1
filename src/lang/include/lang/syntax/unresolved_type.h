#pragma once 

#include <string_view>
#include <lang/semantic/types.h>

namespace lang::syntax {
  /**
   * @brief temporary type for parser, just name of type, 
            will not be unique: if N "int" vars -> N temporary types with name "int"
            live until semantic
   * 
   */

  class UnresolvedType : public semantic::CoreType {
  public:
  	/**
  	 * @brief name of type, save here to process in semantic
		 * @note	string is allocated in SourceFile
  	 */
  	std::string_view name;

  	UnresolvedType() = default;
  	UnresolvedType(
			std::string_view _name
  	):  name(_name)
  	{}
  };
}
