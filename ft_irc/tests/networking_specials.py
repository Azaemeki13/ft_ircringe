GREEN = '\033[92m'
RED = '\033[91m'
YELLOW = '\033[93m'
CYAN = '\033[96m'
RESET = '\033[0m'

import socket
import time

HOST = "127.0.0.1"
PORT = 6969
PASS = "test"

def assert_receive(sock, expected_text, test_name):
    """Like assert_response, but used to check if a target RECEIVED a message."""
    sock.settimeout(2.0)
    full_response = ""
    try:
        while True:
            chunk = sock.recv(4096).decode('utf-8')
            if not chunk: break
            full_response += chunk
            if expected_text in full_response:
                print(f"{GREEN}[SUCCESS] {test_name} -> Target received message")
                return
    except socket.timeout:
        pass
    print(f"{RED}[FAILED]  {test_name} -> Target did not receive '{expected_text}'. Buffer contained:\n{full_response}")

def test_privmsg_variations():
    print(f"\n{YELLOW}--- Testing PRIVMSG Parameters & Delivery ---{RESET}")
    alice = create_client("Alice")
    bob = create_client("Bob")
    
    # 1. Error: Missing Receiver
    alice.send(b"PRIVMSG\r\n")
    assert_response(alice, "411", "Missing Receiver (411)")
    
    # 2. Error: Missing Text
    alice.send(b"PRIVMSG Bob\r\n")
    assert_response(alice, "412", "Missing Text (412)")
    
    # 3. Error: Ghost Target
    alice.send(b"PRIVMSG Casper :You there?\r\n")
    assert_response(alice, "401", "Non-existent Target (401)")
    
    # 4. Success: User to User
    alice.send(b"PRIVMSG Bob :Hello Bob!\r\n")
    assert_receive(bob, "PRIVMSG Bob :Hello Bob!", "User-to-User Delivery")
    
    # 5. Success: User to Channel
    alice.send(b"JOIN #chat\r\n")
    bob.send(b"JOIN #chat\r\n")
    time.sleep(0.1)
    
    alice.send(b"PRIVMSG #chat :Hello Channel!\r\n")
    assert_receive(bob, "PRIVMSG #chat :Hello Channel!", "User-to-Channel Delivery")

    alice.close(); bob.close()

def test_operator_privileges():
    print(f"\n{YELLOW}--- Testing Operator Privileges (KICK, TOPIC, INVITE, MODE) ---{RESET}")
    op = create_client("OpBot")
    op.send(b"JOIN #ops\r\n") # Joins first, gets Operator status
    time.sleep(0.1)
    
    # --- NEW: OpBot explicitly turns on Topic Protection (+t) ---
    op.send(b"MODE #ops +t\r\n")
    time.sleep(0.1)
    # ------------------------------------------------------------
    
    pleb = create_client("PlebBot")
    pleb.send(b"JOIN #ops\r\n") # Joins second, regular user
    time.sleep(0.1)

    # Pleb tries to do Operator things (All should fail with 482)
    pleb.send(b"KICK #ops OpBot :Mutiny!\r\n")
    assert_response(pleb, "482", "Pleb KICK attempt (482)")
     
    pleb.send(b"TOPIC #ops :Anarchy\r\n")
    assert_response(pleb, "482", "Pleb TOPIC attempt (482)")
    
    pleb.send(b"MODE #ops +i\r\n")
    assert_response(pleb, "482", "Pleb MODE attempt (482)")
    
    pleb.send(b"INVITE RandomGhost #ops\r\n")
    assert_response(pleb, "482", "Pleb INVITE attempt (482)")

    op.close(); pleb.close()

def create_client(nickname):
    """Helper to create a fully registered socket."""
    s = socket.socket()
    s.connect((HOST, PORT))
    s.send(f"PASS {PASS}\r\nNICK {nickname}\r\nUSER {nickname} 0 * :{nickname}\r\n".encode())
    s.settimeout(0.5)
    try:
        s.recv(4096)
    except socket.timeout:
        pass
    return s

def assert_response(sock, expected_code, test_name):
    sock.settimeout(2.0)
    full_response = ""
    try:
        while True:
            chunk = sock.recv(4096).decode('utf-8')
            if not chunk:
                break
            full_response += chunk
            if expected_code in full_response:
                print(f"{GREEN}[SUCCESS] {test_name} -> Got {expected_code}")
                return
    except socket.timeout:
        pass
    print(f"{RED}[FAILED]  {test_name} -> Expected {expected_code}, got:\n{full_response}")

# --- 1. CHANNEL & PRIVILEGE ERRORS ---

