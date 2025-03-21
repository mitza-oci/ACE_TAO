// -*- C++ -*-

//=============================================================================
/**
 *  @file    Countdown_Time_T.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_COUNTDOWN_TIME_T_H
#define ACE_COUNTDOWN_TIME_T_H

#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"
#include "ace/Time_Policy.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Countdown_Time
 *
 * @brief Keeps track of the amount of elapsed time.
 *
 * This class has a side-effect on the @c max_wait_time -- every
 * time the stop() method is called the @c max_wait_time is
 * updated.
 */
template <typename TIME_POLICY = ACE_Default_Time_Policy>
class ACE_Countdown_Time_T
{
public:
  /// Cache the @a max_wait_time and call @c start().
  ACE_Countdown_Time_T (ACE_Time_Value *max_wait_time,
                        TIME_POLICY const & time_policy = TIME_POLICY());

  ACE_Countdown_Time_T (const ACE_Countdown_Time_T &) = delete;
  ACE_Countdown_Time_T (ACE_Countdown_Time_T &&) = delete;
  ACE_Countdown_Time_T &operator= (const ACE_Countdown_Time_T &) = delete;
  ACE_Countdown_Time_T &operator= (ACE_Countdown_Time_T &&) = delete;


  /// Destructor, makes sure the max_wait_time that got passed as pointer
  /// to the constructor is updated with the time elapsed.
  ~ACE_Countdown_Time_T ();

  /// Cache the current time and enter a start state.
  void start ();

  /// Subtract the elapsed time from max_wait_time_ and enter a stopped
  /// state.
  void stop ();

  /// Calls stop and then start.  max_wait_time_ is modified by the
  /// call to stop.
  void update ();

  /// Returns true if we've already been stopped, else false.
  bool stopped () const;

  /// Allows applications to control how the timer queue gets the time
  /// of day.
  void set_time_policy(TIME_POLICY const & time_policy);

private:
  /// The policy to return the current time of day
  TIME_POLICY time_policy_;

  /// Maximum time we are monitoring
  ACE_Time_Value *max_wait_time_;

  /// Copy of the maximum time value, used to avoid nested decrements
  ACE_Time_Value max_wait_value_;

  /// Beginning of the start time.
  ACE_Time_Value start_time_;

  /// Keeps track of whether we've already been stopped.
  bool stopped_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#if defined (__ACE_INLINE__)
#include "ace/Countdown_Time_T.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Countdown_Time_T.cpp"

#endif /* ACE_COUNTDOWN_TIME_T_H */
