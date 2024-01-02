import json
import pytz
import datetime

class DB:

	# class builder
	def __init__ (self, db_path: str):
		# save the db_path
		self.db_path = db_path
		# open the db file
		with open(self.db_path + "data.json", "r") as f:
			# save the info
			self.dic = json.load(f)


	# select items
	def __getitem__ (self, key: str):
		try:
			return self.dic[key]
		except:
			raise KeyError(f"DB Error: key not found: {key}")

	# function for time, for the log format
	def __get_time(self):
		# Get the current date and time in UTC
		utc_now = datetime.datetime.utcnow()
		# Create a timezone object for CDMX (UTC-5)
		cdmx_tz = pytz.timezone('America/Mexico_City')
		# Convert the UTC time to CDMX time
		cdmx_now = utc_now.replace(tzinfo=pytz.utc).astimezone(cdmx_tz)
		# Print the current date and time in CDMX time
		return str(cdmx_now.strftime('[%Y-%m-%d - %H:%M:%S]'))

	# save logs
	def log(self, msg: str):
		# get the time
		time = self.__get_time()
		# Open a file in append mode
		with open(self.db_path + 'log.txt', 'a') as file:
			# append the line to the log file
			file.write(f'{msg} {time}\n')