import tune as t
import rhythm as r
import octave as o


def main():
    directory = input(u"请输入目录\n")
    print(directory)
    index = input("请输入歌曲的编号\n")
    t.convert_tune(directory=directory, index=index)
    r.convert_rhythm(directory=directory, index=index)
    o.convert_octave(directory=directory, index=index)
    return


if __name__ == "__main__":
    main()
