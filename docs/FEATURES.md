
Some compatibility details for afpfs-ng 0.8, February 18, 2008.


A. Login methods
----------------

The following UAMs are implemented:

- Cleartxt Passwrd
- No User Authent
- Randnum Exchange*
- 2-Way Randnum Exchange*
- DHCAST128*
- DHX2*

However, only those with a (*) will exist if you build with libgmp and
libgcrypt.  By default, Mac OS X 10.5 and later only support those with a (*).
It is possible to enable cleartext passwords in those versions, but this is
not a great idea.

The following UAMs have not yet been implemented:

- kerberos, this requires integration with a KDC
- reconnect, it is a bit unclear how this should be done with session keys.
  This isn't properly described in the docs.  It also isn't really a UAM.

Password changing isn't implemented, although it has been roughed in.
The interface would be in afpcmd.

There is no support for open directory.

'status' will show you what UAMs are compiled in and what is being used.

B. Connect, disconnect
----------------------

There are basic facilities to receive and send session keys, but these are not
used.

Server disconnect and reconnect isn't currently working.  Right now, there's a
random token that gets sent, but that's it.

The client doesn't recover if the server goes down partway through a transaction.

C. UID and GID mapping
----------------------

One area of complication is around UID and GID mappings.  These may differ
between the client and server.  There are two modes that are enabled in
afpfs-ng:

### Common user directory

This is where both the client and server have
identical UIDs and GIDs.  This is the case where you have an NIS server
between them, or some other common directory.

### Login IDs

This is where all the files appear as the user that logged in.  This would
typically be used where the databases are separate, and one user expects to
be able to read/write all the files he sees.  This can get confusing, since
any files that aren't his will appear to be owned by him, but writing to
them will result in an EPERM.

### Named mapping

This is where the name (not uid) of the owner is mapped correctly.  This is
not implemented.

### Mapping from file

This is where a file is read that translates client and server ids.  This is
not implemented.

afpfs-ng attempts to detect the mapping type automatically; do 'afp_client
status' (for FUSE monts) or 'status' within afpcmd to see what it guessed.


D. Meta information
-------------------

### Server icon

A readonly copy of the server icon can be found in /.servericon.

### Resource forks

Every directory has a hidden directory called .AppleDouble, and if a resource
fork exists, you'll find it there.  As an example, the resource fork for
/foo/bar/testfile can be found in /foo/bar/.AppleDouble/testfile.

The permissions of the resource fork are the same as the data fork.

### Desktop functions

a) Comments
The only desktop function that's actually implemented is comments.  For file
/dir/foo, they can be found in /dir/.AppleDouble/file.comment

Permissions for the comment are the same as the data fork.

1. Catalog searching
2. Icon searching
3. APPL

None of these are really suitable as a filesystem.  But you could get access
to them if you wrote your own client.

4. Finder info

Finder info for files and directories for /dir/foo can be found in
/dir/.AppleDouble/foo.finderinfo.


E. ACLs and extended attributes
-------------------------------

ACLs and extended attributes have not been implemented.


F. Internationalization
-----------------------

For servers that support it, UTF8 usernames, server names, volume names and files are supported.

Older clients (Mac OS 9) that don't use filenames of type long.  Other
charsets for files (MacRomanian, etc) are not supported properly.  Servernames
are supported.

G. Networks
-----------

IPv6: As of v0.8.2, we have support for IPv6.
IPv4: Yes, of course.
Appletalk: There is no support for Appletalk.

There's no concept of multiple protocols, eg. doing getstatus with one protocol, 
then connecting with another, which is what some Apple clients do.

There's no ability to connect based on a name advertized by Bonjour/Avahi, you
need to use the IP or DNS name.


H. Server-specific information
------------------------------

afpfs-ng detects the server type by parsing the Machine Type field in
getstatus.  The command line 'afpgetstatus' will show this without you having
to log in.  'status' will show you this also.

The detection is done in order to deal with some details.

### Mac OS 8

