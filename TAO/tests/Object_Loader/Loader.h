// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Object_Loader
//
// = FILENAME
//   Loader.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_OBJECT_LOADER_LOADER_H
#define TAO_OBJECT_LOADER_LOADER_H

#include "tao/ior_dll/Object_Loader.h"
#include "OLT_Export.h"

class OLT_Export Loader : public TAO_Object_Loader
{
  // = TITLE
  //   Implement the Test interface
  //
  // = DESCRIPTION
  //
public:
  Loader (void);
  // ctor

  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           CORBA::Environment &)
    ACE_THROW_SPEC (());
  // Create a new instance
};

#if defined(__ACE_INLINE__)
#include "Loader.i"
#endif /* __ACE_INLINE__ */

// ACE_STATIC_SVC_DECLARE (Loader)
ACE_FACTORY_DECLARE (OLT, Loader)

#endif /* TAO_OBJECT_LOADER_LOADER_H */
