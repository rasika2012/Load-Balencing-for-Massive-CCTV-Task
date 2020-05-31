import csv
from flask import Flask
import json
from flask_cors import CORS

app = Flask(__name__)
app.config["debug"] = True
CORS(app)


@app.route('/data', methods=['GET'])
def home():
    wholeFile = []
    with open('DataFile.csv','r') as csv_file: #Opens the file in read mode
        csv_reader = csv.reader(csv_file) # Making use of reader method for reading the file
    
    
        for line in csv_reader: #Iterate through the loop to read line by line
            wholeFile.append(line)
            
    return json.dumps(wholeFile)

Flask.run(app, port=9000)

# import flask

# app = flask.Flask(__name__)
# app.config["DEBUG"] = True


# @app.route('/', methods=['GET'])
# def home():
#     return "<h1>Distant Reading Archive</h1><p>This site is a prototype API for distant reading of science fiction novels.</p>"

# app.run()