def convert_rhythm(directory, index):
    # output directory
    output = open(directory + r"\rhythm_" + str(index) + ".out", 'w')
    # input directory
    f = open(directory + r"\rhythm_" + str(index) + ".in", 'r').read()
    # example: 48ss8.8.8 -> QUARTER, EIGHTH, SIXTEENTH, SIXTEENTH, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH,
    for i in range(len(f)):
        char1 = f[i]
        if i + 1 < len(f):
            char2 = f[i+1]
        else:
            char2 = ''

        # DOTTED_ is a prefix
        if char2 == '.':
            print("DOTTED_", end='', file=output)

        # you may define your own mark
        if char1 == '.':
            continue
        elif char1 == '1':
            print("WHOLE", end=', ', file=output)
        elif char1 == '2':
            print("HALF", end=', ', file=output)
        elif char1 == '4':
            print("QUARTER", end=', ', file=output)
        elif char1 == '8':
            print("EIGHTH", end=', ', file=output)
        elif char1 == 's':
            print("SIXTEENTH", end=', ', file=output)
        elif char1 == '\n':
            print('', file=output)
