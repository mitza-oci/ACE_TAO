// -*- C++ -*-

//==========================================================================
/**
 *  @file    Auto_Event.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//==========================================================================

#ifndef ACE_AUTO_EVENT_H
#define ACE_AUTO_EVENT_H
#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Auto_Event
 *
 * @brief Auto Events.
 *
 * Specialization of Event mechanism which wakes up one waiting
 * thread on @c signal.  All platforms support process-scope locking
 * support.  However, only Win32 platforms support global naming and
 * system-scope locking support.
 */
template <class TIME_POLICY = ACE_System_Time_Policy>
class ACE_Auto_Event_T : public ACE_Event_T<TIME_POLICY>
{
public:
  /// Constructor which will create auto event
  ACE_Auto_Event_T (int initial_state = 0,
                    int type = USYNC_THREAD,
                    const char *name = nullptr,
                    void *arg = nullptr);

#if defined (ACE_HAS_WCHAR)
  /// Constructor which will create auto event (wchar_t version)
  ACE_Auto_Event_T (int initial_state,
                    int type,
                    const wchar_t *name,
                    void *arg = nullptr);
#endif /* ACE_HAS_WCHAR */

  /// Default dtor.
  ~ACE_Auto_Event_T () override = default;

  /// Dump the state of an object.
  void dump () const;

  /// Declare the dynamic allocation hooks
  ACE_ALLOC_HOOK_DECLARE;
};

class ACE_Auto_Event :
  public ACE_Auto_Event_T<ACE_System_Time_Policy>
{
public:
  /// Constructor which will create auto event
  ACE_Auto_Event (int initial_state = 0,
                  int type = USYNC_THREAD,
                  const char *name = nullptr,
                  void *arg = nullptr)
  : ACE_Auto_Event_T<ACE_System_Time_Policy> (initial_state, type, name, arg)
  {
  }

#if defined (ACE_HAS_WCHAR)
  /// Constructor which will create auto event (wchar_t version)
  ACE_Auto_Event (int initial_state,
                  int type,
                  const wchar_t *name,
                  void *arg = nullptr)
  : ACE_Auto_Event_T<ACE_System_Time_Policy> (initial_state, type, name, arg)
  {
  }
#endif /* ACE_HAS_WCHAR */

  /// Default dtor.
  ~ACE_Auto_Event () override = default;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include "ace/Auto_Event.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_AUTO_EVENT_H */
