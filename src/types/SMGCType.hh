#pragma once

// Basic type hierarchy for SMG implementation
// (c) PP, 2016

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>

namespace smg {

typedef int frontend_id;    // unique GCC id (or LLVM representation)

class Type;

// TypeId -- reference to type representation (copyable, assignable)
class TypeId {
    Type *p;    // target owned by registry, should be valid
    TypeId(Type *ptr) : p{ptr} {}
    friend class Type;
  public:
    TypeId(): p{nullptr} {};
    TypeId(const TypeId &) = default;
    TypeId&operator=(const TypeId &) = default;

    unsigned size() const;
    unsigned GetSize() const { return size(); }
    std::string toString() const;
    frontend_id id() const;

    bool isInteger() const;
    bool isPointer() const;
    bool isStruct() const;
    bool isUnion() const;
    bool isArray() const;

    // TODO: remove
    void add_field(unsigned o, std::string name, TypeId t);
};

// type output
std::ostream &operator << (std::ostream &s, TypeId t);

// Type representation (not all members are valid for a type)
class Type final {
  frontend_id fid;      // front-end type representation
  unsigned sz;          // sizeof
  unsigned bsize;       // int width, float width, array dimension
  TypeId targetType;    // ptr, array, return type

  struct field_desc {   // for struct/union members, ...
    unsigned offset;    // always zero for union
    std::string name;   // .name
    TypeId type;
  };
  std::vector<field_desc> fields; // struct/union fields
public:
  enum Kind { UNKNOWN, VOID, INT, REAL, PTR, STRUCT, UNION, ARRAY, FUNCTION, };
private:
  Kind kind = UNKNOWN;
public:
  // generic wrapper
  template<typename T1, typename ... T>
  static TypeId create(T1 x, T ... args) {
     if(mapped(x))
        return m[x].get();
     // type not registered yet -- create new and register
     std::unique_ptr<Type> t{new Type(x,args...)};
     m[x] = std::move(t); // TODO use emplace?
     return m[x].get();
  }

private: // constructors
  Type(frontend_id id, unsigned size, unsigned bs): fid(id), sz{size}, bsize{bs}, kind{INT} {}
  Type(frontend_id id, unsigned size, TypeId t): fid(id), sz{size}, targetType(t), kind{PTR} {}
  Type(frontend_id id, unsigned size): fid(id), sz{size}, kind{STRUCT} {}
  Type(frontend_id id, TypeId t, unsigned n): fid(id), sz{t.size()*n}, bsize(n), targetType(t), kind{ARRAY} {}
  Type() = delete;
  Type(const Type&) = delete;           // not copyable
  Type&operator=(const Type&) = delete; // not assignable

public:
  unsigned      size() const { return sz; } // sizeof
  frontend_id   id() const { return fid; }

  bool          isInteger() const { return kind==INT; }
  bool          isPointer() const { return kind==PTR; }
  bool          isStruct() const { return kind==STRUCT; }
  bool          isUnion() const { return kind==UNION; }
  bool          isArray() const { return kind==ARRAY; }

  std::string   toString() const;
  void          add_field(unsigned o, std::string name, TypeId t);
  static TypeId get(frontend_id id) {
     if(mapped(id))     return m[id].get();
     else               return nullptr; // TODO: throw bad_frontend_type_id
  }
private:
  // map registry for all types, updated dynamically
  // TODO: use singleton?
  typedef std::map<frontend_id,std::unique_ptr<Type>> map_t;
  static map_t m; // owns all types
  static bool mapped(frontend_id id) { return (m.find(id)!=m.end()); }
}; // class Type

// class Type methods definitions
inline unsigned TypeId::size() const { return p->size(); }
inline frontend_id TypeId::id() const { return p->id(); }

inline bool TypeId::isInteger() const { return p->isInteger(); }
inline bool TypeId::isPointer() const { return p->isPointer(); }
inline bool TypeId::isStruct() const { return p->isStruct(); }
inline bool TypeId::isUnion() const { return p->isUnion(); }
inline bool TypeId::isArray() const { return p->isArray(); }

inline std::string TypeId::toString() const { return p->toString(); }
inline void TypeId::add_field(unsigned o, std::string name, TypeId t) { p->add_field(o,name,t); }

// TODO: remove/rename
typedef class TypeId SMGCType;

}  // namespace smg
