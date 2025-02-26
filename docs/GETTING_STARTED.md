This is a quick guide on how to use the two different afp clients 
in afpfs-ng.


# The FUSE client

This will let you mount remote filesystems. 

As the user who will be needing to access, the file, run the management daemon
by running:

```
afpfsd
```

This should fork off.  You should see messages in /var/log/messages.  For more
details, run it with the '-d' option to see detailed debug info.

To mount a filesystem:

```
mount_afpfs "afp://username:password@servername/volumename" <mountpoint>
```

After this, you should be able to access files on <mountpoint>.

You can see status by running 'afp_client status'.  See afpfsd(1), 
mount_afpfs(1) and afp_client(1) for more info.

To add an AFP mount to fstab so it mounts automatically on boot:

1. create a file called '/etc/fuse.conf' with one line: 
user_allow_other

2. make sure that any user doing a mount is a member of the group 'fuse' so it can read and write to /dev/fuse

3. create an entry in /etc/fstab entry in the following format:
afpfs#afp://username:mypass10.211.55.2/alexdevries /tmp/xa20 fuse user=adevries,group=fuse 0 0 

Here, username and mypass are the login information on the server 10.211.55.2.
The volume name is alexdevries.  /tmp/xa20 is the name of the mountpoint.  
The user= field is the local user, group needs to be the same the group owner of /dev/fuse (which is typically fuse).

Yes, you will need to put your password in clear text.  There is currently no facility to handle open directory.  Patches welcome. 

# Running the command line client

There are two modes:

## interactive mode

afpcmd is a command line tool like an FTP client.

Just run:

```
afpcmd "afp://username:password@servername/volumename"
```

(if you enter no volumename, it shows which ones are available.  If you 
provide no URL, you can use 'connect'. If you provide a '-' as the
password, you will be prompted for one interactively)

Available commands are:

- get <filename>: retrieves the filename
- get -r <dirname>: recursively retrieves the directory and its contents
- put <filename>: send the file
- dir: show directory listings

Others are available too; touch, chmod, chown, del, rename, etc.  See 
afpcmd(1) for more.

## batch transfer

This will let you quickly transfer one file or recursively a directory,
and then return you to the command prompt.

E.g.

```
> afpcmd afp://user:pass@server/alexdevries/linux-2.6.14.tar.bz2
Connected to server Cubalibre using UAM "DHX2"
Connected to volume alexdevries
    Getting file /linux-2.6.14.tar.bz2
Transferred 39172170 bytes in 2.862 seconds. (13687 kB/s)
```

See afpcmd(1) for more information.

## getting status

You can get status information on servers with 'afpgetstatus <servername>'.  
This provides some information without having to log in.

See afpgetstatus(1) for more information.
