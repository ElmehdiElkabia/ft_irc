# This project has been created as part of the 42 curriculum by <akemmoun>, <hkemmoun>, <eelkabia>.

# ft_irc

A minimal Internet Relay Chat (IRC) server written in C++98, implementing the core IRC protocol while handling multiple simultaneous clients using non-blocking sockets and I/O multiplexing with `poll()`.

---

# Description

`ft_irc` is a network programming project from the 42 curriculum. The objective is to implement an IRC server compatible with standard IRC clients while following the C++98 standard.

The server uses TCP sockets and an event-driven architecture based on `poll()`, allowing a single thread to manage multiple client connections efficiently. It supports client authentication, channel management, message broadcasting, operator commands, and graceful connection handling.

## Features

* TCP server using IPv4 sockets
* Non-blocking sockets
* I/O multiplexing with `poll()`
* Multiple simultaneous client connections
* IRC client authentication using a server password
* Nickname and username registration
* Channel creation and deletion
* Public channel messaging
* Private messaging
* Channel operators
* Support for common IRC commands, including:

  * PASS
  * NICK
  * USER
  * JOIN
  * PART
  * PRIVMSG
  * INVITE
  * KICK
  * TOPIC
  * MODE

---

# Project Architecture

The server follows the typical TCP server lifecycle:

```
socket()
    ↓
bind()
    ↓
listen()
    ↓
poll()
    ↓
accept()
    ↓
recv()
    ↓
Process IRC command
    ↓
send()
    ↓
close()
```

The listening socket accepts new client connections, while each connected client is represented by its own socket descriptor.

The `poll()` system call monitors all sockets simultaneously and notifies the server whenever:

* a new client is attempting to connect,
* a client has sent data,
* a client disconnects,
* a socket becomes ready for writing.

This event-driven approach allows the server to efficiently handle many clients without creating multiple threads.

---

# Instructions

## Requirements

* Linux
* C++98 compiler
* Make

## Compilation

```bash
make
```

This generates the executable:

```bash
./ircserv
```

## Usage

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 password42
```

## Connecting with an IRC client

Using irssi:

```bash
irssi
```

Inside irssi:

```
/connect localhost 6667 password42
```

or

```
/connect 127.0.0.1 6667 password42
```

After connecting:

```
/join #general
```

---

# Project Structure

```
.
├── includes/
├── src/
├── Makefile
├── README.md
└── ircserv
```

---

# Technical Choices

* Language: C++98
* Communication protocol: TCP
* Address family: IPv4 (`AF_INET`)
* Socket type: `SOCK_STREAM`
* Event management: `poll()`
* Non-blocking sockets using `fcntl()`
* Single-threaded event loop
* STL containers for managing clients and channels

---

# Resources

## IRC Protocol

* RFC 1459 — Internet Relay Chat Protocol
* RFC 2810 — IRC Architecture
* RFC 2811 — Channel Management
* RFC 2812 — Client Protocol
* RFC 2813 — Server Protocol

## Socket Programming

* Linux `man` pages:

  * socket(2)
  * bind(2)
  * listen(2)
  * accept(2)
  * poll(2)
  * recv(2)
  * send(2)
  * fcntl(2)

## C++

* cppreference (C++98-compatible features)
* ISO C++98 Standard

---

# AI Usage

Artificial intelligence tools were used as a learning aid throughout the project.

AI assistance included:

* Understanding TCP/IP networking concepts
* Explaining the behavior of Linux sockets
* Learning how `poll()` and non-blocking I/O work
* Reviewing the IRC protocol (RFCs)
* Clarifying edge cases and debugging networking behavior
* Improving code documentation and this README

All architectural decisions, implementation, debugging, testing, and final code were completed and validated by the project authors.
