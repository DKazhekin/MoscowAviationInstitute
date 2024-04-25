# Course Project for Discrete Analysis

## Task: Create a program using Naive Bayes algorithm to classify a set of tags to documents

## Instruction to install and run:
* 1. Clone Repository
* 2. Download the dataset: https://www.kaggle.com/datasets/stackoverflow/stacksample/
* 3. Add it to the project in ```version2/materials``` and rename dataset folder with 'Dataset' like: ```version2/materials/Dataset```
* 4. Install all the requirements: ```pip3 install -r requirements.txt``` and ```python -m spacy download en_core_web_sm```
* 5. Change the code in main() function, where comment “WARNING!” is, if you want to.
* 6. Run the script: ```python3 preprocessing.py```
* 7. Open ```nb_helper.cpp``` file and change the absolute path to the file on 313 line of code, on that, which is now created by script (with suffix ```_ans```)
* 8. Follow to the Makefile and change relative paths in variable ```INPUT_FILE``` to path to file with prefix ```train```, and in variable ```VALID_FILE``` to path to file with prefix ```valid``` 
* 9. After that you are ready to start: ```make compile```
  ```make learn```
  ```make classify```
  ```make clean```
