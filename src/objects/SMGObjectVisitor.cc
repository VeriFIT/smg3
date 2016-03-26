#include "SMGObjectVisitor.hh"
#include <string>
#include "exceptions/UnsupportedOperationException.hh"

namespace smg {

void SMGObjectVisitor::VisitDefault(const SMGObject& object) {
  std::string message = "This visitor cannot handle objects of class [";
  message += typeid(object).name();
  message += "]";
  throw UnsupportedOperationException(message.c_str()); //TODO(anyone) dangling pointer
}

void SMGObjectVisitor::Visit(const SMGObject& object) { VisitDefault(object); }

void SMGObjectVisitor::Visit(const SMGRegion& object) { VisitDefault(object); }

/*@SuppressWarnings("checkstyle:designforextension")
public void visit(final SMGSingleLinkedList pObject) {
  visitDefault(pObject);
}

@SuppressWarnings("checkstyle:designforextension")
public void visit(final SimpleBinaryTree pObject) {
  visitDefault(pObject);
}*/

}  // namespace smg
