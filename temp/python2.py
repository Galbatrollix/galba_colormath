import json


def to_lines(json_data):
	result = []
	for item in json_data:
		item = [round(x, 5) for x in item]
		item_str = f'{item}'[1:-2]
		item_str = '{' + item_str + '}'
		result.append(item_str)
	return result


with open("2000_input1.json") as infile:
	ones = json.load(infile)


with open("2000_input2.json") as infile:
	twos = json.load(infile)


lines1 = to_lines(ones)
lines2 = to_lines(twos)
result = []
for left, right in zip(lines1, lines2):
	result.append("{" + left + ', ' + right + '},\n')

with open("temp2.txt", "w") as outfile:
	outfile.writelines(result)