Quacks' Core
============

Quacks' Core is the name of a game... part of a role-playing game
where some ducks who are spies work together in their spying
organization named Quacks' Core.  This is the code used to implement
the secure login gateway to their electronic computer documents.

So, what kind of technology is used in this secure login gateway?
Basically, this is a set of Windows programs that are designed to (1)
implement a login gateway prompt and (2) encrypt and lock away files
secret location.

For the username and password prompts in the login gateway, we also
additionally have the ability to use a cycling list of "quackwords"
that must be entered, akin to "channel hopping" wireless
communications security.  You are given the current index, and you
must be able to lookup the right codeword and enter it the first time.
If you get it wrong, the index still increases and you must be able to
enter the next codeword in the sequence correctly the first time.
Finally, if there are too many failed login attempts, we force a
restart of the computer on a timed countdown.  That's what you get for
trying to mess with spies, right?

The "encryption" algorithm is rather simple: out of every 8-bit byte,
the most significant bit is flipped, mainly as a safeguard against
anyone opening up the data in a binary editor and immediately seeing
meaningful data.  As for locking away in a secret location, we use the
Windows executable file's resource data object code section to store
away the files.  Windows API provides a nice set of routines for
opening, reading, and modifying the resources inside of existing
binaries.  To avoid modifying a running program, we split up the
resource modification into two stages and use a helper program to
modify the main program... the main program is then later called to
delete the helper program, again to prevent modifying/deleting a
running executable.

One caveat with the encryption is that the passwords also should have
been encrypted, at the very least.  Ideally, they would be stored as
salted hashes, but please note that as one of the goals of this code
was simplicity, this would have to use a rather simple hashing
algorithm.

Why?
====

Because it's fun!  Who has to explain fun?

"But why Windows?" you might ask.  "Nobody has access to Windows
computers as they are quite rare and expensive."  Well, back in the
day, around the year 2008 as you can see this software was written,
computers were a lot rarer and more expensive than they are today.
That meant that even though Microsoft Windows XP was expensive, it was
pretty much the only game in town, especially if you only had one
shared computer at home for the entire family to play with.

Indeed, Windows is a quite impractical operating system to play with
in modern times of the year 2020, and some things like modifying the
resource object code section of binaries don't quite port to modern
Unix-like operating systems as-is, but it is still possible to port
this software to modern Unix-like systems.  The main challenge is that
you have to include all of your own code for object code section
modification within your own source code as there is no standard API
to do this on Unix-like systems, unlike Windows in the case of the
resource data.  Alternatively, you could simplify a bit and assume
that on Unix-like systems, you always have random data files hidden
away in cryptic directories like `/var/lib/...` that are totally
invisible to the non-technical computer user, and just pack the data
away in an independent but still encrypted file.

Are there real user account credentials in this repository?
===========================================================

Yes and no.  The credentials **are** the real ones that were used
historically in the secure login gateway, but this login gateway was
so securely jailed behind a firewall that it was never accessible from
the public Internet.  Besides, the original server that hosted their
accounts has been decommissioned long ago, and they all changed their
passwords in the meantime.  So, you're not going to be able to use
this list of usernames and passwords to break into any modern computer
systems connected to the Internet.
