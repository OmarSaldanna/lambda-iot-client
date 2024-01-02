# this module will handle the http reqiests to the arduinos
import requests
import json

def get(ip: str, data: dict):
	# set the url
	url = f"http://{ip}/iot"
	# this is for the arduino
	params = {"plain": json.dumps(data)}
	# make the request
	response = requests.get(url, params=params, timeout=2)
	# return the answer
	return response.text