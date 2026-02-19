import socket
import time

# Configuration
HOST = "localhost"
PORT = 8080
PASSWORD = "test"
BOT_NICKNAME = "irc_bot"

def handle_message(message):
    """Process incoming messages and return appropriate responses"""
    if ">help" in message:
        return ("Figure it out yourself! 😎 Just kidding, here are some commands you can try: "
                ">labubu, >skibidi")
    elif ">labubu" in message:
        return ("OMG LABUBU IS SO CUTE!!! 🎀✨ The pink monster with tiny teeth is everywhere! "
                "Labubu dance! Labubu plushie! Labubu keychain! Labubu takeover the world! "
                "Monster but make it kawaii~ 💖🦷👾")
    elif ">skibidi" in message:
        return ("SKIBIDI DOP DOP DOP YES YES! 🚽💀 Skibidi toilet rizz! Ohio final boss! "
                "Only in Ohio bruh! Camera head vs toilet head! Skibidi bop bop bop! "
                "The most unhinged brainrot content! We live in a simulation! 📹🚽✨")
    elif ">67" in message:
        return ("67 is the best number! It's prime, it's lucky ✨✨ ")
    return None

def run_bot():
    """Main bot function that connects to IRC and listens for messages"""
    irc = None
    try:
        irc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        irc.connect((HOST, PORT))
        print(f"[*] Connected to {HOST}:{PORT}")

        irc.send(f"PASS {PASSWORD}\r\n".encode())
        irc.send(f"NICK {BOT_NICKNAME}\r\n".encode())
        irc.send(f"USER {BOT_NICKNAME} 0 * :IRC Help Bot\r\n".encode())
        time.sleep(1)
        print(f"[*] Authenticated as {BOT_NICKNAME}")

        print("[*] Requesting channel list...")
        irc.send(b"LIST\r\n")
        
        channels_to_join = []
        buffer = ""
        list_complete = False
        
        while not list_complete:
            data = irc.recv(2048).decode('utf-8', errors='ignore')
            if not data:
                break
            
            buffer += data
            lines = buffer.split('\r\n')
            buffer = lines.pop()
            
            for line in lines:
                if not line:
                    continue
                
                print(f"[<] {line}")
                
                if line.startswith("PING"):
                    pong_response = f"PONG {line.split()[1]}\r\n"
                    irc.send(pong_response.encode())
                    print(f"[>] {pong_response.strip()}")
                    continue
                
                if " 322 " in line:
                    parts = line.split()
                    if len(parts) >= 4:
                        channel_name = parts[3]
                        user_count = int(parts[4]) if len(parts) > 4 and parts[4].isdigit() else 0
                        
                        if user_count > 0 and channel_name.startswith('#'):
                            channels_to_join.append((channel_name, user_count))
                
                if " 323 " in line:
                    list_complete = True
                    break
        
        if channels_to_join:
            print(f"[*] Found {len(channels_to_join)} populated channel(s)")
            for channel, user_count in channels_to_join:
                irc.send(f"JOIN {channel}\r\n".encode())
                print(f"[*] Joining {channel} ({user_count} users)")
                time.sleep(0.5)
        else:
            print("[*] No populated channels found")
        
        print(f"[*] Bot is now listening for messages...")


        buffer = ""
        while True:
            try:
                data = irc.recv(2048).decode('utf-8', errors='ignore')
                if not data:
                    print("[!] Connection closed by server")
                    break
                
                buffer += data
                lines = buffer.split('\r\n')
                buffer = lines.pop()

                for line in lines:
                    if not line:
                        continue
                    
                    print(f"[<] {line}")

                    if line.startswith("PING"):
                        pong_response = f"PONG {line.split()[1]}\r\n"
                        irc.send(pong_response.encode())
                        print(f"[>] {pong_response.strip()}")
                        continue

                    if "PRIVMSG" in line:
                        sender = None
                        if line.startswith(':'):
                            sender_part = line[1:].split(' ')[0]
                            sender = sender_part.split('!')[0]
                        
                        parts = line.split("PRIVMSG", 1)
                        if len(parts) == 2:
                            target_and_msg = parts[1].strip()
                            tokens = target_and_msg.split(" :", 1)
                            if len(tokens) == 2:
                                target = tokens[0].strip()
                                message = tokens[1]
                                
                                if sender == BOT_NICKNAME:
                                    continue
                                
                                response = handle_message(message)
                                if response:
                                    reply_target = target if target.startswith('#') else sender
                                    reply = f"PRIVMSG {reply_target} :{response}\r\n"
                                    irc.send(reply.encode())
                                    print(f"[>] Sent response to {reply_target}")

            except socket.timeout:
                continue
            except Exception as e:
                print(f"[!] Error in main loop: {e}")
                break

    except Exception as e:
        print(f"[!] Bot error: {e}")
    finally:
        if irc:
            irc.close()
        print("[*] Bot disconnected")

if __name__ == "__main__":
    print("=" * 50)
    print("IRC Help Bot Starting...")
    print("=" * 50)
    run_bot()
