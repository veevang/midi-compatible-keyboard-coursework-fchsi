def convert_octave(directory, index):
    # output directory
    output = open(directory + r"\octave_" + str(index) + ".out", 'w')
    # input directory
    f = open(directory + r"\octave_" + str(index) + ".in", 'r').read()
    # example: 01-11 -> 0, 1, -1, 1,
    for char in f:
        if '0' <= char <= '9':
            print(char, end=', ', file=output)
        else:
            print(char, end='', file=output)
    return
