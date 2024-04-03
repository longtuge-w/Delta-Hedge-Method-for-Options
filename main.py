import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


def plotQ1paths():
    # Read the CSV data into a pandas DataFrame
    data = pd.read_csv("Q1path.csv", header=None)

    # Plotting all paths
    plt.figure(figsize=(10, 6))
    for index, row in data.iterrows():
        plt.plot(row, linewidth=0.5)

    plt.title("Stock Price Paths")
    plt.xlabel("Time Steps")
    plt.ylabel("Stock Price")
    plt.savefig("Q1_path_self.png")
    plt.show()
    plt.close()


def plotQ1HE():
    # Assuming the hedging errors data is in a list called 'hedging_errors'
    hedging_errors = pd.read_csv("Q1HE.csv", header=None).iloc[:,:100]
    hedging_errors = hedging_errors.values.reshape(-1)

    # Plotting the distribution
    plt.figure(figsize=(10, 6))
    sns.histplot(hedging_errors, kde=True, bins=30)
    plt.title("Distribution of Hedging Errors")
    plt.xlabel("Hedging Error Value")
    plt.ylabel("Frequency")
    plt.grid(True)
    plt.savefig("Q1_HE_self.png")
    plt.show()
    plt.close()


def plotQ1Option():
    # Load the data
    data = pd.read_csv("Q1option_values.csv", header=None).T

    # Plotting all paths
    plt.figure(figsize=(10, 6))
    for index, row in data.iterrows():
        plt.plot(row, linewidth=0.5)

    plt.title("Option Price Paths")
    plt.xlabel("Time Steps")
    plt.ylabel("Option Price")
    plt.savefig("Q1_Option.png")
    plt.show()
    plt.close()


if __name__ == "__main__":

    # plotQ1paths()
    # plotQ1HE()
    plotQ1Option()