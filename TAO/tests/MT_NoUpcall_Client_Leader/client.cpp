#include "SharedIntf_i.h"
#include "worker.h"
#include "chatter.h"
#include "police.h"

#include "ace/SString.h"
#include "ace/streams.h"

int
main(int argc, char* argv[])
{
  CORBA::ORB_var orb_;
  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    orb_ = CORBA::ORB_init (argc, argv, "myorb-client" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::Object_var poa_object =
      orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    PortableServer::POA_var poa = root_poa;

    poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG((LM_INFO,"(%P|%t) START OF CLIENT TEST\n"));

    // Creating the monitorable servant and activating it
    //
    Test_Idl_SharedIntf_i* intf_i = new Test_Idl_SharedIntf_i (orb_.in());

    PortableServer::ServantBase_var base_var = intf_i;
    PortableServer::ObjectId_var intfId_var =
      poa->activate_object (base_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::Object_var obj_var =
      poa->id_to_reference (intfId_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    Test_Idl::SharedIntf_var intf_var =
      Test_Idl::SharedIntf::_narrow (obj_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Creating stringified IOR of the servant and writing it to a file.
    //
    CORBA::String_var intfString_var =
      orb_->object_to_string (intf_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_CString ior_filename ("client.ior");
    ofstream ior_filestream (ior_filename.c_str());
    ior_filestream << intfString_var.in () << endl;
    ior_filestream.close ();
    ACE_DEBUG ((LM_INFO,"(%P|%t) client IOR to %s\n", ior_filename.c_str()));

    // Running ORB in separate thread
    Worker worker (orb_.in ()); if (worker.activate (THR_NEW_LWP |
      THR_JOINABLE, 1) != 0) ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n",
      "Cannot activate client threads"), -1);

    ACE_DEBUG((LM_INFO,"(%P|%t) Await server initialization\n"));
    poll ("./server.ior");
    ACE_DEBUG((LM_INFO,"(%P|%t) Server IOR file was detected\n"));

    ACE_Mutex mutex;
    ACE_Condition<ACE_Mutex> stop_condition (mutex);

    const char* serverior = "file://server.ior";
    Chatter worker2 (orb_.in (), serverior, stop_condition);

    if (worker2.activate (THR_NEW_LWP | THR_JOINABLE, 2) != 0)
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n",
      "Cannot activate chatty client threads"), -1);


    do {
      stop_condition.wait ();
      ACE_DEBUG((LM_INFO,"(%P|%t) So far, %d/%d requests/replies have been processed\n",
        worker2.nrequests (), worker2.nreplies ()));
    }
    while (worker2.nreplies () < 2);

    // Kill the peer
    {
      CORBA::Object_var rawObject = orb_->string_to_object( serverior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_Idl::SharedIntf_var intf_var =
        Test_Idl::SharedIntf::_narrow(rawObject.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (intf_var.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Nil reference <%s>\n", serverior), -1);

      // make call on server
      ACE_DEBUG((LM_INFO,"(%P|%t) farewell START for %s\n", serverior));

      intf_var->farewell(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG((LM_INFO,"(%P|%t) farewell COMPLETE for %s\n", serverior));
    }

    ACE_DEBUG((LM_INFO,"(%P|%t) END OF CLIENT TEST\n"));
    orb_.in()->shutdown ();
    ACE_TRY_CHECK;

    root_poa->destroy(1,1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    orb_->destroy( ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    ACE_DEBUG((LM_INFO,"(%P|%t) Client Test %s\n",
      (worker2.nrequests() == worker2.nreplies())?"succeded":"failed"));

    result = (worker2.nrequests_ == worker2.nreplies_)? 0 : -1;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error: Exception caught:");
  }
  ACE_ENDTRY;

  ACE_OS::unlink ("client.ior");
  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Condition<ACE_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate  ACE_Condition<ACE_Mutex>
#endif
