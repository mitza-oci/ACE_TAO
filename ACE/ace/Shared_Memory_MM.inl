// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/// Return the name of file that is mapped (if any).
ACE_INLINE const ACE_TCHAR *
ACE_Shared_Memory_MM::filename () const
{
  return this->shared_memory_.filename ();
}

ACE_INLINE int
ACE_Shared_Memory_MM::open (ACE_HANDLE handle,
                            size_t length,
                            int prot,
                            int share,
                            char *addr,
                            ACE_OFF_T pos)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::open");
  return shared_memory_.map (handle, length, prot, share, addr, pos);
}

ACE_INLINE int
ACE_Shared_Memory_MM::open (const ACE_TCHAR *file_name,
                            size_t len,
                            int flags,
                            int mode,
                            int prot,
                            int share,
                            char *addr,
                            ACE_OFF_T pos)
{
  ACE_TRACE ("ACE_Shared_Memory_MM::open");
  return shared_memory_.map (file_name, len, flags, static_cast<mode_t> (mode),
                             prot, share, addr, pos);
}

ACE_END_VERSIONED_NAMESPACE_DECL
