import matplotlib.pyplot as plt


def main():


        R = 4.5/2

        radius = [ 0.21, 0.23, 0.235, 0.300, 0.375, 0.450,
                  0.675, 0.900, 1.025, 1.125, 1.225, 1.350, 1.475,
                  1.575, 1.675, 1.800, 2.025, 2.165, 2.193,
                  2.232, 2.250]

        radius = [r/R for r in radius] 

        twist = [ 0, 0, 0, 0, 8.2, 16.4, 12.1, 8.3, 7.1, 6.1,
                  5.5, 4.8, 4.0, 3.7, 3.2, 2.6, 1.5, 0.7, 0.469,
                  0.231, 0.0]

        chord = [ 195, 195, 90, 90, 165, 240, 207, 178, 166, 158,
                  150, 142, 134, 129, 123, 116, 102, 92, 82, 56, 11]

        chord = [c/1000 for c in chord]
        chord = [c/R for c in chord]


        x = radius
        y = chord


        print(chord)
        print(radius)

        plt.plot(x,y)
        plt.xlabel("r/R")
        plt.ylabel("Chord/R")
        plt.grid()
        plt.show()


        plt.plot(x,twist)
        plt.xlabel("r/R")
        plt.ylabel("twist")
        plt.grid()


        plt.show()




        return


if __name__ == "__main__":
    main()
