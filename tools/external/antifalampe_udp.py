import socket

class Lamp(object):

    def __init__(self, addr="antifalampe", port=1234, num_leds=90):
        self.addr = addr
        self.port = port
        self.num_leds = num_leds

        self.leds = [[0, 0, 0] for _ in range(0, self.num_leds)]

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def clear(self):
        for i in range(0, self.num_leds):
            self.leds[i] = [0, 0, 0]

        self.send()

    def set_led(self, i, a, b, c):
        self.leds[i] = [a, b, c]

    def send(self):
        packet = [2]
        for i in range(0, self.num_leds):
            packet.extend(self.leds[i])
        self.sock.sendto(bytes(packet), (self.addr, self.port))

if __name__ == "__main__":
    import time

    l = Lamp()
    l.clear()

    for i in range(0, 90):
        l.set_led(i, 0, 25, 0)
        l.send()

        time.sleep(0.5)