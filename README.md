👥 The Split

Person A: The Network Architect

    Focus: Low-level socket programming, memory management, and client connectivity.

    Goal: Ensure data arrives reliably, clients stay connected, and the server never crashes.

    Key Challenge: Implementing the poll() loop correctly and handling partial data packets (buffering).

Person B: The Protocol Logic

    Focus: IRC standard implementation, command parsing, and state management.

    Goal: Ensure the server behaves like an actual IRC server (channels, modes, permissions).

    Key Challenge: Managing the complex relationships between users and channels (e.g., who is operator of what).

📅 Execution Plan by Chapter

Here is how you can map the subject's requirements to your split duties.
Chapter 1: Initialization & Infrastructure

    Person A:

        Create the main Server class. Setup socket(), bind(), listen() based on the port argument.

        Implement the main infinite loop using only one poll() (or epoll/kqueue).

        Handle accept() to create new client file descriptors and add them to the poll set.

    Person B:

        Create the Client class (to hold socket FD, nickname, username, authenticated status).

        Create the Channel class (to hold list of clients, operators, topic, modes).

        Write a Parser function: it should take a raw string (e.g., PRIVMSG #chan :hello) and split it into a structured command object (prefix, command, arguments).

Chapter 2: Buffering & Authentication

    Person A:

        Crucial Step: Implement the buffering system. TCP streams can arrive in chunks (e.g., half a command). You must accumulate data in a buffer until you find a \n or \r\n before sending it to B's parser.

        Handle client disconnection (cleaning up FDs and memory) without crashing.

    Person B:

        Implement the Authentication flow:

            PASS: Verify the password matches the server argument.

            NICK: Set user nickname (check for collisions).

            USER: Register the username/realname.

        Ensure a user cannot send other commands until registered.

Chapter 3: Communications (The "Chat" Part)

    Person A:

        Implement the send() logic. Ensure you don't block the server if a client's write buffer is full (advanced non-blocking I/O).

        Integrate B's command responses into the main loop to actually send data back to the clients.

    Person B:

        Implement PRIVMSG:

            Targeting a user: Find the target client and format the message.

            Targeting a channel: Iterate through the Channel's client list and format the message for everyone except the sender.

        Implement JOIN: Logic to create a channel if it doesn't exist, or add the user if it does.

Chapter 4: Administration & Modes

    Person A:

        Stress test the server. Use nc (netcat) to send partial commands (e.g., nc -C logic) and ensure the buffering works.

        Check for memory leaks (valgrind) on client disconnects.

    Person B:

        Implement Operator commands:

            KICK: Remove a user from a channel.

            INVITE: Add a user to the invited list.

            TOPIC: View or change the channel topic.

        Implement MODE with required flags:

            i (invite-only), t (topic restricted), k (password), o (operator privilege), l (user limit).

🚀 Integration Tip

Person B does not need to wait for Person A to finish the network part.

    Person B can write the commands and test them by passing hardcoded strings into the parser (e.g., parser("JOIN #42")).

    Person A can focus purely on making sure that when nc sends data, it appears in a std::string buffer inside the C++ program.
