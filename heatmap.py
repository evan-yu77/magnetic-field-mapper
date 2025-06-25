# simple heatmap to illustrate magnetic field data 


import pandas as pd 
import matplotlib.pyplot as plt 
import numpy as np 
from matplotlib.animation import FuncAnimation


# FuncAnimation updates frame by frame, so frame is a required parameter
def heatmap_plot(frame):
    df = pd.read_csv("https://api.thingspeak.com/channels/2986043/feeds.csv?api_key=97HNBX83QIQ34OIJ", usecols=['field5', 'field6','field4'])

    df.columns = ['Intensity', 'LocX', 'LocY']
    heatmapData = pd.pivot_table(df, index='LocY', columns='LocX', values='Intensity')

    plt.clf()

    img = plt.imshow(heatmapData, cmap='viridis')
    plt.xlabel('X Location')
    plt.ylabel('Y Location')
    plt.title("Intensity of Magnetic Field in different locations around room")
    ticklabels = ['-1', '0', '1']

    # set x ticks and y ticks 
    plt.xticks(range(len(heatmapData[1])), labels=ticklabels)
    plt.yticks(range(len(heatmapData[1])), labels=ticklabels)

    # r for rows, c for columns. index by rows and columsn to enter magnetic field data manually
    for r in range(heatmapData.shape[0]):
        for c in range(heatmapData.shape[1]):

            # access rows and columns via iloc() function
            vals = heatmapData.iloc[r, c]
            text_vals = f"{vals:.3f}"
            plt.text(c, r, text_vals, ha="center", va="center", color='w')

fig, axes = plt.subplots()

# updates every 3 seconds
anim = FuncAnimation(fig, heatmap_plot, interval=3000)

plt.show()

