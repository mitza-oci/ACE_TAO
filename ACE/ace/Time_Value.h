// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_Value.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_TIME_VALUE_H
#define ACE_TIME_VALUE_H

#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_time.h"
#include "ace/Truncate.h"
#include <chrono>
#include <ostream>

// Define some helpful constants.
// Not type-safe, and signed.  For backward compatibility.
#define ACE_ONE_SECOND_IN_MSECS 1000L
suseconds_t const ACE_ONE_SECOND_IN_USECS = 1000000;
#define ACE_ONE_SECOND_IN_NSECS 1000000000L

// needed for ACE_UINT64
#include "ace/Basic_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Time_Value
 *
 * @brief Operations on "timeval" structures, which express time in
 * seconds (secs) and microseconds (usecs).
 *
 * This class centralizes all the time related processing in
 * ACE.  These time values are typically used in conjunction with OS
 * mechanisms like <select>, <poll>, or <cond_timedwait>.
 */
class ACE_Export ACE_Time_Value
{
public:
  /// Constant "0".
  static const ACE_Time_Value zero;

  /**
   * Constant for maximum time representable.  Note that this time is
   * not intended for use with <select> or other calls that may have
   * *their own* implementation-specific maximum time representations.
   * Its primary use is in time computations such as those used by the
   * dynamic subpriority strategies in the ACE_Dynamic_Message_Queue
   * class.
   */
  static const ACE_Time_Value max_time;

  /// Default Constructor.
  ACE_Time_Value ();

  /// Constructor.
  explicit ACE_Time_Value (time_t sec, suseconds_t usec = 0);

  // = Methods for converting to/from various time formats.

  /// Construct the ACE_Time_Value from a timeval.
  explicit ACE_Time_Value (const struct timeval &t);

  /// Construct the ACE_Time_Value object from a timespec_t.
  explicit ACE_Time_Value (const timespec_t &t);

  ACE_Time_Value (const ACE_Time_Value&) = default;
  ACE_Time_Value (ACE_Time_Value&&) = default;

  /// Construct the ACE_Time_Value object from a chrono duration.
  template< class Rep, class Period >
  explicit ACE_Time_Value (const std::chrono::duration<Rep, Period>& duration)
  {
    this->set (duration);
  }

  /// Destructor
  virtual ~ACE_Time_Value () = default;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

# if defined (ACE_WIN32)
  /// Construct the ACE_Time_Value object from a Win32 FILETIME
  explicit ACE_Time_Value (const FILETIME &ft);
# endif /* ACE_WIN32 */

  /// Initializes the ACE_Time_Value from seconds and useconds.
  void set (time_t sec, suseconds_t usec);

  /// Initializes the ACE_Time_Value from a double, which is assumed to be
  /// in second format, with any remainder treated as microseconds.
  void set (double d);

  /// Initializes the ACE_Time_Value from a timeval.
  void set (const timeval &t);

  /// Initializes the ACE_Time_Value object from a timespec_t.
  void set (const timespec_t &t);

# if defined (ACE_WIN32)
  /// Initializes the ACE_Time_Value object from a Win32 FILETIME.
  void set (const FILETIME &ft);
# endif /* ACE_WIN32 */

  /// Initializes the ACE_Time_Value object from a std::duration.
  template< class Rep, class Period >
  void set (const std::chrono::duration<Rep, Period>& duration)
  {
    std::chrono::seconds const s {
      std::chrono::duration_cast<std::chrono::seconds> (duration)};

    std::chrono::microseconds const usec {
      std::chrono::duration_cast<std::chrono::microseconds>(
        duration % std::chrono::seconds (1))};
    this->set (ACE_Utils::truncate_cast<time_t>(s.count ()), ACE_Utils::truncate_cast<suseconds_t>(usec.count ()));
  }

