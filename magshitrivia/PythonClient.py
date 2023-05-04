import socket
import json

SERVER_IP = "127.0.0.1"
SERVER_PORT = 54000

try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = (SERVER_IP, SERVER_PORT)
    sock.connect(server_address)

except ConnectionRefusedError:
    print("Turn on the server first!")
    exit(1)

do_not_send = False
msg = ""
ls = 0
state = "login"

while ls != 99:
    ls = int(input("0.  Login.             1.  Signup.\n2.  Logout.            3.  Create Room.             4.  Join Room.\n5.  Get HighScores.    6.  Get Players In Room.     7.  Get Rooms.\n8.  Get room state.    9.  Leave room.\n10. Start Game.        11. Close room.\n12. Get Question.      13. Submit Answer.           14. Get Results.\n\n99. Exit.\n"))

    if state == "login":
        if ls == 0:
            msg = json.dumps(
                {
                    "type": "login",
                    "username": input("Username? "),
                    "password": input("Password? ")
                }
            )

        elif ls == 1:
            msg = json.dumps(
                {
                    "type": "signup",
                    "username": input("Username? "),
                    "password": input("Password? "),
                    "email": input("Email? ")
                }
            )

        else:
            print("Invalid, nothing sent")
            do_not_send = True

    elif state == "menu":
        if ls == 2:
            msg = json.dumps(
                {
                    "type": "signout",
                    "username": input("Username? ")
                }
            )

        elif ls == 3:
            msg = json.dumps(
                {
                    "type": "create_room",
                    "room_name": input("Room name? "),
                    "max_users": int(input("Max users? ")),
                    "question_count": int(input("Question count? ")),
                    "answer_timeout": int(input("Answer timeout? "))
                }
            )

        elif ls == 4:
            msg = json.dumps(
                {
                    "type": "join_room",
                    "room_id": int(input("Room ID? "))
                }
            )

        elif ls == 5:
            msg = json.dumps(
                {
                    "type": "get_highscores"
                }
            )

        elif ls == 6:
            msg = json.dumps(
                {
                    "type": "get_players_in_room",
                    "room_id": int(input("Room ID? "))
                }
            )

        elif ls == 7:
            msg = json.dumps(
                {
                    "type": "get_rooms"
                }
            )

        else:
            print("Invalid, nothing sent")
            do_not_send = True

    elif state == "room_admin":
        if ls == 8:
            msg = json.dumps(
                {
                    "type": "get_room_state"
                }
            )

        elif ls == 10:
            msg = json.dumps(
                {
                    "type": "start_game"
                }
            )

        elif ls == 11:
            msg = json.dumps(
                {
                    "type": "close_room"
                }
            )

        else:
            print("Invalid, nothing sent")
            do_not_send = True

    elif state == "room_user":
        if ls == 8:
            msg = json.dumps(
                {
                    "type": "get_room_state"
                }
            )

        elif ls == 9:
            msg = json.dumps(
                {
                    "type": "leave_room"
                }
            )

        else:
            print("Invalid, nothing sent")
            do_not_send = True

    elif state == "in_game":
        if ls == 12:
            msg = json.dumps(
                {
                    "type": "get_question"
                }
            )
        elif ls == 13:
            msg = json.dumps(
                {
                    "type": "submit_answer",
                    "answer_id": int(input("Answer ID? "))
                }
            )
        elif ls == 14:
            msg = json.dumps(
                {
                    "type": "get_game_results"
                }
            )
        else:
            print("Invalid, nothing sent")
            do_not_send = True


    if ls != 99 and not do_not_send:
        print("Sent: " + msg)
        sock.sendall(msg.encode())

        msg = json.loads(sock.recv(2048).decode())
        print("Received: ", msg)

    if not do_not_send and state == "login" and (msg["response"] == "login: success" or msg["response"] == "signup: success"):
        state = "menu"

    elif not do_not_send and state == "menu" and msg["response"] == "signout: success":
        state = "login"

    elif not do_not_send and state == "menu" and msg["response"] == "create_room: success":
        state = "room_admin"

    elif not do_not_send and state == "menu" and msg["response"] == "join_room: success":
        state = "room_user"

    elif not do_not_send and state == "room_admin" and msg["response"] == "close_room: success":
        state = "menu"

    elif not do_not_send and state == "room_user" and msg["response"] == "leave_room: success":
        state = "menu"

    elif not do_not_send and state == "room_user" and msg["response"] == "err_game_started":
        state = "in_game"

    elif not do_not_send and state == "room_admin" and msg["response"] == "start_game: success":
        state = "in_game"

    do_not_send = False
