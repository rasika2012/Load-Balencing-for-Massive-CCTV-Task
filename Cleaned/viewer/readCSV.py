import csv
from flask import Flask
from flask import request
import json
from flask_cors import CORS
from datetime import date

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


@app.route('/submitData', methods=['POST'])
def submit():
    print('checked')
    # language = request.args['ip']
    content = request.json
    id = 'CAM0' + str(content['id'])
    ip = content['ip']
    comments = content['comments']
    add = date.today()

    with open('DataFile.csv', 'a+', newline='') as file:
        writer = csv.writer(file)
        writer.writerow([id, ip, add, 'running', comments])

    # print (language['ip'])
    return json.dumps('true')


@app.route('/stream', methods=['POST'])
def meth():
    content = request.json
    ip = content['ip']
    id = "asdf"
    print(ip)
    return json.dumps(id)

        


Flask.run(app, port=9000)


# import flask

# app = flask.Flask(__name__)
# app.config["DEBUG"] = True


# @app.route('/', methods=['GET'])
# def home():
#     return "<h1>Distant Reading Archive</h1><p>This site is a prototype API for distant reading of science fiction novels.</p>"

# app.run()