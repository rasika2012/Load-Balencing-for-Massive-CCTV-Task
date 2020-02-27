import requests

import time
while True:
    r = requests.get("http://127.0.0.1:9000/load")
    print (r.content)
    time.sleep(0.1)