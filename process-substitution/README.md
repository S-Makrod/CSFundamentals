<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta name="viewport" content="initial-scale=1">
</head>
<body>
<h2>Process Substitution</h2>

<p>bash has a convenient extra feature “process substitution” not in sh. In the
simplest form it goes like

<pre>
command1 &lt;(command2)
</pre>

<p>command1 will see a command line argument. We users don't see it, but
command1 does. Let's call it F because I need to refer to it later.  command1
can treat F as a filename and open it for reading (only); command2 can write
data to stdout. Here is the magical part: Whenever command1 reads from “file” F,
it sees command2's output!

<p>But it can be more general!

<pre>command1 &lt;(command2a) &lt;(command2b)
</pre>

<p>The above means: There are secret filenames F2a and F2b. When command1 reads
from F2a, that's command2a's output; when command1 reads from F2b, that's
command2b's output.

<pre>command1 &lt;(command2 &lt;(command3))
</pre>

<p>The above means: There are secret filenames F and G. When commmand1 reads
from F, that's command2's output; when command2 reads from G, that's command3's
output.

<p>Many students used this feature to evade learning a required topic. The prof
is not pleased. The prof's revenge is to make you implement this feature so you
have a harder topic to learn!

<p>(On the bright side, haven't you always wondered how bash pulls off this
trick?)


<h3>Discovery (optional, not graded)</h3>

<p>Write a shell script or C program that prints the command line arguments it
sees. It doesn't actually need to read any file. Use it as command1. This is to
discover the secret F to help find out what's going on. Example bash command:

<pre>
./yourprog &lt;(echo hello)
</pre>

<p>What do you find? What do you think it means?


<h3>Theory</h3>

<p>/dev/fd is a special directory (not on real disk) made up by the
kernel. Whenever a process P looks into it, P finds numeric filenames, and the
numbers are exactly P's current open file descriptors. And if P reads from, say,
/dev/fd/0, it's equivalent to reading from file descriptor 0.

<p>(Naturally, different processes see different content, since the kernel is
making it up on the fly, tailor-made to the process that asks.)

<p>With this friendly service from the kernel, bash can pull off its trick.  (It
is still a pipe.)  We assume that commands are executable programs with
arguments in this assignment, even though bash is much more flexible than that.

<p>For example, here is how to do <code>command1 &lt;(command2)</code>:

<ul>
  <li><p>Create a pipe. Let's say the read-end FD is 42, write-end FD is 43, for
    the sake of concreteness, but this is just an example.

  <li><p>Create a child process to run command1 with argument /dev/fd/42 (the
    secret F above).  So if command1 opens that “file” and read, it effectively
    reads from the pipe.  Note that this is <b>not</b> stdin redirection.

  <li><p>Create a child process to run command2 with stdout redirected as FD 43.
    So if command2 writes to stdout, the data goes to the pipe.

  <li><p>All 3 processes should also close certain unneeded file descriptors, as
    discussed in the lecture concerning pipe hygiene. It is your job to figure
    out which and when.
</ul>


<h3>Implementation</h3>

<p>The function you will implement (in procsub.c) is:

<pre>
int run(struct command *cmd, int *wstatus);
</pre>

<p><code>struct command</code> specifies a command; see
<a href="procsub.h">procsub.h</a>, but informally it has: program name,
number of arguments, and (pointer to) the arguments.

<p><code>struct argument</code> specifies an argument,
see <a href="procsub.h">procsub.h</a>; informally it has two cases:
a string, or a command (meaning that process substitution is to be done).
We will only implement the <code>&lt;(cmd)</code> kind of substitutions.

<p>Further clarifications can be found as examples in
<a href="sample-main.c">sample-main.c</a> with comments stating the bash
equivalents.  In particular, if the bash equivalent is of the
form <code>progname foo</code>, then the <code>numargs</code> field is 1, and
the <code>args</code> array has just one element (for <code>foo</code>); so
this is different from what <code>exec()</code> expects, and you have to bridge
the gap.

<p><code>run()</code> should run the given command in a child process, wait for
it to terminate, and store its wait status at the address given
by <code>wstatus</code> (you may assume that it is non-NULL).  Needless to say,
extra children are also necessary for arguments of the <code>SUBST</code>
case; for simplicity, you do not need to call <code>wait()</code>
or <code>waitpid()</code> on those other children.

<p><code>run()</code> should return 0 except under the conditions in the next
section.


<h3>Error handling</h3>

<p>Any child: If <code>exec()</code> fails, exit with exit code 127. Error
message is up to you, but send any to stderr.

<p>Other errors: You may ignore or handle, your choice.  If you choose to
handle: 1. send any error message to stderr; 2. if the error occurs
during <code>run()</code> (in parent), return -1; 3. if the error occurs in a
child, exit with exit code 127.


<!-- <h3>Clarifications, Errata</h3> -->


</body>
</html>
