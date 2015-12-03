#pragma once

#include <SMGObject.hh>
#include <SMGRegion.hh>

class SMGObjectVisitor
{
private:
  void visitDefault(const SMGObject &);
public:
  void visit(const SMGObject & pObject);
  void visit(const SMGRegion & pObject);
  //void visit(const SMGSingleLinkedList & pObject);
  //void visit(const SimpleBinaryTree & pObject);
};
