#pragma once

#include <vector>

#include <common/common.h>

namespace lang::ast::source_locs
{
    class BaseLoc {
    protected:
        static constexpr inline common::SourceLocation default_pos() noexcept {
            return common::SourceLocation{};
        }
        explicit BaseLoc(common::SourceLocation _full_range_loc = default_pos()): 
            source_full_range_loc(std::move(_full_range_loc))
        {}

    public:
        virtual ~BaseLoc() = default;
        common::SourceLocation source_full_range_loc = default_pos();
    };

    class KeywordLoc: public BaseLoc {
    public:
        common::SourceLocation keyword_loc;

    public:
        explicit KeywordLoc(
          common::SourceLocation _word_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  BaseLoc(std::move(_full_range_loc))
        ,   keyword_loc(_word_loc)
        {}
    };


		class NamedLoc : public BaseLoc {
    public:
        common::SourceLocation name_loc;

    public:
        NamedLoc(common::SourceLocation _name_loc = default_pos()
        ,      	 common::SourceLocation _full_range_loc = default_pos()
        ):  BaseLoc(std::move(_full_range_loc))
        ,   name_loc(_name_loc)
        {}

		};

    class OperatorLoc : public BaseLoc {
    public:
        common::SourceLocation op_loc;
        
    protected:
        explicit OperatorLoc(
          common::SourceLocation _op_loc = default_pos()
        , common::SourceLocation _full_range_loc = default_pos()
        ):  BaseLoc(std::move(_full_range_loc))
        ,   op_loc(_op_loc)
        {}
    };

		// what is that locs ??
    class StackAlloLoc : public BaseLoc {
    public:
        std::vector<common::SourceLocation> locs;
        explicit StackAlloLoc(
						std::vector<common::SourceLocation> _locs = std::vector<common::SourceLocation>()
        ,                       common::SourceLocation _pos = default_pos()
        ):  BaseLoc(_pos)
        ,   locs(std::move(_locs))
        {}
    };
}
