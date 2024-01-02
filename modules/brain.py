# libraries
import json
# modules
import modules.http as http
from modules.db import DB

# instance the db
db = DB("db/")

# function to receive and process the message
def get_content(message:str):
	# extract the data from the message
	_, sender, receiver, content = message.split('-')
	# check that lambda is the sender
	if sender != 'lambda':
		# save a log
		db.log('[Error] Message not from lambda: ' + message)
		# and throw an error
		raise ValueError('Error: sender is not lambda')
	
	db.log('[Message] Message from lambda: ' + content)
	# then return the values as the sender is lambda
	return json.loads(content)


# main function, like the internal brain of the app
# similar to lambda's one
class Brain:

	def __init__ (self):
		# instance a db
		self.db = db

	def __call__ (self, message: str):
		# once get the message, try to get the content
		content = ''
		# get the content
		try:
			content = get_content(message)
		except:
			self.db.log("[Error] couldn't process the content: " + message)
			raise ValueError("Error on message content")

		# having the content, get the alias
		alias = content['alias']
		# get the device data
		device = self.db['devices'][alias]
		# once having the correct one, make a verification:
		# there can not be an output message on a local input
		if content['type'] == "output" and device['type'] == "input":
			# throw an error
			raise TypeError("Type Error: an OUTPUT request for an INPUT defined device")
		# once passing the type verification
		# try to make the request
		try:
			# return the data sent from the arduino
			return http.get(device['ip'], content)
		except:
			# throw an error
			self.db.log(f'System Error: request to {device["ip"]} failed, on devide: "{alias}"')
			raise OSError(f'System Error: request to {device["ip"]} failed, on devide: "{alias}"')