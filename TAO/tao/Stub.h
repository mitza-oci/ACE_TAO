// -*- C++ -*-

//=============================================================================
/**
 *  @file    Stub.h
 *
 *  @author  Portions Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author  Portions Copyright 1997-2002 by Washington University
 */
//=============================================================================

#ifndef TAO_STUB_H
#define TAO_STUB_H

#include /**/ "ace/pre.h"

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/MProfile.h"
#include "tao/ORB_Core_Auto_Ptr.h"
#include <atomic>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_Abstract_ServantBase;
class TAO_Policy_Set;
class TAO_Profile;

namespace TAO
{
  class ObjectKey;
  class Object_Proxy_Broker;
  class Transport_Queueing_Strategy;
}

namespace IOP
{
  struct IOR;
}

/**
 * @class TAO_Stub
 *
 * @brief TAO_Stub
 *
 * Per-objref data includes the (protocol-specific) Profile, which
 * is handled by placing it into a subclass of this type along
 * with data that may be used in protocol-specific caching
 * schemes.
 * The type ID (the data specified by CORBA 2.0 that gets exposed
 * "on the wire", and in stringified objrefs) is held by this
 * module.
 * The stub APIs are member functions of this type.
 */
class TAO_Export TAO_Stub
{
public:
#if (TAO_HAS_CORBA_MESSAGING == 1)
  /**
   * Returns the effective policy if @a type is a known client-exposed
   * policy type.  Returns the effective override for all other policy
   * types.
   */
  virtual CORBA::Policy_ptr get_policy (CORBA::PolicyType type);

  virtual CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  virtual TAO_Stub* set_policy_overrides (const CORBA::PolicyList & policies,
                                          CORBA::SetOverrideType set_add);

  virtual CORBA::PolicyList *get_policy_overrides (
    const CORBA::PolicyTypeSeq & types);
#endif

  /// Return the queueing strategy to be used in by the transport.
  /// Selection will be based on the SyncScope policies.
  TAO::Transport_Queueing_Strategy *transport_queueing_strategy ();

  /// All objref representations carry around a type ID.
  CORBA::String_var type_id;

  /**
   * All objref representations know how to hash themselves and
   * compare themselves for equivalence to others.  It's easily
   * possible to have two objrefs that are distinct copies of data
   * that refers/points to the same remote object (i.e. are
   * equivalent).
   */
  CORBA::ULong hash (CORBA::ULong maximum);

  /// Implement the is_equivalent() method for the CORBA::Object
  CORBA::Boolean is_equivalent (CORBA::Object_ptr other_obj);

  // Our Constructors ...

  /// Construct from a repository ID and a list of profiles.
  TAO_Stub (const char *repository_id,
            const TAO_MProfile &profiles,
            TAO_ORB_Core *orb_core);

  // = Memory management.
  void _incr_refcnt ();
  void _decr_refcnt ();

  /// Return the Profile lock. This lock can be used at places where
  /// profiles need to be edited.
  const TAO_SYNCH_MUTEX& profile_lock () const;

  /// Manage the base (non-forwarded) profiles.
  /// Returns a pointer to the profile_in_use object.  This object
  /// retains ownership of this profile.
  TAO_Profile *profile_in_use ();

  /// Return the ObjectKey
  const TAO::ObjectKey &object_key () const;

  /**
   * Copy of the profile list, user must free memory when done.
   * although the user can call make_profiles() then reorder
   * the list and give it back to TAO_Stub.
   */
  TAO_MProfile *make_profiles ();

  /// Obtain a reference to the basic profile set.
  const TAO_MProfile& base_profiles () const;

  /// Obtain a reference to the basic profile set.
  TAO_MProfile& base_profiles ();

  /// Obtain a pointer to the forwarded profile set
  const TAO_MProfile *forward_profiles () const;

  /// Obtain a pointer to the forwarded profile set
  TAO_MProfile *forward_profiles ();

  /// True if permanent location forward occurred, in this case the lock must be set and the

  // Manage forward and base profiles.
  /**
   * THREAD SAFE.  If forward_profiles is null then this will
   * get the next profile in the base_profiles list.  If forward is not null
   * then this will get the next profile for the list of forwarding
   * profiles.  If all profiles have been tried then 0 is returned and
   * profile_in_use_ is set to the first profile in the base_profiles
   * list.
   */
  TAO_Profile *next_profile ();

  /**
   * THREAD SAFE
   * This method will reset the base profile list to reference the first
   * profile and if there are anmy existing forward profiles they are
   * reset.
   */
  void reset_profiles ();

  /// Returns true if the profile in use is
  /// the same as the profile in use after
  /// reset_profiles() is called.
  CORBA::Boolean at_starting_profile () const;

