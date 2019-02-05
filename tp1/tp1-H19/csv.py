import pandas as pd
data = pd.read_csv('./comparaison.csv')

meanTimeCounting = 0
meanTimeQuick = 0
meanTimeQuickSeuil = 0
meanTimeQuickRdmSeuil = 0

meanTimeCountingSerieA = 0
meanTimeCountingSerieB = 0
meanTimeCountingSerieC = 0

meanTimeQuickSerieA = 0 
meanTimeQuickSerieB = 0 
meanTimeQuickSerieC = 0 

meanTimeQuickSeuilSerieA = 0 
meanTimeQuickSeuilSerieB = 0
meanTimeQuickSeuilSerieC = 0

meanTimeQuickRdmSeuilSerieA = 0
meanTimeQuickRdmSeuilSerieB = 0
meanTimeQuickRdmSeuilSerieC = 0

group = []
timeQuickData = []
timeCountingData = []
timeQuickSeuilData = []
timeQuickRdmSeuilData = []

for k in range(2):
    for i in range(3):
        for j in range(9):
            timeCounting = data.iat[j, 2]
            timeQuick = data.iat[j, 3]
            timeQuickSeuil = data.iat[j, 4]
            timeQuickRdmSeuil = data.iat[j, 5]
            meanTimeCounting += timeCounting
            meanTimeQuick += timeQuick
            meanTimeQuickSeuil += timeQuickSeuil
            meanTimeQuickRdmSeuil += timeQuickRdmSeuil
        meanTimeCounting /= 10
        meanTimeQuick /= 10
        meanTimeQuickSeuil /= 10
        meanTimeQuickRdmSeuil /= 10
        if i == 0:
            meanTimeCountingSerieA = meanTimeCounting
            meanTimeQuickSerieA = meanTimeQuick
            meanTimeQuickSeuilSerieA = meanTimeQuickSeuil
            meanTimeQuickRdmSeuilSerieA = meanTimeQuickRdmSeuil
            timeCountingData.append(meanTimeCountingSerieA)
            timeQuickData.append(meanTimeQuickSerieA)
            timeQuickSeuilData.append(meanTimeQuickSeuilSerieA)
            timeQuickRdmSeuilData.append(meanTimeQuickRdmSeuilSerieA)
            group.append(groupInt)
            groupInt += 1
        if i == 1:
            meanTimeCountingSerieB = meanTimeCounting
            meanTimeQuickSerieB = meanTimeQuick
            meanTimeQuickSeuilSerieB = meanTimeQuickSeuil
            meanTimeQuickRdmSeuilSerieB = meanTimeQuickRdmSeuil
            timeCountingData.append(meanTimeCountingSerieB)
            timeQuickData.append(meanTimeQuickSerieB)
            timeQuickSeuilData.append(meanTimeQuickSeuilSerieB)
            timeQuickRdmSeuilData.append(meanTimeQuickRdmSeuilSerieB)
            group.append(groupInt)
            groupInt += 1

        if i == 2:
            meanTimeCountingSerieC = meanTimeCounting
            meanTimeQuickSerieC = meanTimeQuick
            meanTimeQuickSeuilSerieC = meanTimeQuickSeuil
            meanTimeQuickRdmSeuilSerieC = meanTimeQuickRdmSeuil
            timeCountingData.append(meanTimeCountingSerieC)
            timeQuickData.append(meanTimeQuickSerieC)
            timeQuickSeuilData.append(meanTimeQuickSeuilSerieC)
            timeQuickRdmSeuilData.append(meanTimeQuickRdmSeuilSerieC)
            group.append(groupInt)
            groupInt += 1

d = {'group': group,
    'timeCounting': timeCountingData,
    'timeQuick': timeQuickData,
    'timeQuickSeuil': timeQuickSeuilData, 
    'timeQuickRdmSeuil': timeQuickRdmSeuilData
    }
df = pd.DataFrame(data=d)
df.to_csv("output.csv", index=False, header=False)
print(df)