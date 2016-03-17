
// Basic type hierarchy for SMG implementation
// (c) PP, 2016

#include "SMGCType.hh"

#include <memory>
#include <sstream>

namespace smg {

std::ostream &operator << (std::ostream &s, Type t) {
    return s << t.ToString();
}

void TypeImpl::add_field(size_t o, std::string name, Type t) {
    // TODO: assert( isStruct() || isUnion() )
    field_desc fld = { o, name, t };
    fields.push_back(fld);
}

// convert TypeImpl to string representation
std::string TypeImpl::ToString() const {
    std::ostringstream s;
    s << "TypeImpl#" << fid << " ";
    switch(kind) {
    case INT:
        s << "Integer" << bsize << "(sizeof=" << SizeOf() << ")";
        break;
    case PTR:
        s << "Pointer(sizeof=" << SizeOf() << ") to TypeImpl#" << targetType.FrontendId();
        break;
    case STRUCT:
        s << "Struct(sizeof=" << SizeOf() << ", {\n";
        for(field_desc f: fields)
            s << " offset=" << f.offset << " name=" << f.name
              << " type=#" << f.type.FrontendId() << "\n";
        s << "})";
        break;
    case ARRAY:
        s << "Array["<<bsize<<"](sizeof=" << SizeOf() << ") of " << targetType.ToString();
        break;
    default:
        s << "UNKNOWN";
        break;
    }
    return s.str();
}

TypeImpl::map_t TypeImpl::m;  // definition of static member

}  // namespace smg