  /// Converts from ACE_Time_Value format into milliseconds format.
  /**
   * @return Sum of second field (in milliseconds) and microsecond field
   *         (in milliseconds).  Note that this method can overflow if
   *         the second and microsecond field values are large, so use
   *         the msec (ACE_UINT64 &ms) method instead.
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  unsigned long msec () const;

  /// Converts from ACE_Time_Value format into milliseconds format.
  /**
   * @return Sum of second field (in milliseconds) and microsecond field
   *         (in milliseconds).
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  ACE_UINT64 get_msec () const;

  /// Converts from ACE_Time_Value format into milliseconds format.
  /**
   * @return Sum of second field (in milliseconds) and microsecond field
   *         (in milliseconds) and return them via the @param ms parameter.
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   *
   * @deprecated Use get_msec() instead.
   */
  void msec (ACE_UINT64 &ms) const;

  /// Converts from ACE_Time_Value format into milliseconds format.
  /**
   * @return Sum of second field (in milliseconds) and microsecond field
   *         (in milliseconds) and return them via the @param ms parameter.
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   *
   * @deprecated Use get_msec() instead.
   */
  void msec (ACE_UINT64 &ms) /* const */;

  /// Converts from milli-seconds format into ACE_Time_Value format.
  /**
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  void set_msec (const ACE_UINT64 &ms);

  /// Converts from milli-seconds format into ACE_Time_Value format.
  /**
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  void msec (long);

  /// Converts from milli-seconds format into ACE_Time_Value format.
  /**
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  void msec (int);  // converted to long then calls above.

  /// Returns the value of the object as a timespec_t.
  operator timespec_t () const;

  /// Returns the value of the object as a timeval.
  operator timeval () const;

  /// Returns a pointer to the object as a timeval.
  operator const timeval *() const;

# if defined (ACE_WIN32)
  /// Returns the value of the object as a Win32 FILETIME.
  operator FILETIME () const;
# endif /* ACE_WIN32 */

  // = The following are accessor/mutator methods.

  /// Get seconds.
  /**
   * @return The second field/component of this ACE_Time_Value.
   *
   * @note The semantics of this method differs from the msec()
   *       method.
   */
  time_t sec () const;

  /// Set seconds.
  void sec (time_t sec);

  /// Get microseconds.
  /**
   * @return The microsecond field/component of this ACE_Time_Value.
   *
   * @note The semantics of this method differs from the msec()
   *       method.
   */
  suseconds_t usec () const;

  /// Set microseconds.
  void usec (suseconds_t usec);

  /**
   * @return Sum of second field (in microseconds) and microsecond field
   *         and return them via the @param usec parameter.
   */
  void to_usec (ACE_UINT64 &usec) const;

  // = The following arithmetic methods operate on ACE_Time_Value's.

  /// Add @a tv to this.
  ACE_Time_Value &operator += (const ACE_Time_Value &tv);

  /// Add @a tv to this.
  ACE_Time_Value &operator += (time_t tv);

  /// Assign @a tv to this
  ACE_Time_Value &operator = (const ACE_Time_Value &) = default;
  ACE_Time_Value &operator = (ACE_Time_Value &&)  = default;

  /// Assign @a tv to this
  ACE_Time_Value &operator = (time_t tv);

  /// Subtract @a tv to this.
  ACE_Time_Value &operator -= (const ACE_Time_Value &tv);

  /// Subtract @a tv to this.
  ACE_Time_Value &operator -= (time_t tv);

  /// Add @a std::duration to this.
  template< class Rep, class Period >
  ACE_Time_Value &operator += (const std::chrono::duration<Rep, Period>& duration)
  {
    const ACE_Time_Value tv (duration);
    this->sec (this->sec () + tv.sec ());
    this->usec (this->usec () + tv.usec ());
    this->normalize ();
    return *this;
  }

  /// Assign @a std::duration to this
  template< class Rep, class Period >
  ACE_Time_Value &operator = (const std::chrono::duration<Rep, Period>& duration)
  {
    this->set (duration);
    return *this;
  }

  /// Subtract @a std::duration to this.
  template< class Rep, class Period >
  ACE_Time_Value &operator -= (const std::chrono::duration<Rep, Period>& duration)
  {
    const ACE_Time_Value tv (duration);
    this->sec (this->sec () - tv.sec ());
    this->usec (this->usec () - tv.usec ());
    this->normalize ();
    return *this;
  }

