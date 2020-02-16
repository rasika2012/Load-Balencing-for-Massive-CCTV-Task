# from flask import Flask, request
# from flask_restful import Resource, Api

# import LoadBanlencer

# app = Flask(__name__)
# api = Api(app)

# class HelloWorld(Resource):
#     def get(self):
        
#         LoadBanlencer.pr()


#         return {'ipdata':LoadBanlencer.ipdata}

#     def post(self):
#         some_json = request.get_json()
#         return {'you sent':some_json},201



# api.add_resource(HelloWorld,'/')

# if __name__ == '__main__':
#     app.run()

# Basic blask server to catch events
from flask import Flask, render_template
from flask.ext.socketio import SocketIO, emit
app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)
@socketio.on('my event')                          # Decorator to catch an event called "my event":
def test_message(message):                        # test_message() is the event callback function.
    emit('my response', {'data': 'got it!'})      # Trigger a new event called "my response" 
                                                  # that can be caught by another callback later in the program.
if __name__ == '__main__':
    socketio.run(app)