# Import necessary libraries
import pandas as pd
from sklearn.tree import DecisionTreeClassifier


def menu():
    """
    This function should print out the different menu options
    and take input from the user, then return the entered choice.
    """
    print("""
    1 Read CSV file
    2 Print current data
    3 Add manual data
    4 Build decision tree
    5 Add new observation
    6 Classify test data
    7 Exit program
    """)
    lst = ["1", "2", "3", "4", "5", "6", "7"]
    again = True
    while again:
        inputted_string = input("Your choice: ")
        if inputted_string in lst:
            again = False
    return inputted_string


def load_csv_file(file_name):
    """
    The function should use pandas to load data from a CSV file and
    store it in a pandas DataFrame, which is then returned.
    Error handling should be implemented so that if the file cannot be read,
    an error message is printed and an empty DataFrame is returned.
    """
    try:
        data_frame = pd.read_csv(file_name,  dtype='category', sep=",")
        return data_frame
    except:
        empty_df = pd.DataFrame()
        return empty_df


def add_data(data_frame):
    """
    The purpose of the function is to add manually entered data to the
    submitted DataFrame object as a new row and then return the entire object.
    Except for the identifier, only values that already exist in the DataFrame
    object are allowed in the new row.
    """
    if data_frame.empty:
        print("No data in the frame to add to")
        return data_frame
    else:
        lst = []
        cols = data_frame.axes[1]
        num_of_cols = len(data_frame.axes[1])
        ident = cols[0]
        inputted_ident = input(f"Data identifier ({ident}): ")
        lst.append(inputted_ident)
        count = 1
        while count != num_of_cols:
            rows = list(data_frame.iloc[:, count].cat.categories)
            print(f"{cols[count]}\nPossible values: {rows}")
            again = True
            while again:
                inpuuted_value = input("Input value: ")
                if inpuuted_value in rows:
                    lst.append(inpuuted_value)
                    count += 1
                    again = False
        data_frame.loc[len(data_frame)] = lst
        data_frame.merge(pd.DataFrame(lst), how='cross')
        return data_frame


def build_decision_tree(data_frame):
    """
    The function should use the sklearn module to create a decision tree based
    on the submitted DataFrame object and return it. If the DataFrame object
    is empty, an error message should be printed and None should be returned.
    """
    if data_frame.empty:
        print("Error no data to create tree from")
        return data_frame
    else:
        column_names = data_frame.columns.values.tolist()
        data_frame_numbers = pd.DataFrame()
        for i in column_names:
            data_frame_numbers[i] = data_frame[i].astype("category").cat.codes

        vertical = data_frame_numbers.iloc[:, -1]
        horizontal = data_frame_numbers.iloc[:, 1:-1]
        dtree = DecisionTreeClassifier(criterion='entropy', random_state=0)
        dtree.fit(horizontal, vertical)
        return dtree


def add_observation(data_frame, test_data):
    """
    This function should allow the user to input data to create instances that
    can later be classified, and add it to the DataFrame object test_data,
    which is then returned.
    """
    if data_frame.empty:
        print("No loaded data to create test data for")
        return data_frame

    column_names = data_frame.columns.values.tolist()
    actual_names = column_names[1:-1]
    new_row = {}
    data_frame_numbers = pd.DataFrame()
    for i in column_names:
        data_frame_numbers[i] = data_frame[i].astype("category").cat.codes

    if isinstance(test_data, pd.DataFrame):
        run = True
        counter = 0
        while run and counter < len(column_names[1:-1]):
            current_column = actual_names[counter]
            last_index = (len(data_frame[current_column].unique().tolist()))
            print(f"{current_column} ?")
            print(f"choices: {sorted(data_frame[current_column].unique().tolist())}")
            inputted_choice = input(f"index of option: 0-{last_index-1}:")
            if inputted_choice in map(str, list(range(0, last_index))):
                inputted_choice_lst = [inputted_choice]
                new_row[current_column] = inputted_choice_lst
                counter += 1
            else:
                print("invalid input")
            new_df = pd.DataFrame.from_dict(new_row, orient="columns")
            output = test_data.merge(new_df, how="outer")

    else:
        run = True
        counter = 0
        while run and counter < len(column_names[1:-1]):
            current_column = actual_names[counter]
            last_index = (len(data_frame[current_column].unique().tolist()))
            print(f"{current_column} ?")
            print(f"choices: {sorted(data_frame[current_column].unique().tolist())}")
            inputted_choice = input(f"index of option: 0-{last_index-1}:")
            if inputted_choice in map(str, list(range(0, last_index))):
                inputted_choice_lst = [inputted_choice]
                new_row[current_column] = inputted_choice_lst
                counter += 1
            else:
                print("invalid input")
        new_df = pd.DataFrame.from_dict(new_row, orient="columns")
        output = new_df
        return output


def classify(data_frame, decision_tree, test_data):
    """
    The function should use the decision_tree object to classify the data
    instances in the test_data object, then save and return the result of each
    classification. The results should be in the form of strings representing
    the different possible classifications present in the data_frame.
    """
    if data_frame.empty:
        print("No loaded data to test against was found")
        return data_frame
    if not isinstance(decision_tree, DecisionTreeClassifier):
        print("No loaded data to test against was found")
        return None
    if not isinstance(test_data, pd.DataFrame):
        print("No loaded data to test against was found")
        return None
    if isinstance(test_data, pd.DataFrame) and test_data.empty:
        print("No loaded data to test against was found")
        return None

    lst = []
    predictions = decision_tree.predict(test_data)
    column_names = data_frame.columns.values.tolist()
    predict_lst = data_frame[column_names[-1]].unique().tolist()
    dict = {}
    for i, j in enumerate(predict_lst):
        dict[i] = j
    for prediction in predictions:
        lst.append(dict[prediction])
    return lst


if __name__ == "__main__":
    inputted_file_name = "txt"
    dataframe = pd.DataFrame()
    valid = True
    test = None
    tree = None
    while valid:
        choice = menu()
        if choice == "1":
            inputted_file_name = input("Filename: ")
            dataframe = load_csv_file(inputted_file_name)
        if choice == "2":
            print(dataframe)
        if choice == "3":
            dataframe = add_data(dataframe)
        if choice == "4":
            tree = build_decision_tree(dataframe)
        if choice == "5":
            test = add_observation(dataframe, test)
        if choice == "6":
            answer = classify(dataframe, tree, test)
            if answer is not None:
                print(answer)
        if choice == "7":
            valid = False
