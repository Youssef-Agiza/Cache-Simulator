import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import pandas 
import numpy as np


def main():

    sizes=["16","32","64","128"]
    col_list = ["A", "B", "C", "D", "E", "F"] 
    dp = pandas.read_csv("plotter/EXPERIMENT.csv", usecols=col_list) 
    AValues = np.array(dp["A"])
    BValues = np.array(dp["B"])
    CValues = np.array(dp["C"])
    DValues = np.array(dp["D"])
    EValues = np.array(dp["E"])
    FValues = np.array(dp["F"])    
 

    plt.ylim([-10,120])
    plt.xlim([-0.5,3.5])
    fig, ax = plt.subplots()  # Create a figure and an axes.
    fig.set_size_inches(12.5, 8.5)

    plt.xlabel('cache line size(Bytes)')
    plt.ylabel('hit ratio')
    plt.title('Cache Hit Ratios')

    ax.yaxis.set_major_formatter(mtick.PercentFormatter())
    plt.plot(sizes, AValues,marker="o",label="MemGenA")
    plt.plot(sizes, BValues,marker="o",label="MemGenB")
    plt.plot(sizes, CValues,marker="o",label="MemGenC")
    plt.plot(sizes, DValues,marker="o",label="MemGenD")
    plt.plot(sizes, EValues,marker="o",label="MemGenE")
    plt.plot(sizes, FValues,marker="o",label="MemGenF")
    plt.legend()

    plt.savefig("foo.png")
    #plt.show()


main()
    