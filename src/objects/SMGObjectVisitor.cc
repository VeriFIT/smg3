#include <string>

#include "SMGObjectVisitor.hh"
#include <UnsupportedOperationException.hh>

void SMGObjectVisitor::visitDefault(const SMGObject & pObject)
{
  std::string message = "This visitor cannot handle objects of class [" + pObject.getClassName() + "]";
  throw UnsupportedOperationException(message.c_str());
}

void SMGObjectVisitor::visit(const SMGObject & pObject) 
{
  visitDefault(pObject);
}
void SMGObjectVisitor::visit(const SMGRegion & pObject)
{
  visitDefault(pObject);
}

/*@SuppressWarnings("checkstyle:designforextension")
public void visit(final SMGSingleLinkedList pObject) {
  visitDefault(pObject);
}

@SuppressWarnings("checkstyle:designforextension")
public void visit(final SimpleBinaryTree pObject) {
  visitDefault(pObject);
}*/
