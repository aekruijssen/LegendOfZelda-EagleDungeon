import json
fileName = "DungeonMap.json"

with open(fileName, 'r') as file:
	data = json.load(file)
	for layer in data["layers"]:
		if "objects" in layer:
			#print(layer["name"])
			outFile = open(layer["name"] + ".csv", 'w')
			outFile.write("Type,x,y,width,height,Destination,Direction,State\n")
			#print("Type,x,y,width,height,Destination,Direction,State")
			for object in layer["objects"]:
				line = object["type"] + ','
				line += str(object["x"]) + ','
				line += str(object["y"]) + ','
				line += str(object["width"]) + ','
				line += str(object["height"]) + ','
				if "properties" in object:
					if len(object["properties"]) == 1:
						line += ',' + object["properties"][0]["value"] + ','
					else:
						line += object["properties"][0]["value"] + ','
						line += object["properties"][1]["value"] + ','
						line += object["properties"][2]["value"]
				else:
					line += ",,"
				#print(line)
				outFile.write(line + '\n')