def convert_tune(directory, index):
    # output directory
    output = open(directory + r"\tune_" + str(index) + ".out", 'w')
    # input directory
    f = open(directory + r"\tune_" + str(index) + ".in", 'r').read()
    # example: 123 -> DO, RE, MI,
    for char in f:
        if char == '0':
            print("REST", end=', ', file=output)
        elif char == '1':
            print("DO", end=', ', file=output)
        elif char == '2':
            print("RE", end=', ', file=output)
        elif char == '3':
            print("MI", end=', ', file=output)
        elif char == '4':
            print("FA", end=', ', file=output)
        elif char == '5':
            print("SOL", end=', ', file=output)
        elif char == '6':
            print("LA", end=', ', file=output)
        elif char == '7':
            print("SI", end=', ', file=output)
        else:
            print(char, end='', file=output)
