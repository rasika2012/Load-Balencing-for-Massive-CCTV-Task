import flask
from flask import Flask

app = Flask(__name__)

from app import routes

from app import app

@app.route('/')
@app.route('/index')
def index():
    return "Hello, World!"