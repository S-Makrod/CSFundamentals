<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml"
      lang="en"
      xml:lang="en"
>
<head>
    <meta charset="utf-8" />
    <meta name="generator" content="pandoc" />
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=yes" />
</head>
<body id="top">
    <header class="title">
        <h1 class="title">Sockets Application</h1>
    </header>
    <h2 id="comp">Example Compilation</h2>
    <pre>
gcc client.c -o client
./client 127.0.0.1 31337
gcc server .c -o server
./server sample.dat 31337 </pre>
    <h2 id="remote-suncent-query-service">Remote Suncent Query Service</h2>
    <p>The protocol in this assignment allows clients to look up suncents of Solarbucks customers from a server. In short, a client sends a name to the server, and the server looks up in its customer file and replies with suncents or not-found.</p>
    <p>Since stream sockets are used, we can’t rely on packet boundaries for message boundaries. We use a newline to mark the end of a message.</p>
    <p>Here is the detailed protocol sequence (assuming neither clients nor servers malfunction):</p>
    <ol>
    <li><p>The client connects to the server.</p></li>
    <li><p>Steps 3-4 below can happen 0 or more times before step 5.</p></li>
    <li><p>The client sends a name (1 to 27 bytes) followed by a newline.</p></li>
    <li><p>If the name is found in the server’s customer file, the server sends the suncent amount as a decimal string (at most 10 digits) followed by a newline.</p>
    <p>If the name is not found, the server sends <code>none</code> followed by a newline.</p></li>
    <li><p>The client disconnects.</p></li>
    </ol>
    <h2 id="question-1-client-4-marks">Question 1: Client</h2>
    <p>(Please submit client.c.)</p>
    <p>Implement a client program that takes names from the user and queries the server for suncents.</p>
    <p>There are 2 command line arguments: server address in dot notation, server port number. (When marking, they are always valid.)</p>
    <p>If connection is unsuccessful, you may print an error message of your choice to stderr. Exit with exit code 1.</p>
    <p>If connection is successful:</p>
    <ol type="1">
    <li><p>Print <code>Ready</code> on its own line to stdout. (So the user knows.)</p></li>
    <li><p>Read a line from stdin. You may assume that it is at most 28 bytes including newline.</p></li>
    <li><p>If EOF or a blank line (just a newline and no other character), close the connection and exit with exit code 0.</p></li>
    <li><p>Send the name to the server. Print the server’s reply to stdout. Don’t print more than one newline.</p></li>
    <li><p>Go to 2.</p></li>
    </ol>
    <p>The lack of prompting is to simplify automarking. If you prefer to add prompting, please use stderr.</p>
    <p>Here is a sample session:</p>
    <table>
    <thead>
    <tr class="header">
    <th>stdin</th>
    <th>stdout</th>
    </tr>
    </thead>
    <tbody>
    <tr class="odd">
    <td></td>
    <td><code>Ready</code></td>
    </tr>
    <tr class="even">
    <td><code>Archimedes</code></td>
    <td><code>290</code></td>
    </tr>
    <tr class="odd">
    <td><code>Trebla Lai</code></td>
    <td><code>none</code></td>
    </tr>
    <tr class="even">
    <td><code>Dennis Ritchie</code></td>
    <td><code>1926</code></td>
    </tr>
    </tbody>
    </table>
    <p>Note that there is no blank line after the suncent amount or <code>none</code>.</p>
    <p>Server malfunctions happen all the time due to bugs and service disruptions. Here are the scenerios you must handle as prescribed:</p>
    <ul>
    <li><p>Connection failure is covered above.</p></li>
    <li><p>Error or EOF when sending to or receiving from server: This means disconnection. The client should exit with exit code 1 or get killed by SIGPIPE. You may print error messages to stderr.</p></li>
    <li><p>After receiving 11 bytes from the server you still don’t see a newline: Server bug. The client should not treat it as a valid reply. The client should exit with exit code 1. You may print error messages to stderr.</p></li>
    </ul>
    <h2 id="question-2-server-6-marks">Question 2: Server</h2>
    <p>(Please submit server.c.)</p>
    <p>Implement a server program that replies to client queries based on data in a customer file.</p>
    <p>There are 2 command line arguments: pathname (of the customer file), port number. (When marking, they are always valid.)</p>
    <p>The format of the customer file is as in A2. (You may copy code from your or my A2 solution.)</p>
    <p>The server should bind to the given port at address INADDR_ANY. We do expect this to fail all the time due to the “address already in use” error. If this happens, print an error message to stderr and exit with exit code 1.</p>
    <p>A server should be responsive to multiple existing and incoming clients concurrently, even when a bad client stalls and wants the server to wait forever; bad clients happen all the time by bad luck, bugs, or malice. Well-known approaches are: forking a child process for each client, so the parent is just an accept-fork loop; or multiplexing by <code>select()</code> or <code>epoll()</code>; or multi-threading. You may choose which approach you want. (Forking is the easiest.)</p>
    <p>The server should not exit until SIGTERM or SIGINT. Upon those signals, it should be killed by the signal or exit with exit code 0. (If you use forking, obviously this is required of the parent process only.)</p>
    <p>The protocol specifies that it is the client that initiates disconnection. Therefore, the server side trying to send or receive then gets an error, EOF, or SIGPIPE. This is part of normal workflow; this is not supposed to bring down your server.</p>
    <p>If you use forking: Zombie processes should not happen. And yet, the parent process should not hang indefinitely to wait for a child to die, since it must also stay responsive to new connection requests.</p>
    <p>Busy polling is disallowed. Marking will be done under a tight limit on CPU time.</p>
    <p>Client malfunctions happen all the time, even more than server malfunctions. We expect the Internet to be full of both fools and trolls. Here are the scenerios you should handle as prescribed:</p>
    <ul>
    <li><p>Client disconnects prematurely: As covered above.</p></li>
    <li><p>After receiving 28 bytes from the client you still don’t see a newline: For simplicity, disconnect the client.</p></li>
    </ul>
</body>
</html>
