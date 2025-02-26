# Architecture

```
                                +------+------+------+
                                | fuse | kio  | gio  |
                                +--------------------+  
                                             afpsl.h, afpfsd_conn
                                +--------------------+
                                | afpfsd             |
                                +--------------------+


                  +----------+
                  | cmdline  |
   afp.h          +----------+---+---+---------+
                  | midlevel |   |   | command |
   libafpclient   +----------+   |   |
                  |   lowlevel   |   |
                  +--------------+   |
                  |   proto_*        |
                  +------------------+
                  | Engine                     |
                  +----------------------------+
```

# How afpfs-ng works

The Apple Filing Protocol is a network filesystem that is commonly used 
to share files between Apple Macintosh computers.

A network connection must be established to a server and maintained.  
afpfs-ng provides a basic library on which to build full clients 
(called libafpclient), and a sample of clients (FUSE and a simple 
command line).

# The components

## libafpclient

This is a shared library (libafpclient.so) that implements the basic DSI 
and AFP communication requirements for connecting to AFP servers.  An 
AFP client uses this library through several APIs, defined later.

You should use libafpclient in a situation where you have a stateful process.
This means that the process that's handling your client lives for the duration
of the transactions required.

A key point to know when building libafpclients is that libafpclient will
spawn threads and override signals.  Asynchronous events need to be
hooked into a loop provided by libafpclient.  You cannot write your own
select() loop!

The major subcomponents of libafpclient are all in the lib/ directory.

They are:

### Midlevel

This is an API that simplifies the AFP functions that does some simplification
of the protocol, such as calling multiple AFP functions to perform a basic
task.  This is the most likely API set to use when using libafpclient
directly.

Typically, a midlevel function will:

- translate filenames for you
- handle metainformation (resource forks, special files)
- call the lowlevel function

### Lowlevel

This is an API that handles many AFP functions, while taking care of some
AFP details, such as behaviour differences between AFP versions and 
situations where servers don't adhere to the exact protocol.

An example of this is when listing a directory; ll_readdir() will 
figure out what AFP version is being used, and either call protocols
afp_enumerateext() for AFP 2.x or afp_enumerateext2 for 3.x (which can 
handle larger file lists).

These are implemented in lib/midlevel.c.  The API is exposed in midlevel.h.

You should generally not use these functions.

### Protocol

This is the raw API that exposes individual AFP functions, this 
includes things like afp_listextattr().

These are implemented in lib/proto_* files and exposed in afp.h.

You should almost never use this set of functions.

Other topics
- startup
- metainformation
- scheduling