def test_channel_permissions():
    print(f"{YELLOW}\n--- Testing Channel & Operator Permissions ---")
    
    # User 1 creates and joins the channel (Becomes Operator)
    op = create_client("AdminBot")
    op.send(b"JOIN #secret\r\n")
    time.sleep(0.1)
    
    # User 2 joins normally
    pleb = create_client("PlebBot")
    pleb.send(b"JOIN #secret\r\n")
    time.sleep(0.1)
    
    # User 3 doesn't join at all, tries to send a message
    outsider = create_client("Outsider")
    outsider.send(b"PRIVMSG #secret :Let me in!\r\n")
    assert_response(outsider, "442", "PRIVMSG without joining (442 Not on channel)")
    
    # User 2 tries to KICK User 1 without being an Op
    pleb.send(b"KICK #secret AdminBot :I am taking over!\r\n")
    assert_response(pleb, "482", "KICK without Operator privileges (482 You're not channel operator)")
    
    # Cleanup
    op.close(); pleb.close(); outsider.close()

# --- 2. NETWORK DISTURBANCES ---

def test_partial_command():
    print(f"{YELLOW}\n--- Testing Fragmented / Partial Commands ---")
    s = socket.socket()
    s.connect((HOST, PORT))
    s.send(f"PASS {PASS}\r\n".encode())
    
    # Send a fragmented NICK command
    s.send(b"NI")
    time.sleep(1) # OS delays packet
    s.send(b"CK Fragment\r\n")
    
    s.send(b"USER frag 0 * :frag\r\n")
    time.sleep(0.1)
    
    # If the server correctly reassembled "NICK Fragment", it will accept a PING
    s.send(b"PING :hello\r\n")
    
    # --- FIX: Read continuously until we see PONG ---
    s.settimeout(2.0)
    full_resp = ""
    try:
        while True:
            chunk = s.recv(4096).decode('utf-8')
            if not chunk:
                break
            
            full_resp += chunk
            
            if "PONG" in full_resp or "hello" in full_resp:
                print(f"{GREEN}[SUCCESS] Partial Command -> {CYAN}Server reassembled 'NICK' successfully!")
                s.close()
                return
    except socket.timeout:
        pass # Expected when no more data is coming
        
    print(f"{RED}[FAILED]  Partial Command -> {CYAN}Expected PONG, but got:\n", full_resp)
    s.close()

def test_ctrl_z_simulation():
    print(f"{YELLOW}\n--- Testing Suspended Client (Ctrl+Z) & Buffer Limits ---")
    
    # Create the victim and severely limit its OS receive buffer (simulates a frozen client instantly)
    victim = create_client("FrozenBot")
    victim.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1024) 
    victim.send(b"JOIN #flood\r\n")
    time.sleep(0.1)
    
    spammer = create_client("SpamBot")
    spammer.send(b"JOIN #flood\r\n")
    time.sleep(0.1)

    print(f"{YELLOW}[*] FrozenBot is sleeping (not calling recv). SpamBot is firing 500 messages...")
    
    # Flood the channel. Because Victim is frozen, the C++ server's send() will return EAGAIN
    # and push all these into the C++ `addTowBuffer`.
    for i in range(500):
        try:
            spammer.send(f"PRIVMSG #flood :This is heavy spam message {i} filling the buffer!\r\n".encode())
        except Exception:
            pass # Spammer might get blocked if server stops reading
            
    print(f"{YELLOW}[*] Flood sent. Checking if server is still alive for NEW users...")
    
    # The ultimate test: Did the server hang?
    try:
        test_alive = create_client("PingBot")
        test_alive.send(b"PING :are_you_dead\r\n")
        test_alive.settimeout(2.0)
        resp = test_alive.recv(1024).decode('utf-8')
        if "PONG" in resp:
            print(f"{GREEN}[SUCCESS] Ctrl+Z Flood -> {CYAN}Server is STILL ALIVE and responding to others!")
        test_alive.close()
    except Exception:
        print(f"{RED}[FAILED]  Ctrl+Z Flood -> {CYAN}Server hung or crashed! (Check your EAGAIN logic)")
        
    # Cleanup
    victim.close()
    spammer.close()

if __name__ == "__main__":
    print(f"{CYAN}=====================================")
    print(f"{RED}42 IRC SERVER INTEGRATION TEST SUITE")
    print(f"{CYAN}=====================================")
    test_channel_permissions()
    time.sleep(1)
    test_partial_command()
    time.sleep(1)
    test_ctrl_z_simulation()
    time.sleep(1)
    test_privmsg_variations()
    time.sleep(1)
    test_operator_privileges()
    print("\nAll automated tests finished.")
