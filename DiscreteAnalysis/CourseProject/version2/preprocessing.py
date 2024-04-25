import pandas as pd
from bs4 import BeautifulSoup
import re
import nltk
import spacy
from string import punctuation
from nltk.corpus import stopwords
from nltk.stem.wordnet import WordNetLemmatizer
from sklearn.model_selection import train_test_split
nltk.download('stopwords')

REPLACE_BY_SPACE_RE = re.compile('[/(){}\[\]|@,;]')
BAD_SYMBOLS_RE = re.compile('[^0-9a-z #+_]')
STOPWORDS = set(stopwords.words('english'))

def most_common(tags, tags_features):
    """Function to filter rows in dataset with specified tags"""
    filtered_tags = []
    for i in range(len(tags)):
        if tags[i] in tags_features:
            filtered_tags.append(tags[i])
    return filtered_tags

def lemmatizing(text, nlp):
    """Lemmatizing function"""
    doc = nlp(text)
    lemmatized = " ".join([token.lemma_ for token in doc])
    return lemmatized

def text_prepare(text):
    """
        text: a string
        return: modified initial string
    """
    text = text.lower()
    text = re.sub(REPLACE_BY_SPACE_RE, " ", text)
    text = re.sub(BAD_SYMBOLS_RE, "", text)
    text = text.split()
    return ' '.join([i for i in text if i not in STOPWORDS])


def main():
    # Reading csv to dataframe
    question = pd.read_csv('materials/Dataset/Questions.csv', encoding="ISO-8859-1")
    tags = pd.read_csv('materials/Dataset/Tags.csv', encoding="ISO-8859-1")

    # Deleting not needed columns
    grouped_tags = tags.groupby("Id")['Tag'].apply(lambda x: ' '.join(x.dropna().astype(str))).reset_index()
    question.drop(columns=['OwnerUserId', 'CreationDate', 'ClosedDate'], inplace=True)

    # Merging questions with tags
    df = question.merge(grouped_tags, on="Id")

    # WARNING!: Adjust 'score' to change dataset size ! (Less 'score' means that StackOverflow questions in dataset starts from 'score' points)
    score = 5
    new_df = df[df['Score'] > score].copy()
    print("Dataset shape: ", new_df.shape)

    # Delete not needed columns
    new_df.drop(columns=['Id', 'Score'], inplace=True)

    # Make Tag column consists of list of tags
    new_df['Tag'] = new_df['Tag'].apply(lambda x: x.split())

    # Calculate the most frequent tags
    # WARNING!: Adjust 'most' to make classification for 'most' tags
    most = 15
    flat_list = [item for sublist in new_df['Tag'].values for item in sublist]
    keywords = nltk.FreqDist(flat_list)
    keywords = nltk.FreqDist(keywords)
    frequencies_words = keywords.most_common(most)
    tags_features = [word[0] for word in frequencies_words]
    print("Top tags: ", tags_features)

    # Filter rows with specified tags above
    new_df['Tag'] = new_df['Tag'].apply(lambda x: most_common(x, tags_features) if len(most_common(x, tags_features)) == len(x) else None)
    new_df.dropna(subset=['Tag'], inplace=True)

    # Convert html format to text
    new_df['Body'] = new_df['Body'].apply(lambda x: BeautifulSoup(x, 'html.parser').get_text())
    new_df = new_df.reset_index()
    new_df.drop(columns=["index"])

    # Lemmatizing, deleting stop words and punctuation in title and text in body
    nlp = spacy.load("en_core_web_sm")
    new_df['Body'] = new_df['Body'].apply(lambda x: text_prepare(x))
    new_df['Body'] = new_df['Body'].apply(lambda x: lemmatizing(x, nlp))
    new_df['Title'] = new_df['Title'].apply(lambda x: text_prepare(x))
    new_df['Title'] = new_df['Title'].apply(lambda x: lemmatizing(x, nlp))

    #WARNING!: Change test_size if u want (Make train and validation parts of dataset)
    test_size = 0.1
    train_df, valid_df = train_test_split(new_df, test_size=test_size, random_state=42)

    # WARNING!: Don't forget to change filename - (Writing training set to file)
    with open('materials/train_xxl.txt', 'w') as file:
        for _, row in train_df.iterrows():
            file.write("1\n")
            file.write(' '.join(row['Tag']) + '\n')
            file.write(row['Title'] + '\n')
            file.write(row['Body'] + '\n')

    # WARNING!: Don't forget to change filename - (Writing validation set and answers to them to file)
    with open('materials/valid_xxl.txt', 'w') as file, open('materials/valid_xxl_ans.txt', 'w') as answers:
        for _, row in valid_df.iterrows():
            file.write("1\n")
            answers.write(' '.join(row['Tag']) + '\n')
            file.write(row['Title'] + '\n')
            file.write(row['Body'] + '\n')

if __name__ == '__main__':
    main()
