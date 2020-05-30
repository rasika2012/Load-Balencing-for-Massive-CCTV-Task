
import json
def setup_config():
    print('hello')
    conf = open("./config.json","r") 
    lines = json.loads(str(conf.read()))
    print(lines['configs']['ips'])

setup_config()