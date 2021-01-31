## this is based on https://cdn-learn.adafruit.com/downloads/pdf/led-tricks-gamma-correction.pdf

gamma = 2.8

max_in = 255
max_out = 1

out = ""
for i in range(0, max_in + 1):
    if (i > 0):
        out += ","
    out += "%.10f" % (((i / max_in) ** gamma) * max_out)

print(out)