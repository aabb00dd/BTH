# Import necessary libraries
import pandas as pd
from sklearn.tree import DecisionTreeClassifier
import sklearn.metrics as score
from sklearn import preprocessing
from sklearn.model_selection import train_test_split


def read(file_name):
    """
    Reads a CSV file, drops rows with missing values,
    and encodes categorical columns.
    """
    try:
        data_frame = pd.read_csv(file_name, sep=",")
        new_df = data_frame.dropna()
        try:
            cols = ["Sex", "Ticket", "Fare", "Embarked"]
            for i in cols:
                label_encoder = preprocessing.LabelEncoder()
                new_df[i] = label_encoder.fit_transform(new_df[i])
            return new_df
        except:
            return new_df
    except:
        empty_df = pd.DataFrame()
        return empty_df


def knn_classifier(data):
    """
    Trains a Decision Tree Classifier on the provided data
    and evaluates its performance.
    """
    num_of_cols = len(data.axes[1])
    rows = data.iloc[:, 2:num_of_cols]
    X = rows.values.tolist()
    Y = list(data.iloc[:, 1])
    dtree = DecisionTreeClassifier(criterion='entropy', random_state=0)
    trainX, testX, trainY, testY = train_test_split(X, Y, test_size=0.3, random_state=0)
    dtree.fit(trainX, trainY)
    y_pred = dtree.predict(testX)
    prec = score.precision_score(testY, y_pred)
    acc = score.accuracy_score(testY, y_pred)
    rec = score.recall_score(testY, y_pred)
    return rec, acc, prec


if __name__ == "__main__":
    inputted_string = input("skriv csv fil här: ")
    new_data = read(inputted_string)
    print("Recall: ", knn_classifier(new_data)[0])
    print("Accuracy: ", knn_classifier(new_data)[1])
    print("Precision: ", knn_classifier(new_data)[2])
