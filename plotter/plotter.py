import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import pandas
import numpy as np
import sys
import ntpath


def config_plot(xlim=[-0.5, 3.5], ylim=[-10, 120], figSizeX=12.5, figSizeY=8.5):
    plt.xlim(xlim)
    plt.ylim(ylim)
    fig, ax = plt.subplots()  # Create a figure and an axes.
    fig.set_size_inches(figSizeX, figSizeY)
    ax.yaxis.set_major_formatter(mtick.PercentFormatter())


def add_labels(xlabel, ylabel='hit ratio percentage'):
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title('Cache Hit Ratios')
    plt.legend()


# conditions: col_list has the x-axis values in the first column, and every column has a title
# postcondition: plots x_data against the columns in file name and assigns labels to them
def plot(col_list, filename="plotter/EXPERIMENT.csv"):
    dp = pandas.read_csv(filename, usecols=col_list)
    # get values for x-axis from the first column
    x_values = np.array((dp[col_list[0]])).astype(str).tolist()

    for i in range(1, len(col_list)):
        value = np.array(dp[col_list[i]])  # converts data to array
        plt.plot(x_values, value, marker="o", label="MemGen" +
                 col_list[i])  # and use it in the plot


def main():

    if(len(sys.argv) < 3):
        print("use: plotter.py <exp_number> <file_path>.csv <x_col_title>(optional)")
        exit(1)

    if(len(sys.argv) >= 4):
        x_col_title = sys.argv[3]
    elif(int(sys.argv[1]) == 1):
        x_col_title = "line sizes"
    else:
        x_col_title = "ways"

    file_path = sys.argv[2]
    col_list = [x_col_title, "A", "B", "C", "D", "E", "F"]

    config_plot()
    plot(col_list, file_path)
    add_labels(x_col_title)

    file_name = ntpath.basename(file_path)
    output_base_path = "./plots/"
    plt.savefig(output_base_path+file_name.replace("csv", "png"))
    # plt.show()


main()
