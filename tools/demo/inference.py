import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from sklearn import metrics
import seaborn as sns

plt.figure(figsize=(8, 7))
sns.set(style='whitegrid', palette='rocket', font_scale=1.5)

filepath = "tools/demo/model_85acc.h5" #85.2% accuracy model
test = pd.read_csv("tools/demo/demodata.csv")
# test = test.drop(columns=["Unnamed: 0"])

# Scaling to between 0, 1. Ensure that each sensor data is scaled by the same factor
cols = {'a':0,'g':0,'m':0}
dims = ['X','Y','Z']

for col in cols:
    cols[col] = max(cols[col], test[col].max())

for col in cols:
    test[col] /= cols[col]
    for dim in dims:
        test[col + dim] /= cols[col]

T = 50
STEP = 1

test_features = test.loc[:,:"gZ"]
test_labels = test.loc[:, "isRoll":]
prepend_features = test_features.iloc[-(T-1):] #check
test_features = pd.concat([prepend_features, test_features], axis=0)

X_test, y_test = [], []
for i in range(0, test_labels.shape[0], 1):
    X_test.append(test_features.iloc[i:i+T].values)
    y_test.append(test_labels.iloc[i])
X_test, y_test = np.array(X_test), np.array(y_test).reshape(-1,4) 

M_TEST = X_test.shape[0]             # number of test examples (2D),full=X_test.shape[0]

from keras.models import load_model
model = load_model(filepath)
y_hat = model.predict(X_test, batch_size=M_TEST, verbose=1)
predicted =np.argmax(y_hat,axis=1)
actual = np.argmax(y_test,axis=1)

confusion_matrix = metrics.confusion_matrix(y_true=actual, y_pred=predicted)

LABELS = ['Rolling','Grasping','Dropping','No Action']

sns.heatmap(confusion_matrix, xticklabels=LABELS, yticklabels=LABELS, annot=True, fmt="d", cmap="rocket")
plt.title("Confusion matrix")
plt.ylabel('True Action')
# plt.ylim((16,14))
plt.xlabel('Predicted Action')
plt.show()

output = pd.concat([test, pd.DataFrame(y_hat,columns=["isRoll_p","isGrasp_p","isDrop_p","isNone_p"])], axis=1)
output.head()
output.to_csv("tools/demo/output.csv")