  /**
    \brief Multiply the time value by the @a d factor.
    \note The result of the operator is valid for results from range
    < (ACE_INT32_MIN, -999999), (ACE_INT32_MAX, 999999) >. Result
    outside this range are saturated to a limit.
     */
  ACE_Time_Value &operator *= (double d);

  /// Increment microseconds as postfix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value operator++ (int);

  /// Increment microseconds as prefix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value &operator++ ();

  /// Decrement microseconds as postfix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value operator-- (int);

  /// Decrement microseconds as prefix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value &operator-- ();

  /// Adds two ACE_Time_Value objects together, returns the sum.
  friend ACE_Export ACE_Time_Value operator + (const ACE_Time_Value &tv1,
                                               const ACE_Time_Value &tv2);

  /// Subtracts two ACE_Time_Value objects, returns the difference.
  friend ACE_Export ACE_Time_Value operator - (const ACE_Time_Value &tv1,
                                               const ACE_Time_Value &tv2);

  /// True if @a tv1 < @a tv2.
  friend ACE_Export bool operator < (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);

  /// True if @a tv1 > @a tv2.
  friend ACE_Export bool operator > (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);

  /// True if @a tv1 <= @a tv2.
  friend ACE_Export bool operator <= (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  /// True if @a tv1 >= @a tv2.
  friend ACE_Export bool operator >= (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  /// True if @a tv1 == @a tv2.
  friend ACE_Export bool operator == (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  /// True if @a tv1 != @a tv2.
  friend ACE_Export bool operator != (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  //@{
  /// Multiplies the time value by @a d
  friend ACE_Export ACE_Time_Value operator * (double d,
                                               const ACE_Time_Value &tv);

  friend ACE_Export ACE_Time_Value operator * (const ACE_Time_Value &tv,
                                               double d);
  //@}

  /// Get current time of day.
  /**
   * @return  Time value representing current time of day.
   *
   * @note    This method is overloaded in the time policy based template
   *          instantiations derived from this class. Allows for time policy
   *          aware time values.
   */
  virtual ACE_Time_Value now () const;

  /// Converts absolute time value to time value relative to current time of day.
  /**
   * @return  Relative time value.
   *
   * @note    This method is overloaded in the time policy based template
   *          instantiations derived from this class. Allows for time policy
   *          aware time values.
   *          The developer is responsible for making sure this is an absolute
   *          time value compatible with the active time policy (which is system
   *          time for the base class).
   */
  virtual ACE_Time_Value to_relative_time () const;

  /// Converts relative time value to absolute time value based on current time of day.
  /**
   * @return  Absolute time value.
   *
   * @note    This method is overloaded in the time policy based template
   *          instantiations derived from this class. Allows for time policy
   *          aware time values.
   *          The developer is responsible for making sure this is a relative
   *          time value. Current time of day is determined based on time policy
   *          (which is system time for the base class).
   */
  virtual ACE_Time_Value to_absolute_time () const;

  /// Duplicates this time value (incl. time policy).
  /**
   * @return  Dynamically allocated time value copy.
   *
   * @note    The caller is responsible for freeing the copy when it's not needed
   *          anymore.
   */
  virtual ACE_Time_Value * duplicate () const;

  /// Dump is a no-op.
  /**
   * The dump() method is a no-op.  It's here for backwards compatibility
   * only, but does not dump anything. Invoking logging methods here
   * violates layering restrictions in ACE because this class is part
   * of the OS layer and @c ACE_Log_Msg is at a higher level.
   */
  void dump () const;

# if defined (ACE_WIN32)
  /// Const time difference between FILETIME and POSIX time.
  static const DWORDLONG FILETIME_to_timval_skew;
# endif /* ACE_WIN32 */

private:
  /// Put the timevalue into a canonical form.
  void normalize (bool saturate = false);

  /// Store the values as a timeval.
#if defined (ACE_HAS_TIME_T_LONG_MISMATCH)
  // Windows' timeval is non-conformant, so swap in a struct that conforms
  // to the proper data types to represent the entire time range that this
  // class's API can accept.
  // Also, since this class can supply a pointer to a timeval that things
  // like select() expect, we need the OS-defined one as well. To make this
  // available, use a real timeval called ext_tv_ and set it up when needed.
  // Since this is most often for relative times that don't approach 32 bits
  // in size, reducing a time_t to fit should be no problem.
  struct {
    time_t tv_sec;
    suseconds_t tv_usec;
  } tv_;
  // Must be mutable as the zero and max static members are const, but are modified
  // in the timeval operations, having it not mutable will cause an access violation
  // with some compilers as they put these in the const section.
  mutable timeval ext_tv_;
#else
  timeval tv_;
#endif /* ACE_HAS_TIME_T_LONG_MISMATCH */
};

extern ACE_Export std::ostream &operator<<(std::ostream &o, const ACE_Time_Value &v );

ACE_END_VERSIONED_NAMESPACE_DECL

// Additional chrono operators.
namespace std
{
  namespace chrono
  {
    /**
    * @name Streaming ACE_Time_Value to chrono
    *
    * Streaming an ACE_Time_Value into one of the chrono types (nanoseconds,
    * microseconds, milliseconds, seconds, minutes, or hours).
    *
    */
    //@{
    ACE_Export nanoseconds& operator <<(nanoseconds &ns, ACE_Time_Value const &tv);
    ACE_Export microseconds& operator <<(microseconds &us, ACE_Time_Value const &tv);
    ACE_Export milliseconds& operator <<(milliseconds &ms, ACE_Time_Value const &tv);
    ACE_Export seconds& operator <<(seconds &s, ACE_Time_Value const &tv);
    ACE_Export minutes& operator <<(minutes &m, ACE_Time_Value const &tv);
    ACE_Export hours& operator <<(hours &h, ACE_Time_Value const &tv);
    //@}

    /**
    * @name Adding ACE_Time_Value to chrono
    *
    * Adding an ACE_Time_Value to one of the chrono types (nanoseconds,
    * microseconds, milliseconds, seconds, minutes, or hours).
    *
    */
    //@{
    ACE_Export nanoseconds& operator +=(nanoseconds &ns, ACE_Time_Value const &tv);
    ACE_Export microseconds& operator +=(microseconds &us, ACE_Time_Value const &tv);
    ACE_Export milliseconds& operator +=(milliseconds &ms, ACE_Time_Value const &tv);
    ACE_Export seconds& operator +=(seconds &s, ACE_Time_Value const &tv);
    ACE_Export minutes& operator +=(minutes &m, ACE_Time_Value const &tv);
    ACE_Export hours& operator +=(hours &h, ACE_Time_Value const &tv);
    //@}

    /**
    * @name Substracting ACE_Time_Value from chrono
    *
    * Substracting an ACE_Time_Value from one of the chrono types (nanoseconds,
    * microseconds, milliseconds, seconds, minutes, or hours).
    *
    */
    //@{
    ACE_Export nanoseconds& operator -=(nanoseconds &ns, ACE_Time_Value const &tv);
    ACE_Export microseconds& operator -=(microseconds &us, ACE_Time_Value const &tv);
    ACE_Export milliseconds& operator -=(milliseconds &ms, ACE_Time_Value const &tv);
    ACE_Export seconds& operator -=(seconds &s, ACE_Time_Value const &tv);
    ACE_Export minutes& operator -=(minutes &m, ACE_Time_Value const &tv);
    ACE_Export hours& operator -=(hours &h, ACE_Time_Value const &tv);
    //@}
  }
}

#if defined (__ACE_INLINE__)
#include "ace/Time_Value.inl"
#endif /* __ACE_INLINE__ */

#if defined (__MINGW32__)
ACE_BEGIN_VERSIONED_NAMESPACE_DECL
// The MingW linker has problems with the exported statics
// zero and max_time with these two statics the linker will be able to
// resolve the static exported symbols.
static const ACE_Time_Value& __zero_time = ACE_Time_Value::zero;
static const ACE_Time_Value& __max_time = ACE_Time_Value::max_time;
ACE_END_VERSIONED_NAMESPACE_DECL
#endif /* __MINGW32__ */

#include /**/ "ace/post.h"

#endif /* ACE_TIME_VALUE_H */
