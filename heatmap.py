# simple heatmap to illustrate magnetic field data 


import pandas as pd 
import matplotlib.pyplot as plt 
import numpy as np 

# read csv file 

df = pd.read_csv("C:\\Users\\evany\\Documents\\Arduino\\code_for_magnetometer\\data.csv", usecols=['field5', 'field6','field4'])

df.columns = ['Intensity', 'LocX', 'LocY']
cols = df.columns

heatmapData = pd.pivot_table(df, index='LocY', columns='LocX', values='Intensity')

fig, axes = plt.subplots() 
img = axes.imshow(heatmapData)

ticklabels = ['-1', '0', '1']
axes.set_xticks(range(len(cols[1])-1), labels=ticklabels)
axes.set_yticks(range(len(cols[2])-1), labels=ticklabels)

# r for rows, c for columns 
for r in range(heatmapData.shape[0]):
    for c in range(heatmapData.shape[1]):
        vals = heatmapData.iloc[r, c]
        text_vals = f"{vals:.2f}"
        axes.text(c, r, text_vals, ha="center", va="center", color='w')

plt.xlabel('X Location')
plt.ylabel('Y Location')
axes.set_title("Intensity of Magnetic Field in different locations around room")

plt.show()

