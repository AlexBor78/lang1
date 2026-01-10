#include <common/diagnostic/diagnostic.h>
#include <lang/common/compile/unit.h>
#include <string_view>

namespace lang
{
    UnitID CompileUnitsManager::gen_new_id() {
        UnitID id;
        id.id = counter++;

        auto unit = std::make_unique<CompileUnit>();
        unit->id = id;

        units.emplace_back(std::move(unit));
        // i don't like that - 1 with size_t, not sure what i can do with that
        indexes_context[id.id] = units.size() - 1;

        return id;
    }

    void CompileUnitsManager::update_contexts(UnitID id) {
        if(!id.is_valid) throw common::diagnostic::InterError("UnitsManager: update_contexts(): got non valid id");
        
        // if id not specified, try to find
        if(id.id == ULLONG_MAX) id = try_complete(id);

        // update symbol path context, if not empty
        if(!id.symbolpath.empty()) sympaths_context[id.symbolpath] = id.id;
        
        // update file path context, if not empty
        if(!id.filepath.empty()) filepaths_context[id.filepath] = id.id;
    }

    CompileUnit* CompileUnitsManager::get(UnitID id) {
        if(id.id == ULLONG_MAX) id = try_complete(id);
        return units.at(indexes_context.at(id.id)).get();
    }

    const CompileUnit* CompileUnitsManager::get(UnitID id) const {
        if(id.id == ULLONG_MAX) id = try_complete(id);
        return units.at(indexes_context.at(id.id)).get();
    }

    UnitID CompileUnitsManager::try_complete(UnitID id) const {
        // id
        if(id.id == ULLONG_MAX) {
            if(!id.symbolpath.empty()
            && sympaths_context.contains(id.symbolpath)) id.id = sympaths_context.at(id.symbolpath);
        
            if(!id.filepath.empty() 
            && filepaths_context.contains(id.filepath)) id.id = filepaths_context.at(id.filepath);

            if(id.id == ULLONG_MAX) return id;
        }

        // symbol path            // go to unit and set as in it
        if(id.symbolpath.empty()) id.symbolpath = units[indexes_context.at(id.id)]->id.symbolpath;

        // file path            // go to unit and set as in it
        if(id.filepath.empty()) id.filepath = units[indexes_context.at(id.id)]->id.filepath;
        
        return id;
    }

    bool CompileUnitsManager::contains(UnitID id) const noexcept {
        if(id.id == ULLONG_MAX) id = try_complete(id);
        if(id.id != ULLONG_MAX) return true;
        return false;
    }

    bool CompileUnitsManager::contains(SymbolPath sympath) const noexcept {
        return sympaths_context.contains(sympath);
    }

    bool CompileUnitsManager::contains(const std::string& filepath) const noexcept {
        return filepaths_context.contains(filepath);
    }
}