  /// Returns true if a forward profile has successfully been used.
  /// profile_success_ && forward_profiles_
  CORBA::Boolean valid_forward_profile ();

  /// NON-THREAD-SAFE.  Will set profile_success_ to true.
  void set_valid_profile ();

  /// Returns true if a connection was successful with at least
  /// one profile.
  CORBA::Boolean valid_profile () const;

  /// Initialize the base_profiles_ and set profile_in_use_ to
  /// reference the first profile.
  TAO_Profile *base_profiles (const TAO_MProfile& mprofiles);

  /**
   * THREAD SAFE.
   * Set the forward_profiles.  This object will assume ownership of
   * this TAO_MProfile object!!  if @a permanent_forward is true,
   * currently used profiles will be replaced permanently, otherwise
   * stub may fallback to current profiles later.  The flag
   * permanent_forward=true is only valid if currently used profile
   * set represents a GroupObject (IOGR), otherwise this flag will be
   * ignored.
   *
   * Updates the profile in use to be the first profile of
   * the newly set forward target.
   */
  void add_forward_profiles (const TAO_MProfile &mprofiles,
                             const CORBA::Boolean permanent_forward = false);

  /**
   * THREAD SAFE
   * Used to get the next profile after the one being used has
   * failed during the initial connect or send of the message!
   */
  CORBA::Boolean next_profile_retry ();

  /// Accessor.
  TAO_ORB_Core* orb_core () const;

  /// Is this stub collocated with the servant?
  CORBA::Boolean is_collocated () const;

  /// Mutator to mark this stub as being collocated with the servant.
  void is_collocated (CORBA::Boolean);

  /// This returns a duplicated ORB pointer.
  CORBA::ORB_ptr servant_orb_ptr ();

  /// This returns the ORB var itself (generally for temporary use).
  CORBA::ORB_var &servant_orb_var ();

  /**
   * Accesor and mutator for the servant ORB.  Notice that the mutator
   * assumes the ownership of the passed in ORB and the accesor does not
   * return a copy of the orb since the accessing of the ORB is considered
   * temporary.
   */
  void servant_orb (CORBA::ORB_ptr orb);

  /// Mutator for setting the servant in collocated cases.
  void collocated_servant (TAO_Abstract_ServantBase* servant);

  /// Accessor for the servant reference in collocated cases.
  TAO_Abstract_ServantBase* collocated_servant () const;

  /// Mutator for setting the object proxy broker pointer.
  /// CORBA::Objects using this stub will use this for standard calls
  /// like is_a; get_interface; etc...
  void object_proxy_broker (TAO::Object_Proxy_Broker *proxy_broker);

  /// Accessor for getting the object proxy broker pointer.
  /// CORBA::Objects using this stub use this for standard calls
  /// like is_a; get_interface; etc...
  TAO::Object_Proxy_Broker *object_proxy_broker () const;

  /**
   * Create the IOP::IOR info. We will create the info at most once.
   * Get the index of the profile we are using to make the invocation.
   */
  int create_ior_info (IOP::IOR *&ior_info, CORBA::ULong &index);

  /// Deallocate the TAO_Stub object.
  /**
   * This method is intended to be used only by the CORBA::Object
   * class.
   */
  void destroy ();

  /// Return the cached value from the ORB_Core.
  /**
   * This flag indicates whether the stub code should make use of the
   * collocation opportunities that are available to the ORB.
   */
  CORBA::Boolean optimize_collocation_objects () const;

  /// Needed to avoid copying forward_profiles for thread safety
  CORBA::Boolean marshal (TAO_OutputCDR&);

  void forwarded_on_exception (bool forwarded);
  bool forwarded_on_exception () const;

protected:
  /// Destructor is to be called only through _decr_refcnt() to
  /// enforce proper reference counting.
  virtual ~TAO_Stub ();

  /// NON-THREAD SAFE version of reset_profiles ();
  void reset_profiles_i ();

  /// NON-THREAD SAFE version of next_profile ()
  TAO_Profile *next_profile_i ();

private:
  /// Makes a copy of the profile and frees the existing profile_in_use.
  /// NOT THREAD SAFE
  TAO_Profile *set_profile_in_use_i (TAO_Profile *pfile);

  /// NON-THREAD-SAFE.  Utility method which resets or initializes
  /// the base_profile list and forward flags.
  void reset_base ();

  /// NON-THREAD-SAFE.  Utility method which unrolls (removes or pops)
  /// the top most forwarding profile list.
  void forward_back_one ();

   /// NOT THREAD-SAFE.  Utility method which pops all forward profile
   /// lists and resets the forward_profiles_ pointer.
  void reset_forward ();

  /// NON-THREAD-SAFE.  utility method for next_profile.
  TAO_Profile *next_forward_profile ();

