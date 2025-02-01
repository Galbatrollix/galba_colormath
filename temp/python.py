def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]


with open("2000_output.txt") as infile:
	dupa = infile.readlines();

dupa = [item.strip() for item in dupa]
lines = []
for chunk in chunks(dupa, 5):
	chunk.append('\n');
	lines.append(" ".join(chunk))
print(lines)

with open("temp.txt", "w") as outfile:
	outfile.writelines(lines)