afpfs-ng has never been used with Mac OS 8, so there's no data.  You could do
this with AFP over TCP/IP, but this could be difficult. Email me if you have any info.

### Mac OS 9

This speaks AFP 2.1, so this presents certain restrictions, such as:

- smaller limits on file and disk sizes (4GB)
   - creating files larger than 2GB isn't possible and isn't handled properly
   - 'df' will report a max of 4GB.
- no support for Unix privileges; all files are reported as 0600, directories
  as 0700.
- for directories, timestamps are reported as the mount times, which is what
  the Mac OS X client does.

There is no proper charset conversions for filenames.  Patches accepted.

This has been lightly tested. 

### Mac OS X

Various versions have been tested, including 10.2, 10.3, 10.4 and 10.5.x. This has been most 
heavily tested.  Note the restrictions on UAMs above.

10.5 introduces AFP function 76, but there's no documentation on this.  Too
bad.

### Airport Extreme

The airport extreme with firmware 7.1 and 7.2.1 has been tested, and has two
oddities:

- Unix permissions aren't handled at all

- the device has a software bug which can let an authenticated user freeze the
  device.  I won't describe the problem in any more detail.  Apple has
  acknowledged the problem, but hasn't yet released updated firmware.

Note that the Airport can serve up SMB and AFP disks; afpfs-ng only handles
AFP.

### Time Capsule

The Time Capsule is a network backup device meant to handle Time Machine
backups over AFP.  This hasn't been released and my wife won't let me buy one,
so there's been no testing.  Donations appreciated.

### Netatalk

This open source server has a few issues, and afpfs-ng tries to work around
them.  The most significant one is around file permissions; there's a bug in
older versions whereby some permissions cannot be set with a chmod (particularly 
the execute bit on files).

It becomes a bit difficult to identify if you have the newer or older version
of netatalk since it has been changed in CVS, and occurred after 2.0.3.  2.0.4
hasn't been released yet (almost 3 years later).  Some distributions (such as
Fedora 8) ship a broken version.

There's a patch available on the afpfs-ng download site, although grabbing a
later version of netatalk from CVS will work also.

After you attempt to 'chmod +x foo', 'status' will show you if it is broken or
not.

### LaCie devices

The LaCie device has an ARM processor in it, and speaks netatalk.  Part of the
problem with that some login crypto is so slow that older versions of afpfs-ng
timeout before the server can complete the crypto.  This should be fixed as of
0.8, but this hasn't been tested properly.


I. FUSE-specific bugs
---------------------

There are no facilities for automounting home directories, which is something
that people ask for frequently.  This requires having integration into open
directory.

There are some bugs around race conditions that can make heavy load operations
(eg. compiling a kernel) freeze or stall.  

Testing has been done based on FUSE 2.7.0.

J.Other
-------
- length of file is currently fixed at 255; this isn't correct for AFP >3.0

K. References
-------------

Not all references are easy to find. The useful ones are:

- Apple Filing Protocol Programming Guide, Version 3.2, 2005-06-04
- AppleTalk Filing Protocol, Versions 2.1 and version 2.2., Apple Computer Inc, 1999
- Inside Macintosh, Macintosh Toolbox Essentials, 1992

And other software:

- netatalk: Netatalk is the server side, and it implements AFP 3.1 over
  Appletalk and TCP/IP. It has a long history and has been heavily tested, but
  is creative in some of its implementation.

- afpfs: The original afpfs was last release for Linux kernel 2.2.5 and was

  written in kernel space. It was written by Ben Hekster, then taken over by
  David Foster. I think it was last maintained in 1999 and only handled AFP 2.x.
  Truly, afpfs-ng was intended to take over where they left off, but this is a
  complete rewrite in order to fit into FUSE.

- hfsplus: This might not seem related, but the way that hfsplus handles

  presenting resource forks to userspace may be relevant to how afpfs-ng does
  the same.

- wireshark (aka ethereal): the AFP packet parsing is excellent