  /// THREAD-SAFE Create the IOR info
  int get_profile_ior_info (TAO_MProfile &profile, IOP::IOR *&ior_info);

private:
  TAO_Stub (const TAO_Stub &) = delete;
  TAO_Stub (TAO_Stub &&) = delete;
  TAO_Stub &operator = (const TAO_Stub &) = delete;
  TAO_Stub &operator = (TAO_Stub &&) = delete;

protected:
  /// Automatically manage the ORB_Core reference count
  /**
   * The ORB_Core cannot go away until the object references it
   * creates are destroyed.  There are multiple reasons for this, but
   * in particular, the allocators used for some of the TAO_Profile
   * objects contained on each TAO_Stub are owned by the TAO_ORB_Core.
   *
   * This <B>must</B> be the first field of the class, otherwise the
   * TAO_ORB_Core is destroyed too early!
   */
  TAO_ORB_Core_Auto_Ptr orb_core_;

  /// ORB required for reference counting.  This will help us keep the
  /// ORB around until the CORBA::Object we represent dies.
  /**
   * @todo Why do we need both a reference to the ORB_Core and its
   *       ORB? It think the memory management rules for the ORB_Core
   *       changed, in the good old days it was the CORBA::ORB class
   *       who owned the ORB_Core, now it is the other way around....
   */
  CORBA::ORB_var orb_;

  /// Flag that indicates that this stub is collocated (and that it
  /// belongs to an ORB for which collocation optimisation is active).
  CORBA::Boolean is_collocated_;

  /**
   * If this stub refers to a collocated object then we need to hold on to
   * the servant's ORB (which may be different from the client ORB) so that,
   *   1. we know that the ORB will stay alive long enough, and,
   *   2. we can search for the servant/POA's status starting from
   *      the ORB's RootPOA.
   */
  CORBA::ORB_var servant_orb_;

  /// Servant pointer.  It is 0 except for collocated objects.
  TAO_Abstract_ServantBase *collocated_servant_;

  /// Pointer to the Proxy Broker
  /**
    * This cached pointer instance takes care of routing the call for
    * standard calls in CORBA::Object like _is_a (), _get_component
    * () etc.
    */
  TAO::Object_Proxy_Broker *object_proxy_broker_;

  /// Ordered list of profiles for this object.
  TAO_MProfile base_profiles_;

  /// The list of forwarding profiles.  This is actually implemented as a
  /// linked list of TAO_MProfile objects.
  TAO_MProfile *forward_profiles_;

  /// The bookmark indicating permanent forward occurred,
  /// the pointer is used to identify bottom of stack forward_profiles_
  TAO_MProfile *forward_profiles_perm_;

  /// This is the profile that we are currently sending/receiving with.
  TAO_Profile *profile_in_use_;

  /// Mutex to protect access to the forwarding profile.
  TAO_SYNCH_MUTEX profile_lock_;

  /// Have we successfully talked to the forward profile yet?
  CORBA::Boolean profile_success_;

  /// Reference counter.
  std::atomic<uint32_t> refcount_;

  /// The policy overrides in this object, if nil then use the default
  /// policies.
  TAO_Policy_Set *policies_;

  /**
   * The ior info. This is needed for GIOP 1.2, as the clients could
   * receive an exception from the server asking for this info.  The
   * exception that the client receives is LOC_NEEDS_ADDRESSING_MODE.
   * The data is set up here to be passed on to Invocation classes
   * when they receive an exception. This info is for the base
   * profiles that this class stores
   */
  IOP::IOR *ior_info_;

  /// Forwarded IOR info
  IOP::IOR *forwarded_ior_info_;

  /// TRUE if we want to take advantage of collocation optimization in
  /// this ORB.
  /**
   * This should be the same value as cached in the ORB_Core. The
   * reason for caching this helps our generated code, notably the
   * stubs to be decoupled from ORB_Core. Please do not move it away.
   */
  CORBA::Boolean const collocation_opt_;

  /// True if forwarding request upon some specific exceptions
  /// (e.g. OBJECT_NOT_EXIST) already happened.
  std::atomic<bool> forwarded_on_exception_;
};

/**
 * Custom deleter to decrement the refcount when called
 */
struct TAO_Export TAO_Stub_Decr_Refcnt
{
  void operator()(TAO_Stub* stub) const { if (stub) stub->_decr_refcnt(); }
};

/**
 * TAO_Stub_Auto_Ptr will decrement the refcount when going our of scope
 * using std::unique_ptr and a custom deleter
 */
using TAO_Stub_Auto_Ptr = std::unique_ptr<TAO_Stub, TAO_Stub_Decr_Refcnt>;

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Stub.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_STUB_H */
