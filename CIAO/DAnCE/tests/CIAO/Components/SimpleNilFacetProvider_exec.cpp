// $Id$
//
// ****              Code generated by the                 ****
// ****  Component Integrated ACE ORB (CIAO) CIDL Compiler ****
// CIAO has been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// CIDL Compiler has been developed by:
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about CIAO is available at:
//    http://www.dre.vanderbilt.edu/CIAO

#include "SimpleNilFacetProvider_exec.h"
#include "ciao/CIAO_common.h"

namespace CIAO_Simple_SimpleNilFacetProvider_Impl
{
  //==================================================================
  // Facet Executor Implementation Class:   Trigger_exec_i
  //==================================================================

  Trigger_exec_i::Trigger_exec_i (void)
  {
    CIAO_TRACE ("Trigger_exec_i::Trigger_exec_i (void)");
  }

  Trigger_exec_i::~Trigger_exec_i (void)
  {
    CIAO_TRACE ("Trigger_exec_i::~Trigger_exec_i (void)");
  }

  // Operations from ::Simple::Trigger

  void
  Trigger_exec_i::hello (
    const char * hello )
  {
    CIAO_TRACE ("Trigger_exec_i::hello ()");
    // Your code here.
    CIAO_DEBUG ((LM_EMERGENCY, "Trigger_exec_i::hello - "
                 "Got the following information from trig port: %C\n",
                 hello));
  }

  //==================================================================
  // Component Executor Implementation Class:   SimpleNilFacetProvider_exec_i
  //==================================================================

  SimpleNilFacetProvider_exec_i::SimpleNilFacetProvider_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::SimpleNilFacetProvider_exec_i (void)");
  }

  SimpleNilFacetProvider_exec_i::~SimpleNilFacetProvider_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::~SimpleNilFacetProvider_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  ::Simple::CCM_Trigger_ptr
  SimpleNilFacetProvider_exec_i::get_trig ()
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::get_trig ()");
    return ::Simple::CCM_Trigger::_nil ();
  }

  // Operations from Components::SessionComponent

  void
  SimpleNilFacetProvider_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimpleNilFacetProvider_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleNilFacetProvider_exec_i::configuration_complete ()
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleNilFacetProvider_exec_i::ccm_activate ()
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::ccm_activate ()");
    // Your code here.
  }

  void
  SimpleNilFacetProvider_exec_i::ccm_passivate ()
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimpleNilFacetProvider_exec_i::ccm_remove ()
  {
    CIAO_TRACE ("SimpleNilFacetProvider_exec_i::ccm_remove ()");
    // Your code here.
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleNilFacetProviderHome_exec_i
  //==================================================================

  SimpleNilFacetProviderHome_exec_i::SimpleNilFacetProviderHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetProviderHome_exec_i::SimpleNilFacetProviderHome_exec_i (void)");
  }

  SimpleNilFacetProviderHome_exec_i::~SimpleNilFacetProviderHome_exec_i (void)
  {
    CIAO_TRACE ("SimpleNilFacetProviderHome_exec_i::~SimpleNilFacetProviderHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleNilFacetProviderHome_exec_i::create ()
  {
    CIAO_TRACE ("SimpleNilFacetProviderHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleNilFacetProvider_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLENILFACETPROVIDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleNilFacetProviderHome_Impl (void)
  {
    CIAO_TRACE ("create_Simple_SimpleNilFacetProviderHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleNilFacetProviderHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

