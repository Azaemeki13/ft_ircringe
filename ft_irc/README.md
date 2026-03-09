# ft_irc

*This project has been created as part of the 42 curriculum by chsauvag, adi-marc, cauffret.*

## Description

ft_irc is a custom IRC (Internet Relay Chat) server implementation in C++98. It handles multiple clients simultaneously using epoll for efficient I/O multiplexing and implements core IRC protocol features.

**Key Features:**
- Client authentication and registration
- Channel creation and management  
- Private messaging
- Channel operations (join, kick, invite, topic, modes)
- Non-blocking I/O with epoll
- IRC protocol (RFC 1459/2812) compliance

## Instructions

### Compilation

```bash
make          # Compile the project
make clean    # Remove object files
make fclean   # Remove all generated files
make re       # Recompile everything
```

This generates the `ircserv` executable.

### Running the Server

```bash
./ircserv <port> <password>
```

Example:
```bash
./ircserv 6969 mypassword
```

Stop the server with `Ctrl+C`.

### Connecting

Using irssi:
```bash
irssi
/connect localhost 6969 mypassword
```

Using netcat:
```bash
nc localhost 6969
PASS mypassword
NICK yournickname
USER username 0 * :Real Name
```

### Available Commands

- **PASS** - Set connection password
- **NICK** - Set or change nickname
- **USER** - Set user information
- **JOIN** - Join a channel
- **PRIVMSG** - Send messages to users or channels
- **KICK** - Remove a user from a channel (operators only)
- **INVITE** - Invite a user to a channel
- **TOPIC** - View or change channel topic
- **MODE** - Change channel modes (i, t, k, o, l)
- **LIST** - List available channels
- **PING/PONG** - Keep-alive mechanism

### Bonus: IRC Bot

Run the included Python bot:
```bash
make bonus    # Launch the bot automatically
# OR
python3 bot.py
```

Bot commands: `>help`, `>labubu`, `>skibidi`, `>67`

Configure connection settings in `bot.py` (HOST, PORT, PASSWORD).

## Resources

### Documentation
- [RFC 1459](https://tools.ietf.org/html/rfc1459) - IRC Protocol
- [RFC 2812](https://tools.ietf.org/html/rfc2812) - IRC Client Protocol
- [Modern IRC Docs](https://modern.ircdocs.horse/)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [epoll man page](https://man7.org/linux/man-pages/man7/epoll.7.html)

### AI Usage

AI tools (ChatGPT, GitHub Copilot) were used for:
- Understanding IRC protocol specifications and RFC documentation
- Debugging socket handling and epoll edge cases
- C++98 compliance verification
- Test case design

**Note**: All core implementation, logic, and architecture decisions were written by the project authors. AI was used as a learning resource similar to documentation.

## Project Structure

```
ft_irc/
├── includes/           # Header files
│   ├── Channel.hpp    # Channel class definition
│   ├── Client.hpp     # Client class definition
│   ├── Commands.hpp   # Command declarations
│   ├── Parser.hpp     # Message parser
│   ├── Replies.hpp    # IRC numeric replies
│   ├── Server.hpp     # Server class definition
│   └── Utils.hpp      # Utility functions
├── src/               # Source files
│   ├── main.cpp       # Entry point
│   ├── Server.cpp     # Server implementation
│   ├── Client.cpp     # Client implementation
│   ├── Channel.cpp    # Channel implementation
│   ├── Parser.cpp     # Message parser implementation
│   ├── Utils.cpp      # Utility functions
│   └── commands/      # IRC command implementations
│       ├── cmd_pass.cpp
│       ├── cmd_nick.cpp
│       ├── cmd_user.cpp
│       ├── cmd_join.cpp
│       ├── cmd_privmsg.cpp
│       ├── cmd_kick.cpp
│       ├── cmd_invite.cpp
│       ├── cmd_topic.cpp
│       ├── cmd_mode.cpp
│       ├── cmd_list.cpp
│       └── cmd_pong.cpp
├── tests/             # Test scripts
│   ├── tests.py
│   ├── flood.sh
│   └── flood (copy).sh
├── bot.py             # IRC bot (bonus)
├── Makefile           # Build configuration
└── README.md          # This file
```

## Authors

- **cauffret** - Server architecture, epoll implementation
- **chsauvag** - Channel management, protocol implementation, command handling
- **adi-marc** - Client handling, testing, bot implementation

## License

This project is part of the 42 school curriculum and is intended for educational purposes.
