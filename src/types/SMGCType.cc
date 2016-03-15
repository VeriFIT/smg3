
// Basic type hierarchy for SMG implementation
// (c) PP, 2016

#include "SMGCType.hh"

#include <memory>
#include <sstream>

namespace smg {


std::ostream &operator << (std::ostream &s, TypeId t) {
    return s << t.toString();
}

void Type::add_field(unsigned o, std::string name, TypeId t) {
    // TODO: assert( isStruct() || isUnion() )
    field_desc fld = { o, name, t };
    fields.push_back(fld);
}

// convert Type to string representation
std::string Type::toString() const {
    std::ostringstream s;
    s << "Type#" << fid << " ";
    switch(kind) {
    case INT:
        s << "Integer" << bsize << "(sizeof=" << size() << ")";
        break;
    case PTR:
        s << "Pointer(sizeof=" << size() << ") to Type#" << targetType.id();
        break;
    case STRUCT:
        s << "Struct(sizeof=" << size() << ", {\n";
        for(field_desc f: fields)
            s << " offset=" << f.offset << " name=" << f.name
              << " type=#" << f.type.id() << "\n";
        s << "})";
        break;
    case ARRAY:
        s << "Array["<<bsize<<"](sizeof=" << size() << ") of " << targetType.toString();
        break;
    default:
        s << "UNKNOWN";
        break;
    }
    return s.str();
}

Type::map_t Type::m;  // definition of static member

}  // namespace smg
