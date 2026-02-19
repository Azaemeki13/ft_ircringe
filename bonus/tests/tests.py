import socket
import threading
import time
import random

#     base name channel name

# Variables 

HOST = "localhost"
PORT = 8080
PASSWORD = "test"
CHANNEL_NAME = "#ok"
CHANNEL_PW = ""
BOT_COUNT = 100
BASE_NAME = "Rocco"
TIMER = 1

def bot_generation(bot_id):
    nickname = f"{BASE_NAME}{bot_id}"
    irc = None
    try:
        irc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        irc.connect((HOST, PORT))

        username = f"{bot_id}"
        realname = f"Bot Number {bot_id}"

        irc.send(f"PASS {PASSWORD}\r\n".encode())
        irc.send(f"NICK {nickname}\r\n".encode())
        irc.send(f"USER {username} 0 * :{realname}\r\n".encode())
        time.sleep(TIMER)
        if CHANNEL_PW == "":
            irc.send(f"JOIN {CHANNEL_NAME}\r\n".encode())
            time.sleep(TIMER)
        else:
            irc.send(f"JOIN {CHANNEL_NAME} {CHANNEL_PW}\r\n".encode())
            time.sleep(TIMER)
        msg2 = f"Hey babygirl, I'm {nickname}"
        while True:
            irc.send(f"PRIVMSG {CHANNEL_NAME} :{msg2}\r\n".encode())
            time.sleep(20)
            try:
                irc.settimeout(0.1)
                data = irc.recv(2048)
                if not data:
                    break
                if data.startswith(b"PING"):
                    irc.send(b"PONG " + data.split()[1] + b"\r\n")
            except socket.timeout:
                pass
            except Exception:
                break
    except Exception as e:
        print(f"Bot {nickname} has died {e}")
    finally:
        irc.close()


thread = []
print(f"Launching {BOT_COUNT} bots to {HOST}{PORT}...")
for i in range(BOT_COUNT):
    t = threading.Thread(target=bot_generation, args=(i,))
    thread.append(t)
    t.start()
    time.sleep(0.1)
